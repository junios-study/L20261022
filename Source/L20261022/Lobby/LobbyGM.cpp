// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"


ALobbyGM::ALobbyGM()
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGM() %d"), GetNetMode());
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay() %d"), GetNetMode());
}