// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetworkPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TP_RESEAU_API ANetworkPlayerState : public APlayerState
{
	GENERATED_BODY()

	public:
		
		ANetworkPlayerState();
	
		
		UFUNCTION(BlueprintCallable, Category = "PlayerState")
		int32 GetPlayerSkinIndex() const;
	
		
		UFUNCTION(BlueprintCallable, Category = "PlayerState")
		void SetPlayerSkinIndex(int32 value);

	protected:
		
		UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
		int32 PlayerMaterialIndex = 0;

	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(class APlayerState* PlayerState) override;

	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(class APlayerState* PlayerState) override;
	
		
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
