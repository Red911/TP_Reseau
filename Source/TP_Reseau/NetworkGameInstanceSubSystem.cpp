// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstanceSubSystem.h"

void UNetworkGameInstanceSubSystem::HostGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->ClientTravel("/Game/Levels/Lobby?listen", TRAVEL_Absolute);
	}
	
}

void UNetworkGameInstanceSubSystem::JoinGame(FString IP)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->ClientTravel(IP, TRAVEL_Absolute);
	}
}

void UNetworkGameInstanceSubSystem::TravelToMap(int32 nConnectedPlayers, FString LevelName)
{
	GetWorld()->ServerTravel(LevelName);
}
