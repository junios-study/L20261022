// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"
#include "Kismet/KismetSystemLibrary.h"

ALobbyPC::ALobbyPC()
{
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
			}
		}
	}
}


