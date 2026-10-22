// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	//LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));
	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessLoginClick);
	}
}

void UTitleWidgetBase::ProcessLoginClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Login Click : %s %s"), *UserID->GetText().ToString(), *Password->GetText().ToString());
}

void UTitleWidgetBase::ProcessConnectClick()
{
}

void UTitleWidgetBase::ProcessStartServerClick()
{
}
