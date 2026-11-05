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

}


bool UVoiceSessionSubSystyem::StartTalking()
{
	return true;
}

void UVoiceSessionSubSystyem::StopTalking()
{

}

void UVoiceSessionSubSystyem::HandleCreateComplete(FName SessionName, bool bSuccess)
{

}


void UVoiceSessionSubSystyem::ClearCreateDelegate()
{

}

