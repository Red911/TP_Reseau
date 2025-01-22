// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"


UNetworkGameInstance::UNetworkGameInstance()
{
	PlayerProfileSlot = "PlayerProfile_Slot";

	// IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	// if (OnlineSubsystem)
	// {
	// 	SessionInterface = OnlineSubsystem->GetSessionInterface();
	// 	if (SessionInterface.IsValid())
	// 	{
	// 		// Bind Delegate
	// 		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnCreateSessionComplete);
	// 		SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnStartOnlineGameComplete);
	// 		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnJoinSessionComplete);
	// 	}
	// }
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


//===========Create / JoinSession===========

/*
bool UNetworkGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set("SETTING_LOBBY", FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UNetworkGameInstance::CreateMultiPlayerSession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		
		if (SessionInterface.IsValid())
		{
			//Set Delegate to check if it was succes or not
			

			//Set Session Setting
			FOnlineSessionSettings SessionSettings;

			SessionSettings.NumPublicConnections = MaxNumberOfPlayer; // Max Player
			SessionSettings.bIsLANMatch = isLanConnection; //Session Lan
			SessionSettings.bShouldAdvertise = true; //Visible for the other players
			SessionSettings.bUsesPresence = true; //For friend request

			//Get PlayerController
			TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
			TSharedPtr<const FUniqueNetId> UserId = PlayerController->PlayerState->GetUniqueId().GetUniqueNetId();

			if (UserId.IsValid())
			{
				//Create the Session
				SessionInterface->CreateSession(*UserId, SessionName, SessionSettings);
			}
		}
	}
}

bool UNetworkGameInstance::JoinSession()
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		//Get PlayerController
		TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
		TSharedPtr<const FUniqueNetId> UserId = PlayerController->PlayerState->GetUniqueId().GetUniqueNetId();

		if (PlayerController)
		{
			
		}
		
	}
		
	return bSuccessful;
}




void UNetworkGameInstance::OnCreateSessionComplete(FName Name, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}
		
	}
}

void UNetworkGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
	}
}


void UNetworkGameInstance::OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		
		if (SessionInterface.IsValid())
		{
			if (Result == EOnJoinSessionCompleteResult::Success)
			{
				//Get PlayerController
				TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					FString TravelURL;

					if (SessionInterface->GetResolvedConnectString(Name, TravelURL))
					{
						PlayerController->ClientTravel(TravelURL,TRAVEL_Absolute);
					}
				}
			}
		}
	}
	
}


*/
//============================================

