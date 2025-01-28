// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"
#include "Net/UnrealNetwork.h"

ANetworkPlayerState::ANetworkPlayerState()
{
	
	PlayerMaterialIndex = 0;

	
	bReplicates = true;
}

int32 ANetworkPlayerState::GetPlayerSkinIndex() const
{
	return PlayerMaterialIndex;
}

void ANetworkPlayerState::SetPlayerSkinIndex(int32 value)
{
	PlayerMaterialIndex = value;
}

void ANetworkPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkPlayerState, PlayerMaterialIndex);
}
