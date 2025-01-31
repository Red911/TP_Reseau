// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "SGPlayerProfile.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetworkGameInstance.generated.h"


class UUserWidget;


UCLASS()

class TP_RESEAU_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UNetworkGameInstance();



	// Public Variable
	public:
	UPROPERTY(BlueprintReadWrite)
	FPlayerProfile PlayerProfileInfo;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USGPlayerProfile> PlayerProfile;

	UPROPERTY(BlueprintReadOnly)
	bool isLanConnection;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxNumberOfPlayer;
	
	UPROPERTY(BlueprintReadWrite)
	FName SessionName = "MySession";

	UPROPERTY(BlueprintReadOnly)
	uint8 PlayerSkinIndex = 0;

	// Private Variable
	private:
	FString PlayerProfileSlot;
	
	//Public Function
	public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int32 GetPlayerSkinIndex() const;
	
		
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetPlayerSkinIndex(int32 value);


	//===== Player Profile =====
	UFUNCTION(BlueprintCallable)
	void CheckForSavedProfile();

	UFUNCTION(BlueprintCallable)
	void SaveProfile();

	UFUNCTION(BlueprintCallable)
	USGPlayerProfile* LoadProfile();

	//==========================

	
};
