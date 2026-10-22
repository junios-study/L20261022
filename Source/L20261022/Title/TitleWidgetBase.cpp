// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "../DataGIS.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	//LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));
	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessLoginClick);
	}

	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessConnectClick);
	}

	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessStartServerClick);
	}
}

void UTitleWidgetBase::ProcessLoginClick()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		UDataGIS* GIS = GI->GetSubsystem<UDataGIS>();

		if (GIS)
		{
			GIS->UserId = UserID->GetText().ToString();
			GIS->Password = Password->GetText().ToString();
		}
	}


	UE_LOG(LogTemp, Warning, TEXT("Login Click : %s %s"), *UserID->GetText().ToString(), *Password->GetText().ToString());
}

void UTitleWidgetBase::ProcessConnectClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), *ServerIP->GetText().ToString());
}

void UTitleWidgetBase::ProcessStartServerClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Lobby"), true, TEXT("listen"));
}
