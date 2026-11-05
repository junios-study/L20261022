// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/VoiceInterface.h"

#include "VoiceSessionSubSystyem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoiceSubsystemSessionCreated, bool, bSuccess);


/**
 * 
 */
UCLASS()
class L20261022_API UVoiceSessionSubSystyem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "Voice Session")
    void CreateVoiceSession();

    UPROPERTY(BlueprintAssignable, Category = "Voice Session")
    FVoiceSubsystemSessionCreated OnVoiceSessionCreated;


    UPROPERTY(BlueprintReadOnly, Category = "Voice Session")
    bool bVoiceSessionCreated = false;

    virtual void Deinitialize() override;


    UFUNCTION(BlueprintCallable, Category = "Voice Session")
    bool StartTalking();

    UFUNCTION(BlueprintCallable, Category = "Voice Session")
    void StopTalking();


    IOnlineSessionPtr Sessions;
    FDelegateHandle CreateHandle;
    IOnlineVoicePtr Voice;


    bool bCreating = false;

    bool bTransmissionRequested = false;


    void HandleCreateComplete(FName SessionName, bool bSuccess);
    void ClearCreateDelegate();
};
