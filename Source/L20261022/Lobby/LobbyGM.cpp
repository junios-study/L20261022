// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"


ALobbyGM::ALobbyGM()
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGM() %d"), GetNetMode());
}

void ALobbyGM::SpendTime()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->SpendTime();
	}
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay() %d"), GetNetMode());

	//GetWorldTimerManager().SetTimer(
	//	LeftTimerHandle,
	//	FTimerDelegate::CreateWeakLambda(this, [this]()
	//		{
	//			ALobbyGS* GS = GetGameState<ALobbyGS>();
	//			if (GS)
	//			{
	//				GS->SpendTime();
	//			}
	//		}
	//	),
	//	1.0f,
	//	true
	//);

	GetWorldTimerManager().SetTimer(
		LeftTimerHandle,
		this,
		&ALobbyGM::SpendTime,
		1.0f,
		true
	);

}

void ALobbyGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = TEXT("나가");

	UE_LOG(LogTemp, Warning, TEXT("PreLogin Options %s %s"), *Options, *Address);
}

APlayerController* ALobbyGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("Login Options %s"), *Options);

	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGM::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
