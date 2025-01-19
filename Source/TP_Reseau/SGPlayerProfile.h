// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SaveGame.h"
#include "UObject/ObjectMacros.h"
#include "SGPlayerProfile.generated.h"

USTRUCT(BlueprintType)
struct FPlayerProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PlayerName;

	FPlayerProfile()
	{
		
		PlayerName = FText::FromString("Default");
	}
};

struct FPlayerProfile;

UCLASS()
class TP_RESEAU_API USGPlayerProfile : public USaveGame
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FPlayerProfile PlayerProfileStruct;
	
};
