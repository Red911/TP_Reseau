// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TP_ReseauCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATP_ReseauCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimingAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootingAction;

	

public:
	ATP_ReseauCharacter();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> SpellNS;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> SkinMaterials;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Bullet;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* BP_HitCapsule_Red;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* BP_HitCapsule_Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantSpawnCapsule = true;
	
	
private:

	UPROPERTY(ReplicatedUsing = OnRep_IsAiming)
	bool bIsAiming;

	UPROPERTY(ReplicatedUsing = OnRep_IsShooting)
	bool isShooting;

	UPROPERTY(ReplicatedUsing = OnRep_SkinIndex)
	int32 SkinIndex = 0;

	AActor* Capsule;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Aiming(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = Aiming)
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	UFUNCTION(BlueprintCallable, Category = Aiming)
	FORCEINLINE void SetIsAiming(const bool value) { bIsAiming = value; }

	UFUNCTION(BlueprintCallable, Category = Aiming)
	FORCEINLINE bool GetIsShooting() const { return isShooting; }
	UFUNCTION(BlueprintCallable, Category = Aiming)
	FORCEINLINE void SetIsShooting(const bool value) { isShooting = value; }

	// Server Function to set Aiming State
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsAiming(const bool value);

	UFUNCTION()
	void OnRep_IsAiming();

	UFUNCTION()
	void OnRep_IsShooting();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsShooting(const bool value);

	UFUNCTION(BlueprintCallable)
	void SetSkinIndex(int32 Index);

	UFUNCTION()
	void OnRep_SkinIndex();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSkinIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void OnFire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendShotRequest(FVector_NetQuantize HitStart, FVector_NetQuantize HitEnd, float ClientHitTime);

	UFUNCTION(NetMulticast, Reliable)  
	void Multicast_SpawnHitCapsule(FVector Location, UClass* Bp);  

	

	
	




	
};





