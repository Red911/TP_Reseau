// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"

#include "TP_ReseauCharacter.h"
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

	ATP_ReseauCharacter* Player = Cast<ATP_ReseauCharacter>(GetOwner());
	if (Player)
	{
		Player->SetSkinIndex(PlayerMaterialIndex);
	}
}

void ANetworkPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (PlayerState)
	{
		ANetworkPlayerState* NPS = Cast<ANetworkPlayerState>(PlayerState);

		if (NPS)
		{
			//NPS->PlayerMaterialIndex = PlayerMaterialIndex;
			NPS->SetPlayerSkinIndex(PlayerMaterialIndex);
		}
	}
}


void ANetworkPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkPlayerState, PlayerMaterialIndex);
}
