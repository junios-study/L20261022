// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FChangeLeftTimeSignature, ALobbyGS, OnChangeLeftTime, int, NewTime);
/**
 * 
 */
UCLASS()
class L20261022_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int64 LeftTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int64 ConnectionCount = 0;

	void SpendTime();

	void SetConnectionCount(int NewCount);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FChangeLeftTimeSignature OnChangeLeftTime;
};
