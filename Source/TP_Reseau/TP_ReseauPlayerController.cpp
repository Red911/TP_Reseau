// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ReseauPlayerController.h"
#include "NetworkPlayerState.h"
#include "TP_ReseauCharacter.h"


ATP_ReseauPlayerController::ATP_ReseauPlayerController()
{
}

float ATP_ReseauPlayerController::GetServerWorldTimeDelta() const
{
	return ServerWorldTimeDelta;
}

float ATP_ReseauPlayerController::GetServerWorldTime() const
{
	return GetWorld()->GetTimeSeconds() + ServerWorldTimeDelta;
}

void ATP_ReseauPlayerController::PostNetInit()
{
	Super::PostNetInit();
	if (GetLocalRole() != ROLE_Authority)
	{
		RequestWorldTime_Internal();
		if (NetworkClockUpdateFrequency > 0.f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::RequestWorldTime_Internal, NetworkClockUpdateFrequency, true);
		}
	}
}

void ATP_ReseauPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		ANetworkPlayerState* PS = PlayerController->GetPlayerState<ANetworkPlayerState>();
		if (PS)
		{
			int32 PlayerSkinIndex = PS->GetPlayerSkinIndex();

			ATP_ReseauCharacter* PlayerCharacter = Cast<ATP_ReseauCharacter>(GetCharacter());

			if (PlayerCharacter)
			{
				
			}
			
		}
	}
}

void ATP_ReseauPlayerController::RequestWorldTime_Internal()
{
	ServerRequestWorldTime(GetWorld()->GetTimeSeconds());
}

void ATP_ReseauPlayerController::ClientUpdateWorldTime_Implementation(float ClientTimestamp, float ServerTimestamp)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - ClientTimestamp;
	if (RoundTripTime < ShortestRoundTripTime)
	{
		ShortestRoundTripTime = RoundTripTime;
		ServerWorldTimeDelta = ServerTimestamp - ClientTimestamp - ShortestRoundTripTime / 2.f;
	}
}

void ATP_ReseauPlayerController::ServerRequestWorldTime_Implementation(float ClientTimestamp)
{
	const float Timestamp = GetWorld()->GetTimeSeconds();
	ClientUpdateWorldTime(ClientTimestamp, Timestamp);
}
