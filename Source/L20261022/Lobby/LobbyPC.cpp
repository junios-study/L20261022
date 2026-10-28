// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LobbyGS.h"
#include "LobbyGM.h"
#include "Kismet/GameplayStatics.h"

ALobbyPC::ALobbyPC()
{
}

void ALobbyPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	const FString Message = FString::Printf(TEXT("%s LocalRole %s, RemoteRole %s"),
		*this->GetName(),
		*StaticEnum<ENetRole>()->GetValueAsString(GetLocalRole()),
		*StaticEnum<ENetRole>()->GetValueAsString(GetRemoteRole())
	);

	UKismetSystemLibrary::PrintString(GetWorld(), Message);

	if (IsLocalController())
	{
		LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		if (LobbyWidgetObject)
		{
			LobbyWidgetObject->AddToViewport();

			if (HasAuthority())
			{
				LobbyWidgetObject->ShowStartButton();
				//예외 상황으로 로그인 정보는 위젯보다 먼저 처리됨.
				ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
				if (GS)
				{
					GS->OnRep_ConnectionCount();
					GS->OnRep_LeftTime();
				}
			}
		}
	}

	bShowMouseCursor = true;

	SetInputMode(FInputModeGameAndUI());
}

bool ALobbyPC::C2S_SendMessage_Validate(const FText& Messsage)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText& Messsage)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->S2C_SendMessage(Messsage);
		}
	}
}

void ALobbyPC::S2C_SendMessage_Implementation(const FText& Messsage)
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddMessage(Messsage);
	}
}

void ALobbyPC::S2C_ShowLoadingScreen_Implementation()
{
	ShowLoadingScreen();
}

void ALobbyPC::ShowLoadingScreen()
{
	if (LoadingWidgetClass && !LoadingWidgetObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowLoadingScreen %s"), *GetName());

		LoadingWidgetObject = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		if (LoadingWidgetObject)
		{
			LoadingWidgetObject->AddToViewport();
		}
	}
}

void ALobbyPC::C2S_StartGame_Implementation()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
			if (PC)
			{
				PC->S2C_ShowLoadingScreen();
			}
		}

		GM->OpenInGame();
	}
}


