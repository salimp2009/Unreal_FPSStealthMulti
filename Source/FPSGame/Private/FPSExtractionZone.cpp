// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h"

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
	// UE_LOG(LogTemp, Warning, TEXT("Extraction Zone Active: Overlap True"));

	// Use PlayerInterface to check if Other actor has PlayerInterface and HasObjective
	IFPSPlayerInterface* PlayerInterface = Cast<IFPSPlayerInterface>(OtherActor);

	if (PlayerInterface && PlayerInterface->Execute_HasObjective(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Has Objective"));

		//AFPSGameMode* CurrentGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		IFPSGameModeInterface* GM = Cast<IFPSGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			// Get a reference to current player that Overlapped
			APawn* MyPawn = PlayerInterface->GetPlayer();
			//CurrentGameMode->CompleteMission(PlayerInterface->GetPlayer());
			if(MyPawn) GM->HasCompletedMission(MyPawn);
		}
	}

}

