// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerInterface.h"
#include "GameFramework/Character.h"



// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	OverlapComp->SetBoxExtent(FVector(75.0f, 75.0f, 50.0f));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverLapLaunchPad);

	/* CollisionSetup overriden in BP; also left it here to test it */
	//OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;
}

void AFPSLaunchPad::OverLapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchStrength * LaunchDirection.Vector();

	// get player as Character thru PlayerInterface; to avoid Heavy casting
	IFPSPlayerInterface* PlayerInterface = Cast<IFPSPlayerInterface>(OtherActor);

	// Launch Character
	if (PlayerInterface)
	{
		ACharacter* MyCharacter = PlayerInterface->GetPlayerCharacter();

		/** bXYOverride; if true replace the XY part of the Character's velocity instead of adding to it. 
			bZOverride;  if true replace the Z component of the Character's velocity instead of adding to it.
		*/
		MyCharacter->LaunchCharacter(LaunchVelocity, true, true);

		/* Play FX; check if this can be used instead of GetWorld(); turn this into function*/
		if (LaunchFX) UGameplayStatics::SpawnEmitterAtLocation(	GetWorld(), LaunchFX, GetActorLocation());
		if(LaunchSound) UGameplayStatics::PlaySound2D(this, LaunchSound);

	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{

		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		
		/* Play FX; check if this can be used instead of GetWorld(); turn this into function*/
		if (LaunchFX) UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
		if (LaunchSound) UGameplayStatics::PlaySound2D(this, LaunchSound);
	}
}
