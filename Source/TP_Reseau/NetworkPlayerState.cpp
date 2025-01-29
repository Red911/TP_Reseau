// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"
#include "Net/UnrealNetwork.h"

ANetworkPlayerState::ANetworkPlayerState()
{
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

void ANetworkPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (PlayerState)
	{
		ANetworkPlayerState* NPS = Cast<ANetworkPlayerState>(PlayerState);

		if (NPS)
		{
			NPS->PlayerMaterialIndex = PlayerMaterialIndex;
		}
	}
}

void ANetworkPlayerState::OverrideWith(class APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	if (PlayerState)
	{
		ANetworkPlayerState* NPS = Cast<ANetworkPlayerState>(PlayerState);

		if (NPS)
		{
			PlayerMaterialIndex = NPS->PlayerMaterialIndex ;
		}
	}
}

void ANetworkPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkPlayerState, PlayerMaterialIndex);
}
