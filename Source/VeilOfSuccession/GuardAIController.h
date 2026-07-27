// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GuardAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class AGuardCharacter;
class ASpyCharacter;

UENUM()
enum class EGuardState : uint8
{
	Patrol,
	Suspicious,
	Chasing
};

UCLASS()
class VEILOFSUCCESSION_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGuardAIController();

	virtual void Tick(float DeltaSeconds) override;

	EGuardState GetState() const { return State; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, Category="AI")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

private:
	void EnterState(EGuardState NewState);
	void TickPatrol(AGuardCharacter* Guard, float DeltaSeconds);
	void TickSuspicious(AGuardCharacter* Guard, float DeltaSeconds);
	void TickChasing(AGuardCharacter* Guard, ASpyCharacter* Player, float DeltaSeconds);

	ASpyCharacter* ResolvePlayer() const;
	void MoveToward(APawn* InPawn, const FVector& Target) const;

	EGuardState State = EGuardState::Patrol;

	float Suspicion = 0.f;
	bool bSightActive = false;

	FVector LastStimulusLocation = FVector::ZeroVector;
	FVector LastKnownPlayerLocation = FVector::ZeroVector;

	int32 PatrolIndex = 0;
	bool bPatrolPausing = false;
	float PatrolPauseTimer = 0.f;

	float SuspiciousWaitTimer = 0.f;

	// Breadcrumb search: on losing sight mid-chase the guard learns only the
	// player's first couple of steps past the obstacle, then must search.
	TArray<FVector> Breadcrumbs;
	int32 BreadcrumbIndex = 0;
	int32 BreadcrumbsToSample = 0;
	float BreadcrumbSampleTimer = 0.f;
	float TimeSinceSeenPlayer = 0.f;
	float MeleeCooldown = 0.f;
	float HearingCooldown = 0.f;
	float PendingMeleeAt = -1.f;
};
