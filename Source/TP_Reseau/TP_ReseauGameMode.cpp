// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ReseauGameMode.h"
#include "TP_ReseauCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATP_ReseauGameMode::ATP_ReseauGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	bUseSeamlessTravel = true;
}
