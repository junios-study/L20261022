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

	virtual void PostInitializeComponents() override;


	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULobbyWidgetBase> LobbyWidgetObject;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> LoadingWidgetObject;

	UFUNCTION(Server, WithValidation, Reliable)
	void C2S_SendMessage(const FText& Messsage);
	bool C2S_SendMessage_Validate(const FText& Messsage);
	void C2S_SendMessage_Implementation(const FText& Messsage);

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Messsage);
	void S2C_SendMessage_Implementation(const FText& Messsage);

	UFUNCTION(Client, Reliable)
	void S2C_ShowLoadingScreen();
	void S2C_ShowLoadingScreen_Implementation();

	void ShowLoadingScreen();

	UFUNCTION(Server, Reliable)
	void C2S_StartGame();
	void C2S_StartGame_Implementation();

};
