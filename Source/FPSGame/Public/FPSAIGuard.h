// Copy Rights by Salim Pamukcu 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"


UENUM(BlueprintType)
enum class EAIState: uint8  // unint8 is required if exposed to Blueprint
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UPawnSensingComponent* PawnSensingComp;

	/* The function that will be binded dynamically to OnSeePawn delegate of PawnSensingComponent */
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	FTimerHandle TimerHandleResetOrientation;

	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	/** Will Notify Clients when GuardState changes; similar to RepNotify in BP*/
	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	UPROPERTY(EditInstanceOnly, Category="AI")
	bool bPatrol;

	/** meta specifier meta=(EditCondition="bPatrol") allows 
	this property to be editable if bPatrol is true */
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* FirstPatrolPoint;


	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* SecondPatrolPoint;

	/** current locatin whether AI is either Moving or standing; not exposed to BP */
	AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();
};
