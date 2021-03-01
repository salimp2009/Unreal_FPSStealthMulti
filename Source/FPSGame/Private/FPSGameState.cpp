// Developedby Salim Pamukcu 2021


#include "FPSGameState.h"
#include "EngineUtils.h"
#include "FPSPlayerController.h"


void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		/** OK to cast FPSPlayerController since it only has one extra BP function otherwise make an Interface for it */
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

		/** Player Controller runs on both Server and Client; we want only the Client PC to display the HUD */	
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			
			//Disable Input
			APawn* MyPawn = PC->GetPawn(); // PlayerInterface is not needed since APawn will be called anyway
			if (MyPawn)
			{
				if (MyPawn) MyPawn->DisableInput(PC);
				
			}
		}
	}

}
