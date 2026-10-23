// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"


void ALobbyGS::OnRep_LeftTime()
{
	OnChangeLeftTime.Broadcast(LeftTime);
}

void ALobbyGS::OnRep_ConnectionCount()
{
	OnChangeConnectoinCount.Broadcast(ConnectionCount);
}

void ALobbyGS::SpendTime()
{
	LeftTime--;
}

void ALobbyGS::SetConnectionCount(int NewCount)
{
	ConnectionCount = NewCount;
	OnRep_ConnectionCount();
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
}
