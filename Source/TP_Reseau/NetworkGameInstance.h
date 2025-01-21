// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGPlayerProfile.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
class UUserWidget;


UCLASS()

class TP_RESEAU_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UNetworkGameInstance();

	//========Delegate========
	/* Delegate called when session created */
	DECLARE_DELEGATE(FOnCreateSessionCompleteDelegateSignature);
	FOnCreateSessionCompleteDelegateSignature OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	DECLARE_DELEGATE(FOnStartSessionCompleteDelegate);
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	// Public Variable
	public:
	UPROPERTY(BlueprintReadWrite)
	FPlayerProfile PlayerProfileInfo;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USGPlayerProfile> PlayerProfile;

	UPROPERTY(BlueprintReadOnly)
	bool isLanConnection;

	// Private Variable
	private:
	FString PlayerProfileSlot;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	//Public Function
	public:

	UFUNCTION(BlueprintCallable)
	bool ChangeConnectionType();

	//===== Player Profile =====
	UFUNCTION(BlueprintCallable)
	void CheckForSavedProfile();

	UFUNCTION(BlueprintCallable)
	void SaveProfile();

	UFUNCTION(BlueprintCallable)
	USGPlayerProfile* LoadProfile();

	//==========================

	UFUNCTION(BlueprintCallable)
	void CreateMultiplayerSession();

	protected:
	virtual void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	

	//Private Function
	private:
	/**
	*	Function to host a game
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
		bool HostSession(TSharedPtr<const FUniqueNetId> UniqueId, FName SessionName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers);

		void OnStartSessionComplete(FName SessionName, bool bSuccess);
	
	
};
