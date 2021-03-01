// Developedby Salim Pamukcu 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:

	/** NetMulticast ; if sent from Server it will be sent to all clients; run on clients & server 
		If they are called from clients, they will only execute locally, and will not execute on the server
	*/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess);

};
