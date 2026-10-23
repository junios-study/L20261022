// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"

void ULobbyWidgetBase::NativeOnInitialized()
{
}

void ULobbyWidgetBase::OnStart()
{

}

void ULobbyWidgetBase::ShowStartButton()
{
	StartButton->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(StartButtonAnimation,
		0,
		1,
		EUMGSequencePlayMode::Forward,
		1,
		false
	);
}
