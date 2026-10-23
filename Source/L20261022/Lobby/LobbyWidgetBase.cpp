// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "LobbyGS.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "LobbyGM.h"

void ULobbyWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeLeftTime.AddDynamic(this, &ULobbyWidgetBase::UpdateLeftTime);
		GS->OnChangeConnectoinCount.AddDynamic(this, &ULobbyWidgetBase::UpdateConnectionCount);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::ProcessTextCommited);
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnStart);
	}
}

void ULobbyWidgetBase::ProcessTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer());
			if (PC)
			{
				//Call Client, Execute Server
				PC->C2S_SendMessage(Text);
			}
			///Text

			if (ChatInput)
			{
				ChatInput->SetText(FText::FromString(TEXT("")));
			}
		}

		case ETextCommit::OnCleared:
		{
			if (ChatInput)
			{
				ChatInput->SetUserFocus(GetOwningPlayer());
			}
		}

	}
}

void ULobbyWidgetBase::OnStart()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OpenInGame();
	}
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

void ULobbyWidgetBase::UpdateConnectionCount(int NewCount)
{
	if (ConnectionCount)
	{
		const FString Time = FString::Printf(TEXT("%d/100명"), NewCount);
		ConnectionCount->SetText(FText::FromString(Time));
	}
}

void ULobbyWidgetBase::AddMessage(const FText& Message)
{
	if (ChatScroll)
	{
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatScroll);
		NewTextBlock->SetText(Message);
		FSlateFontInfo FontInfo = NewTextBlock->GetFont(); 
		FontInfo.Size = 18;
		NewTextBlock->SetFont(FontInfo);
		ChatScroll->AddChild(NewTextBlock);
		ChatScroll->ScrollToEnd();
	}
}