// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidgetBase.h"


void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	//Path¸¸
	FSoftClassPath  TitleWidgetPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Title/WBP_Title.WBP_Title_C'"));


	//#include "TitleWidgetBase.h"
	//class UTitleWidgetBase {}
	UClass* TitleWidgetClass = TitleWidgetPath.TryLoadClass<UTitleWidgetBase>();

	if (TitleWidgetClass)
	{
		//, UTitleWigetBase* Instance = new UTitleWidgetBase
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		if (TitleWidgetObject)
		{
			TitleWidgetObject->AddToViewport();
		}
	}

	bShowMouseCursor = true;

	SetInputMode(FInputModeUIOnly());

}
