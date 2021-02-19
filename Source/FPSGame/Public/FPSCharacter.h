// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayerInterface.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetPickup);

UCLASS()
class AFPSCharacter : public ACharacter, public IFPSPlayerInterface
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	bool bIsCarryingObjective;

	// Player Interface Functions
	
	virtual void GetPickupItem() override;

	virtual class APawn* GetPlayer() override;

	virtual FOnGetPickupNative& GetPickupDelegate() override
	{
		return OnGetPickupNative;
	}


	void Pickup_Implementation() override;  // c++ implementtation of BPNative interface event of Pickup()

	bool HasObjective_Implementation() override;


	///** Delegate called when the GetPickup has been called */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnGetPickup OnGetPickup;
	
	///** Native version above, called before BP delegate */
	FOnGetPickupNative OnGetPickupNative;

	/** BP  implementation of the OnGetPickup interface functions; Works */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
	void GetPickup();

protected:

	/** Fires a projectile. */
	void Fire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

};

