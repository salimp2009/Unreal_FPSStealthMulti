// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));
	
	// no pickups 
	bIsCarryingObjective = false;
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** to see the arms of player in right angle on server 
		RemoteViewPitch is run both server and client
		we only need to change Remote copy of this instance; client has already right rotation ??
	*/
	if (!IsLocallyControlled())
	{
		//FRotator NewRot = Mesh1PComponent->GetRelativeRotation();
		//Mesh1PComponent->SetRelativeRotation(NewRot);
		
		FRotator NewRot = CameraComponent->GetRelativeRotation();
		
		/** RemoteViewPitch is store as 1 byte (uint8) for compression; 
			decompress to see actual value in World */
		NewRot.Pitch = RemoteViewPitch*360.0f/255.0f;

		CameraComponent->SetRelativeRotation(NewRot);
	}
	
}

// Interface implementation C++ and BP using delegates
void AFPSCharacter::GetPickupItem()
{
	bIsCarryingObjective = true;

	// Delegates update/call events both c++ and BP native 
	OnGetPickupNative.Broadcast();
	OnGetPickup.Broadcast();

	// BP implementable version
	GetPickup();  
	UE_LOG(LogTemp, Warning, TEXT("GetPickupItem(): C++ and BP Delegates"));
}

// returns player as Pawn; needed for GameMode
APawn* AFPSCharacter::GetPlayer()
{
	return this;
}

// returns as Actor; needed for LaunchPad...etc
ACharacter* AFPSCharacter::GetPlayerCharacter()
{
	return this;
}

// C++ implementation of  BP version
void AFPSCharacter::Pickup_Implementation()
{
	bIsCarryingObjective = true;
	UE_LOG(LogTemp, Warning, TEXT("Pickup(): BP Native; C++ Implementation "));
}

bool AFPSCharacter::HasObjective_Implementation()
{
	return bIsCarryingObjective;
}

void AFPSCharacter::Fire()
{
	// Server will spawn the Projectile in the Server_Implementation
	ServerFire();

	// try and play the sound if specified; Runs on requesting Client only
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}

void AFPSCharacter::ServerFire_Implementation()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

}

/** server checks if anythng is wrong and disconnects; to prevent cheating; 
	generallt true; if false  then do some custom checks */
bool AFPSCharacter::ServerFire_Validate()
{
	return true;
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}