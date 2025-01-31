// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "TP_ReseauCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Net/UnrealNetwork.h"


UNetworkGameInstance::UNetworkGameInstance()
{
	PlayerProfileSlot = "PlayerProfile_Slot";

	
}




void UNetworkGameInstance::CheckForSavedProfile()
{
	if(UGameplayStatics::DoesSaveGameExist(PlayerProfileSlot, 0))
	{
		 PlayerProfile = LoadProfile();
	}
	else
	{
		SaveProfile();
	}
}

void UNetworkGameInstance::SaveProfile()
{
	
	if (PlayerProfile != nullptr)
	{
		PlayerProfile->PlayerProfileStruct = PlayerProfileInfo;
	}
	else
	{
		PlayerProfile = Cast<USGPlayerProfile>(UGameplayStatics::CreateSaveGameObject(USGPlayerProfile::StaticClass()));
		PlayerProfile->PlayerProfileStruct = PlayerProfileInfo;

	}
	
	UGameplayStatics::SaveGameToSlot(PlayerProfile, PlayerProfileSlot, 0);
	

	
}

USGPlayerProfile* UNetworkGameInstance::LoadProfile()
{
	USaveGame* LoadedSaveGame = UGameplayStatics::LoadGameFromSlot(PlayerProfileSlot, 0);
	if(LoadedSaveGame)
	{
		

		USGPlayerProfile* playerprofile = Cast<USGPlayerProfile>(LoadedSaveGame);
		if(playerprofile)
		{
			PlayerProfileInfo = playerprofile->PlayerProfileStruct;
			UE_LOG(LogTemp, Warning, TEXT("Player Profile"));
		}

		return playerprofile;
	}

	return nullptr;
}

int32 UNetworkGameInstance::GetPlayerSkinIndex() const
{
	return PlayerSkinIndex;
}

void UNetworkGameInstance::SetPlayerSkinIndex(int32 value)
{
	PlayerSkinIndex = value;

	ATP_ReseauCharacter* Player = Cast<ATP_ReseauCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Player)
	{
		Player->SetSkinIndex(PlayerSkinIndex);
	}
}

