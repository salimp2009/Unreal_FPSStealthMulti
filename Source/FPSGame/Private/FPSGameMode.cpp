// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::HasCompletedMission(APawn* PlayerPawn)
{
	if(PlayerPawn) CompleteMission(PlayerPawn);
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		// Disableinput expects PlayerController or nullptr; both are OK!!!
		/** Disableinput stops movement of the Pawn 
			but PlayerController can still get input to access menus..etc
		*/
		UE_LOG(LogTemp, Warning, TEXT("FPSGameMode; Mission is Complete"));
		InstigatorPawn->DisableInput(nullptr);
		
		
		if (SpectatingViewPointClass)
		{
			TArray<AActor*> ReturnedActors;

			UGameplayStatics::GetAllActorsWithInterface(this, SpectatingViewPointClass, ReturnedActors);
			//UGameplayStatics::GetAllActorsWithTag(this, "SpectatingCamera", ReturnedActors); // Alternative way to get actors with tags
		
			// change view target if any valid actor found 
			if (ReturnedActors.Num() >0)
			{
				AActor* NewViewTarget = ReturnedActors[0];

				// Get Player Controller to Set View Target to Spectating Camera Actor (NewViewTarget) 
				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->Controller);
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 3.0f, EViewTargetBlendFunction::VTBlend_Linear);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GAMEMode::No SpectatingViewPointClass is SET in World. Please setup in GameMode Defaults!!!"));
		}

		OnMissionCompleted(InstigatorPawn);
	}

}


