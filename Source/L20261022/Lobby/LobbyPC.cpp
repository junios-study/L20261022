// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"

ALobbyPC::ALobbyPC()
{
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddToViewport();
		LobbyWidgetObject->ShowStartButton();
	}
}


