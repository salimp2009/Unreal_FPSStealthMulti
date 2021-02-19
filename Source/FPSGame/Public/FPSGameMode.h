// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeInterface.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase, public IFPSGameModeInterface
{
	GENERATED_BODY()

public:

	AFPSGameMode();

	// GameInterface functions; to avoid casting to GameMode directly
	virtual void HasCompletedMission(class APawn* PlayerPawn) override; // pure c++ implementation

	void CompleteMission(APawn* InstigatorPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn);

};



