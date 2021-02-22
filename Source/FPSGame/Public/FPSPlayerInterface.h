// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSPlayerInterface.generated.h"

/** Delegate called when the entire inventory has been loaded, all items may have been replaced
 FPS VERSION
*/
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetPickup);
DECLARE_MULTICAST_DELEGATE(FOnGetPickupNative);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPSPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGAME_API IFPSPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	// C++ implementation only
	UFUNCTION()
	virtual void GetPickupItem() = 0;

	//UFUNCTION()
	virtual class APawn* GetPlayer() = 0;

	//UFUNCTION()
	virtual class ACharacter* GetPlayerCharacter() = 0;

	/** Gets the delegate for when the inventory loads */
	virtual FOnGetPickupNative& GetPickupDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void Pickup();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	bool HasObjective();

};
