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

	public:
	ATP_ReseauPlayerController();

	protected:
	virtual void BeginPlay();
};
