// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceSessionSubSystyem.h"
#include "Engine/World.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "OnlineSubsystemUtils.h"


void UVoiceSessionSubSystyem::CreateVoiceSession()
{
	if (bCreating)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Session"));
		return;
	}

	if (bVoiceSessionCreated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already VoiceSessionCreated."));
		return;
	}

	IOnlineSubsystem* OSS = Online::GetSubsystem(GetWorld());

	Sessions = OSS ? OSS->GetSessionInterface() : nullptr;

	if (!Sessions.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("not Session."));
		return;
	}


	if (Sessions->GetNamedSession(NAME_GameSession))
	{
		UE_LOG(LogTemp, Warning, TEXT("already exist Session."));
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = 10;
	Settings.bIsLANMatch = false;
	Settings.bShouldAdvertise = false;
	Settings.bAllowJoinInProgress = true;
	Settings.bUsesPresence = false;
	Settings.bUseLobbiesIfAvailable = false;

	CreateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UVoiceSessionSubSystyem::HandleCreateComplete));
	
	bCreating = true;

	//비동기함수
	const bool bAccept = Sessions->CreateSession(0, NAME_GameSession, Settings);

	if (!bAccept && bCreating)
	{
		ClearCreateDelegate();
		bCreating = false;
		UE_LOG(LogTemp, Warning, TEXT("CreateSession failed."));
		return;
	}
}

void UVoiceSessionSubSystyem::Deinitialize()
{
	StopTalking();
	Voice.Reset();
	ClearCreateDelegate();
	Sessions.Reset();
	bCreating = false;
	bVoiceSessionCreated = false;

	Super::Deinitialize();
}


bool UVoiceSessionSubSystyem::StartTalking()
{
	if (!bVoiceSessionCreated)
	{
		return false;
	}

	if (bTransmissionRequested)
	{
		return true;
	}

	IOnlineSubsystem* OSS = Online::GetSubsystem(GetWorld());
	Voice = OSS ? OSS->GetVoiceInterface() : nullptr;

	if (!Voice.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("voice interface failed."));

		return false;
	}

	if (!Voice->RegisterLocalTalker(0))
	{
		Voice->StopNetworkedVoice(0);
		UE_LOG(LogTemp, Warning, TEXT("microphone not."));
		return false;
	}

	Voice->StartNetworkedVoice(0);
	bTransmissionRequested = true;

	UE_LOG(LogTemp, Warning, TEXT("voice Transmission start."));

	return true;
}

void UVoiceSessionSubSystyem::StopTalking()
{
	if (Voice.IsValid())
	{
		Voice->StopNetworkedVoice(0);
	}

	if (bTransmissionRequested)
	{
		UE_LOG(LogTemp, Warning, TEXT("voice Transmission stop."));
	}

	bTransmissionRequested = false;
}

void UVoiceSessionSubSystyem::HandleCreateComplete(FName SessionName, bool bSuccess)
{
	if (SessionName != NAME_GameSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCreateComplete not same SessionName."));

		return;
	}

	ClearCreateDelegate();
	bCreating = false;
	bVoiceSessionCreated = bSuccess;

	if (bSuccess)
	{
		StopTalking();
	}

	UE_LOG(LogTemp, Warning, TEXT("HandleCreateComplete %s."), bSuccess ? TEXT("Success") : TEXT("Failed"));
}


void UVoiceSessionSubSystyem::ClearCreateDelegate()
{
	if (Sessions.IsValid() && CreateHandle.IsValid())
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateHandle);
	}

	CreateHandle.Reset();
}

