// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "EnhancedInputSubsystems.h"
#include "VoiceSessionSubSystyem.h"

void ABasePC::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMapping)
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	if (IsLocalController())
	{
		UGameInstance* GI = GetGameInstance();
		if (GI)
		{
			UVoiceSessionSubSystyem* Voice = GI->GetSubsystem<UVoiceSessionSubSystyem>();
			if (Voice)
			{
				Voice->CreateVoiceSession();
			}
		}
	}
}

void ABasePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::V, IE_Pressed, this, &ABasePC::OnVoicePressed);
	InputComponent->BindKey(EKeys::V, IE_Released, this, &ABasePC::OnVoiceReleased);
}

void ABasePC::OnVoicePressed()
{
	if (IsLocalController())
	{
		UGameInstance* GI = GetGameInstance();
		if (GI)
		{
			UVoiceSessionSubSystyem* Voice = GI->GetSubsystem<UVoiceSessionSubSystyem>();
			if (Voice)
			{
				Voice->StartTalking();
			}
		}
	}
}

void ABasePC::OnVoiceReleased()
{
	if (IsLocalController())
	{
		UGameInstance* GI = GetGameInstance();
		if (GI)
		{
			UVoiceSessionSubSystyem* Voice = GI->GetSubsystem<UVoiceSessionSubSystyem>();
			if (Voice)
			{
				Voice->StopTalking();
			}
		}
	}
}
