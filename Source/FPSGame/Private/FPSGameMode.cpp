// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

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
	CompleteMission(PlayerPawn);
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

		OnMissionCompleted(InstigatorPawn);
	}

}


