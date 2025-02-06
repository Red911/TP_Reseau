// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ReseauCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NetworkGameInstance.h"
#include "NetworkPlayerState.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATP_ReseauCharacter

ATP_ReseauCharacter::ATP_ReseauCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	bReplicates = true;
}

void ATP_ReseauCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ANetworkPlayerState* NPS = Cast<ANetworkPlayerState>(GetPlayerState());
	
	// if (NPS)
	// {
	// 	SetSkinIndex(NPS->GetPlayerSkinIndex());
	// }

	UNetworkGameInstance* GI = Cast<UNetworkGameInstance>(GetGameInstance());
	
	if (GI)
	{
		SetSkinIndex(GI->GetPlayerSkinIndex());
	}
	
}

void ATP_ReseauCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ReseauCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_ReseauCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_ReseauCharacter::Look);

		// Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ATP_ReseauCharacter::Aiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ATP_ReseauCharacter::Aiming);

		// Shooting
		//EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Triggered, this, &ATP_ReseauCharacter::Shoot);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_ReseauCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Ajoute ici les propriétés à répliquer
	DOREPLIFETIME(ATP_ReseauCharacter, bIsAiming);
	DOREPLIFETIME(ATP_ReseauCharacter, SkinIndex);
}

void ATP_ReseauCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATP_ReseauCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATP_ReseauCharacter::Aiming(const FInputActionValue& Value)
{
	
	if (HasAuthority())// If we are the server
	{
		SetIsAiming(Value.Get<bool>());
		OnRep_IsAiming();
	}
	else //if we are the client
	{
		ServerSetIsAiming(Value.Get<bool>()); // Call the server to change the state
	}
}

void ATP_ReseauCharacter::Shoot(const FInputActionValue& Value)
{
	if (Value.Get<bool>() == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Input Pressed"));
		if (SpellNS)
		{
			UE_LOG(LogTemp, Log, TEXT("Spell != nullptr"));
			TObjectPtr<USceneComponent> AttachComponent = GetMesh();

			if (AttachComponent)
			{
				UE_LOG(LogTemp, Log, TEXT("AttachComp != nullptr"));
				TObjectPtr<UNiagaraComponent> NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				SpellNS,
				AttachComponent,
				"CastSocket",
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
				);
			}
		}
	}
}

void ATP_ReseauCharacter::ServerSetIsAiming_Implementation(const bool value)
{
	bIsAiming = value;
	OnRep_IsAiming(); //call the notification function
}

bool ATP_ReseauCharacter::ServerSetIsAiming_Validate(const bool value)
{
	return true;
}

void ATP_ReseauCharacter::OnRep_IsAiming()
{
	if (bIsAiming)
	{
		UE_LOG(LogTemp, Log, TEXT("ADS activé"));
		
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ADS désactivé"));
	}
}

void ATP_ReseauCharacter::SetSkinIndex(int32 Index)
{
	if (HasAuthority())// If we are the server
	{
		ANetworkPlayerState* PS = Cast<ANetworkPlayerState>(GetPlayerState());

		if (PS)
		{
			SkinIndex = Index;
			OnRep_SkinIndex();
		}
	}
	else //if we are the client
	{
		ServerSetSkinIndex(Index); // Call the server to change the state
	}
}

void ATP_ReseauCharacter::OnRep_SkinIndex()
{
	USkeletalMeshComponent* mesh = GetMesh();
	if (mesh)
	{
		mesh->SetMaterial(0, SkinMaterials[SkinIndex]);
	}
}

void ATP_ReseauCharacter::ServerSetSkinIndex_Implementation(int32 Index)
{
	ANetworkPlayerState* PS = Cast<ANetworkPlayerState>(GetPlayerState());
	if (PS)
	{
		SkinIndex = Index;
		OnRep_SkinIndex();
	}
}

bool ATP_ReseauCharacter::ServerSetSkinIndex_Validate(int32 Index)
{
	return true;
}




