// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TP_ReseauPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TP_RESEAU_API ATP_ReseauPlayerController : public APlayerController
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditDefaultsOnly, Category = GameState)
	float NetworkClockUpdateFrequency = 5.f;

	private:

	float ServerWorldTimeDelta = 0.f;
	float ShortestRoundTripTime = BIG_NUMBER;
	
	public:
	ATP_ReseauPlayerController();

	UFUNCTION(BlueprintPure)
	float GetServerWorldTimeDelta() const;
	UFUNCTION(BlueprintPure)
	float GetServerWorldTime() const;

	void PostNetInit() override;

	protected:
	virtual void BeginPlay();

	private:

	void RequestWorldTime_Internal();

	UFUNCTION(Server, Unreliable)
	void ServerRequestWorldTime(float ClientTimestamp);

	UFUNCTION(Client, Unreliable)
	void ClientUpdateWorldTime(float ClientTimestamp, float ServerTimestamp);
};
