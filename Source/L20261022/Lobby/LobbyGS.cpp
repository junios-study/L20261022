// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"


void ALobbyGS::SpendTime()
{
	LeftTime--;
	OnChangeLeftTime.Broadcast(LeftTime);
}

void ALobbyGS::SetConnectionCount(int NewCount)
{
	ConnectionCount = NewCount;
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
}
