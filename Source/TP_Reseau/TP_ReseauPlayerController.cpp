// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ReseauPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


ATP_ReseauPlayerController::ATP_ReseauPlayerController()
{
}

void ATP_ReseauPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if (IsLocalController())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this, false);
		bShowMouseCursor = false;
	}
}
