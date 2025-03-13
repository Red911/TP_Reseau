// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ReseauCharacter.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT()  
struct FSavedMove  
{
	GENERATED_BODY()
	
	FTransform CapsuleTransform;  
	float Timestamp;

	FSavedMove()
	{
		Timestamp = 0;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TP_RESEAU_API ULagCompensationComponent : public UActorComponent
{
	
	GENERATED_BODY()

	  

public:
	// Sets default values for this component's properties
	ULagCompensationComponent();

	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	TArray<FSavedMove> SavedMoves;

	FVector HitLocation;


private:
	FTransform OriginalTransform;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyTransform(const FTransform& NewTransform);
	bool PerformRaycast(const FVector& Start, const FVector& End);
	void RestoreOriginalTransform();
	bool ServerSideRewind(float HitTime, FVector HitStart, FVector HitEnd);

	
};
