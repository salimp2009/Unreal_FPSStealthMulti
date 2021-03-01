// Developedby Salim Pamukcu 2021


#include "FPSGameState.h"
#include "EngineUtils.h"
#include  "FPSPlayerInterface.h"


void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	/** Original way except used TActorIterator<APawn> ; other one depreciated */
	
	//for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	//{
	//	APawn* MyPawn = *It;
	//	if (MyPawn && MyPawn->IsLocallyControlled())
	//	{
	//		MyPawn->DisableInput(nullptr);
	//	}
	//}


	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(It->Get());

		if (PC && PC->IsLocalController())
		{
			//IFPSPlayerInterface* PlayerInterface = Cast<IFPSPlayerInterface>(PC->GetPawn());
			
			APawn* MyPawn2 = PC->GetPawn(); // Alternative if PlayerInterface version does not work
			if (MyPawn2)
			{
				//APawn* MyPawn = PlayerInterface->GetPlayer();
				//if(MyPawn) MyPawn->DisableInput(nullptr);

				if (MyPawn2) MyPawn2->DisableInput(nullptr);
			}
		}
	}

}
