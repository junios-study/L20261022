// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataGIS.generated.h"

/**
 * 
 */
UCLASS()
class L20261022_API UDataGIS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Password;
};
