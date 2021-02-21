// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	OverlapComp->SetBoxExtent(FVector(75.0f, 75.0f, 50.0f));
	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverLapLaunchPad);

	//OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;
}

void AFPSLaunchPad::OverLapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO; implement actions when player overlap and when PhysicActor w/ SimulatePhysics overlap

	UE_LOG(LogTemp, Warning, TEXT("LAUNCHPAD:Actor Overlapped!!!"));

}
