// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "LobbyGS.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeLeftTime.AddDynamic(this, &ULobbyWidgetBase::UpdateLeftTime);
	}

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

void ULobbyWidgetBase::UpdateLeftTime(int NewLeftTime)
{
	if (LeftTime)
	{
		const FString Time = FString::Printf(TEXT("%d초 남음"), NewLeftTime);
		LeftTime->SetText(FText::FromString(Time));
	}
}
