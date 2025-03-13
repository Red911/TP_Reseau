// Fill out your copyright notice in the Description page of Project Settings.


#include "LagCompensationComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULagCompensationComponent::ULagCompensationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Owner = GetOwner();
}


// Called when the game starts
void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		FSavedMove NewMove;
		NewMove.CapsuleTransform = GetOwner()->GetActorTransform();
		NewMove.Timestamp = GetWorld()->GetTimeSeconds();
		SavedMoves.Add(NewMove);
	}
}

void ULagCompensationComponent::ApplyTransform(const FTransform& NewTransform)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		// Appliquer la nouvelle transformation
		OwnerCharacter->SetActorTransform(NewTransform);
	}
}

bool ULagCompensationComponent::PerformRaycast(const FVector& Start, const FVector& End)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner()); // Ignorer le propriétaire du raycast

	// Effectuer le raycast
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldDynamic, // Channel de collision
		CollisionParams
	);

	DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor(255, 0, 0),
			true, -1, 0,
			12.333
		);

	// Vérifier si le raycast a touché un adversaire
	if (bHit && HitResult.GetActor())
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor());
		if (HitCharacter && HitCharacter != GetOwner())
		{
			HitLocation = HitCharacter->GetActorLocation();
			return true; // Toucher confirmé
		}
	}

	return false; // Aucun toucher
}

void ULagCompensationComponent::RestoreOriginalTransform()
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		// Restaurer la transformation actuelle
		OwnerCharacter->SetActorTransform(OriginalTransform);
	}
}

bool ULagCompensationComponent::ServerSideRewind(float HitTime, FVector HitStart, FVector HitEnd)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		// Stocker la transformation actuelle
		OriginalTransform = OwnerCharacter->GetActorTransform();

		// Trouver la frame correspondante dans l'historique
		for (const FSavedMove& Move : SavedMoves)
		{
			if (Move.Timestamp <= HitTime)
			{
				// Appliquer la transformation historique
				ApplyTransform(Move.CapsuleTransform);

				// Effectuer le raycast
				bool bHit = PerformRaycast(HitStart, HitEnd);

				// Restaurer la transformation actuelle
				RestoreOriginalTransform();

				// Retourner le résultat du raycast
				return bHit;
			}
		}

		// Restaurer la transformation actuelle si aucune frame n'est trouvée
		RestoreOriginalTransform();
	}

	return false; // Aucun toucher
}



