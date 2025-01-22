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

	// TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	//
	// FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	// FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	//
	// FDelegateHandle OnStartSessionCompleteDelegateHandle;
	// FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	

	// Private Variable
	private:
	FString PlayerProfileSlot;
	// IOnlineSessionPtr SessionInterface;
	// TSharedPtr<FOnlineSessionSearch> SessionSearch;
	


	//Public Function
	public:



	//===== Player Profile =====
	UFUNCTION(BlueprintCallable)
	void CheckForSavedProfile();

	UFUNCTION(BlueprintCallable)
	void SaveProfile();

	UFUNCTION(BlueprintCallable)
	USGPlayerProfile* LoadProfile();

	//==========================

	//==== Create / Join Session ==================

	/*
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	UFUNCTION(BlueprintCallable, Category = "Network")
	void CreateMultiPlayerSession();

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool JoinSession();

	private:
	virtual void OnCreateSessionComplete(FName Name, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result);
	*/

	//===================================================
	
	
	
};
