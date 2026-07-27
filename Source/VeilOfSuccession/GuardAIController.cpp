// Veil of Succession - fictional political thriller. All characters fictitious.
#include "GuardAIController.h"
#include "GuardCharacter.h"
#include "EngineUtils.h"
#include "SpyCharacter.h"
#include "StealthComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

AGuardAIController::AGuardAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2200.f;
	SightConfig->LoseSightRadius = 2600.f;
	SightConfig->PeripheralVisionAngleDegrees = 70.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComp->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComp->ConfigureSense(*HearingConfig);

	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	SetPerceptionComponent(*PerceptionComp);
}

void AGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (PerceptionComp)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AGuardAIController::OnPerceptionUpdated);
	}

	EnterState(EGuardState::Patrol);
}

void AGuardAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ASpyCharacter* Spy = Cast<ASpyCharacter>(Actor);
	if (!Spy)
	{
		return;
	}

	// Only track sight here; hearing is polled in Tick for robustness.
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		bSightActive = Stimulus.WasSuccessfullySensed();
		if (bSightActive)
		{
			LastStimulusLocation = Actor->GetActorLocation();
		}
	}
}

ASpyCharacter* AGuardAIController::ResolvePlayer() const
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			return Cast<ASpyCharacter>(PC->GetPawn());
		}
	}
	return nullptr;
}

void AGuardAIController::MoveToward(APawn* InPawn, const FVector& Target) const
{
	if (!InPawn)
	{
		return;
	}
	FVector Dir = (Target - InPawn->GetActorLocation()).GetSafeNormal2D();
	if (Dir.IsNearlyZero())
	{
		return;
	}

	// Probe for a wall ahead; if blocked, slide along it so the guard goes
	// AROUND obstacles instead of grinding into them.
	if (UWorld* World = InPawn->GetWorld())
	{
		const FVector Start = InPawn->GetActorLocation();
		const FVector End = Start + Dir * 170.f;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(GuardSteer), false, InPawn);
		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)
			&& !Cast<APawn>(Hit.GetActor()))
		{
			const FVector N = Hit.ImpactNormal.GetSafeNormal2D();
			FVector Slide = (Dir - N * FVector::DotProduct(Dir, N)).GetSafeNormal2D();
			if (Slide.IsNearlyZero())
			{
				Slide = FVector::CrossProduct(N, FVector::UpVector).GetSafeNormal2D();
			}
			Dir = Slide;
		}
	}

	InPawn->AddMovementInput(Dir);
}

void AGuardAIController::EnterState(EGuardState NewState)
{
	State = NewState;

	float Speed = 220.f;
	switch (NewState)
	{
	case EGuardState::Patrol:
		Speed = 220.f;
		bPatrolPausing = false;
		PatrolPauseTimer = 0.f;
		break;
	case EGuardState::Suspicious:
		Speed = 420.f;
		SuspiciousWaitTimer = 3.f;
		break;
	case EGuardState::Chasing:
		Speed = 600.f;
		MeleeCooldown = 0.f;
		TimeSinceSeenPlayer = 0.f;
		break;
	}

	if (AGuardCharacter* Guard = Cast<AGuardCharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* Move = Guard->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = Speed;
		}
	}
}

void AGuardAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AGuardCharacter* Guard = Cast<AGuardCharacter>(GetPawn());
	if (!Guard)
	{
		return;
	}

	// Dead/stunned guard: controller stops all logic.
	if (Guard->IsDeadOrStunned())
	{
		StopMovement();
		return;
	}

	ASpyCharacter* Player = ResolvePlayer();
	const FVector GuardLoc = Guard->GetActorLocation();

	if (MeleeCooldown > 0.f)  { MeleeCooldown -= DeltaSeconds; }
	if (HearingCooldown > 0.f) { HearingCooldown -= DeltaSeconds; }

	const bool bPlayerValid = (Player != nullptr) && !Player->IsDead();

	// Suspicion from sight.
	if (bPlayerValid && bSightActive)
	{
		const float Dist = FVector::Dist(GuardLoc, Player->GetActorLocation());
		float Vis = 1.f;
		if (UStealthComponent* SC = Player->GetStealthComponent())
		{
			Vis = SC->GetVisibilityMultiplier();
		}
		const float DistFactor = FMath::Clamp(1.6f - Dist / 2200.f, 0.25f, 1.6f);
		Suspicion += 1.2f * Vis * DistFactor * DeltaSeconds;

		LastKnownPlayerLocation = Player->GetActorLocation();
		LastStimulusLocation = LastKnownPlayerLocation;
		TimeSinceSeenPlayer = 0.f;
	}
	else
	{
		Suspicion -= 0.35f * DeltaSeconds;
		TimeSinceSeenPlayer += DeltaSeconds;
	}

	// Corpse discovery: a patrolling guard that walks near a fallen comrade
	// gets suspicious and investigates - bodies are now evidence (TLOU/MGS).
	if (State == EGuardState::Patrol && Suspicion < 0.25f)
	{
		for (TActorIterator<AGuardCharacter> It(GetWorld()); It; ++It)
		{
			AGuardCharacter* Other = *It;
			if (!Other || Other == Guard || !Other->IsDeadOrStunned())
			{
				continue;
			}
			if (FVector::DistSquared(GuardLoc, Other->GetActorLocation()) < FMath::Square(450.f))
			{
				Suspicion = 0.45f;
				LastStimulusLocation = Other->GetActorLocation();
				break;
			}
		}
	}

	// Hearing (polled): loud player nearby bumps suspicion.
	if (bPlayerValid && HearingCooldown <= 0.f)
	{
		const float Noise = Player->GetNoiseLevel();
		const float Dist = FVector::Dist(GuardLoc, Player->GetActorLocation());
		if (Noise > 0.f && Dist < 1500.f * Noise)
		{
			Suspicion += 0.5f;
			LastStimulusLocation = Player->GetActorLocation();
			HearingCooldown = 1.0f;
		}
	}

	Suspicion = FMath::Clamp(Suspicion, 0.f, 1.f);

	// Full suspicion => chase.
	if (Suspicion >= 1.f && bPlayerValid && State != EGuardState::Chasing)
	{
		EnterState(EGuardState::Chasing);
	}

	switch (State)
	{
	case EGuardState::Patrol:
		TickPatrol(Guard, DeltaSeconds);
		break;
	case EGuardState::Suspicious:
		TickSuspicious(Guard, DeltaSeconds);
		break;
	case EGuardState::Chasing:
		TickChasing(Guard, Player, DeltaSeconds);
		break;
	}
}

void AGuardAIController::TickPatrol(AGuardCharacter* Guard, float DeltaSeconds)
{
	if (Suspicion > 0.25f)
	{
		EnterState(EGuardState::Suspicious);
		return;
	}

	const TArray<FVector>& Points = Guard->GetPatrolPoints();
	if (Points.Num() == 0)
	{
		return;
	}
	if (!Points.IsValidIndex(PatrolIndex))
	{
		PatrolIndex = 0;
	}

	if (bPatrolPausing)
	{
		PatrolPauseTimer -= DeltaSeconds;
		if (PatrolPauseTimer <= 0.f)
		{
			bPatrolPausing = false;
			PatrolIndex = (PatrolIndex + 1) % Points.Num();
		}
		return;
	}

	const FVector Target = Points[PatrolIndex];
	const float Dist = FVector::Dist2D(Guard->GetActorLocation(), Target);
	if (Dist <= 80.f)
	{
		bPatrolPausing = true;
		PatrolPauseTimer = 2.f;
	}
	else
	{
		MoveToward(Guard, Target);
	}
}

void AGuardAIController::TickSuspicious(AGuardCharacter* Guard, float DeltaSeconds)
{
	const float Dist = FVector::Dist2D(Guard->GetActorLocation(), LastStimulusLocation);
	if (Dist <= 80.f)
	{
		SuspiciousWaitTimer -= DeltaSeconds;
		if (SuspiciousWaitTimer <= 0.f)
		{
			Suspicion = 0.f;
			EnterState(EGuardState::Patrol);
		}
	}
	else
	{
		MoveToward(Guard, LastStimulusLocation);
	}
}

void AGuardAIController::TickChasing(AGuardCharacter* Guard, ASpyCharacter* Player, float DeltaSeconds)
{
	if (!Player || Player->IsDead())
	{
		LastStimulusLocation = LastKnownPlayerLocation;
		Suspicion = 0.9f;
		EnterState(EGuardState::Suspicious);
		return;
	}

	// Chasing raises the player's global exposure.
	Player->AddDetection(0.5f * DeltaSeconds);

	const FVector PlayerLoc = Player->GetActorLocation();
	const float Dist = FVector::Dist(Guard->GetActorLocation(), PlayerLoc);

	if (bSightActive)
	{
		// Eyes on: pursue directly and arm a fresh breadcrumb trail for the
		// moment sight breaks (player slipping behind a wall at "point A").
		Breadcrumbs.Reset();
		BreadcrumbIndex = 0;
		BreadcrumbsToSample = 2;              // only the first couple of steps
		BreadcrumbSampleTimer = 0.45f;
	}
	else
	{
		// Sight lost: sample the player's next few real positions briefly -
		// the guard "knows" roughly which way you went, but not where you are.
		if (BreadcrumbsToSample > 0)
		{
			BreadcrumbSampleTimer -= DeltaSeconds;
			if (BreadcrumbSampleTimer <= 0.f)
			{
				Breadcrumbs.Add(PlayerLoc);
				--BreadcrumbsToSample;
				BreadcrumbSampleTimer = 0.45f;
			}
		}

		// Walk the trail: last-seen point first, then the breadcrumbs.
		FVector SearchTarget = LastKnownPlayerLocation;
		bool bTrailDone = false;
		if (FVector::Dist2D(Guard->GetActorLocation(), LastKnownPlayerLocation) <= 90.f)
		{
			if (Breadcrumbs.IsValidIndex(BreadcrumbIndex))
			{
				SearchTarget = Breadcrumbs[BreadcrumbIndex];
				if (FVector::Dist2D(Guard->GetActorLocation(), SearchTarget) <= 90.f)
				{
					++BreadcrumbIndex;
				}
			}
			else if (BreadcrumbsToSample <= 0)
			{
				bTrailDone = true;
			}
		}

		if (bTrailDone || TimeSinceSeenPlayer > 12.f)
		{
			// Trail went cold: search the area, then drift back to patrol.
			LastStimulusLocation = Breadcrumbs.Num() > 0 ? Breadcrumbs.Last() : LastKnownPlayerLocation;
			Suspicion = 0.6f;
			EnterState(EGuardState::Suspicious);
			return;
		}

		MoveToward(Guard, SearchTarget);
		return;
	}

	// Staggered by a counter: stand there and take it.
	if (Guard->IsStaggered())
	{
		return;
	}

	// Resolve a windup that has ripened - unless the player parried it away.
	if (PendingMeleeAt > 0.f && GetWorld()->GetTimeSeconds() >= PendingMeleeAt)
	{
		PendingMeleeAt = -1.f;
		if (!Guard->IsStaggered()
			&& FVector::Dist(Guard->GetActorLocation(), Player->GetActorLocation()) <= 240.f)
		{
			UGameplayStatics::ApplyDamage(Player, 20.f, this, Guard, nullptr);
		}
	}

	if (Dist <= 180.f)
	{
		if (MeleeCooldown <= 0.f && PendingMeleeAt < 0.f)
		{
			Guard->BeginMeleeWindup();
			PendingMeleeAt = GetWorld()->GetTimeSeconds() + 0.45f;
			MeleeCooldown = 1.6f;
		}
	}
	else
	{
		MoveToward(Guard, PlayerLoc);
	}

	// Lost the player for too long => downgrade to investigating last known spot.
	if (TimeSinceSeenPlayer >= 6.f)
	{
		LastStimulusLocation = LastKnownPlayerLocation;
		Suspicion = 0.9f;
		EnterState(EGuardState::Suspicious);
	}
}
