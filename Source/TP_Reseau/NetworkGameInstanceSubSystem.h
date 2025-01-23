// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkGameInstanceSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class TP_RESEAU_API UNetworkGameInstanceSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//Public Variable
	public:

	//Private Variable
	private:

	//Public Function
	public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "1"), Category = "Network Game|Gameplay")
	void HostGame();

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "2"), Category = "Network Game|Gameplay")
	void JoinGame(FString IP);
	
	//Private Function
	private:
	
};
