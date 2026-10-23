// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

class UButton;
class UTextBlock;
class UEditableTextBox;
class UScrollBox;
/**
 * 
 */
UCLASS()
class L20261022_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> LeftTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ConnectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void ProcessTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnStart();

	void ShowStartButton();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> StartButtonAnimation;

	UFUNCTION()
	void UpdateLeftTime(int NewLeftTime);

	UFUNCTION()
	void UpdateConnectionCount(int NewCount);

	UFUNCTION(BlueprintCallable)
	void AddMessage(const FText& Message);

};
