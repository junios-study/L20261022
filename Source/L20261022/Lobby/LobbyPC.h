// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class L20261022_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPC();

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULobbyWidgetBase> LobbyWidgetObject;

};
