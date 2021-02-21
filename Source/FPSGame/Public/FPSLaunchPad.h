// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* OverlapComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;


	UFUNCTION()
	void OverLapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, Category="LaunchPad")
	float LaunchStrength;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchPitchAngle;


	UPROPERTY(EditDefaultsOnly, Category="LaunchPad")
	class USoundBase* LaunchSound;

	UPROPERTY(EditDefaultsOnly, Category="LaunchPad")
	UParticleSystem* LaunchFX;

};
