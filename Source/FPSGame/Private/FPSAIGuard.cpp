// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"



// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// No need attachment since it is not a scene component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
	
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Patrol Goal Checks
	if (CurrentPatrolPoint)
	{
		FVector DeltaToGoal = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		//float DistanceToGoal = DeltaToGoal.Size();  // include Z as a distance
		float DistanceToGoal = DeltaToGoal.DistXY(GetActorLocation(), CurrentPatrolPoint->GetActorLocation());

		// if we are within 70 units of patrol point then pick the next patrol point
		if (DistanceToGoal < 70.0f)
		{
			MoveToNextPatrolPoint();
		}
	}
}


void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn==nullptr)
	{
		return;
	}

	 
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

// Interface class of GameMode is used to avoid casting
	IFPSGameModeInterface* GM = Cast<IFPSGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->HasCompletedMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);

	bPatrol = false;
	//StopMoving if patrolling
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	// if already Seen; it takes priority
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	/* if two noises heard when one timer is still on ; the second noise event will clear the current timer */
	GetWorldTimerManager().ClearTimer(TimerHandleResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandleResetOrientation,this, &AFPSAIGuard::ResetOrientation, 2.0f);

	SetGuardState(EAIState::Suspicious);

	//StopMoving if patrolling
	if (Controller)
	{
		Controller->StopMovement();
	}

}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);

	// Stop investigating and patrol if we are a patrolling Pawn; bPatrol=true
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void  AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState ==NewState)
	{
		return;
	}

	GuardState = NewState;
	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint==SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	// No need to call BP Library function
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

	// Alternative ; and it works
	//AAIController* GuardController = Cast<AAIController>(Controller);
	//GuardController->MoveToActor(CurrentPatrolPoint);
	
}




