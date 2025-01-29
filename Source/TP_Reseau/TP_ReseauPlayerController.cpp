// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ReseauPlayerController.h"
#include "NetworkPlayerState.h"
#include "TP_ReseauCharacter.h"


ATP_ReseauPlayerController::ATP_ReseauPlayerController()
{
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
