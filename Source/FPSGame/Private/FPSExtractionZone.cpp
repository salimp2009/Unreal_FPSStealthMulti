// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerInterface.h"
#include "FPSGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"


// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp=CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(true);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
	

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(RootComponent); 
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Use PlayerInterface to check if Other actor has PlayerInterface and HasObjective
	IFPSPlayerInterface* PlayerInterface = Cast<IFPSPlayerInterface>(OtherActor);

	if (PlayerInterface == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Extraction Zone: Player Entered Extraction Zone!!!"));

	// Get a reference to current player that Overlapped; using FPSPlayerInterface
	APawn* MyPawn = PlayerInterface->GetPlayer();
	
	// Check Player has Objective
	if (PlayerInterface->Execute_HasObjective(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Extraction Zone: Player Has Objective!!!"));

		// Interface class of GameMode is used to avoid casting
		IFPSGameModeInterface* GM = Cast<IFPSGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			// Call mission Complete in GameMode using GameModeInterface; no casting
			if (MyPawn) GM->HasCompletedMission(MyPawn, true);
		}
	}
	else
	{
		if(ObjectiveMissingSound) UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

	

}

