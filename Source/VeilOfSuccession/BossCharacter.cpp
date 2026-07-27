// Veil of Succession - fictional political thriller. All characters fictitious.
#include "BossCharacter.h"
#include "MissionSubsystem.h"
#include "Engine/DamageEvents.h"
#include "Engine/GameInstance.h"
#include "SpyCharacter.h"
#include "MissionSubsystem.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Boss handles its own combat in Tick; it does not use the guard state machine.
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MaxHealth = 600.f;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Slightly larger, imposing silhouette.
	SetActorScale3D(FVector(1.15f));
}

FLinearColor ABossCharacter::GetBodyColor() const
{
	// Gold-colored blockout body.
	return FLinearColor(0.85f, 0.65f, 0.13f);
}

float ABossCharacter::GetBossHealthPct() const
{
	return (MaxHealth > 0.f) ? FMath::Clamp(Health / MaxHealth, 0.f, 1.f) : 0.f;
}

bool ABossCharacter::IsActivated() const
{
	return bActivated;
}

void ABossCharacter::Activate()
{
	if (bActivated || IsDeadOrStunned())
	{
		return;
	}
	bActivated = true;
	CurrentPhase = 1;

	// Repeating pistol-volley cadence (each begins with a telegraph).
	GetWorldTimerManager().SetTimer(VolleyTimer, this, &ABossCharacter::StartVolleyTelegraph, 2.5f, true, 1.0f);

	UE_LOG(LogVeil, Log, TEXT("Boss activated."));
}

ASpyCharacter* ABossCharacter::ResolvePlayer() const
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

void ABossCharacter::StartVolleyTelegraph()
{
	if (!bActivated || IsDeadOrStunned())
	{
		return;
	}
	ASpyCharacter* Player = ResolvePlayer();
	if (!Player || Player->IsDead())
	{
		return;
	}

	const FVector Muzzle = GetActorLocation() + FVector(0.f, 0.f, 40.f);
	const FVector Target = Player->GetActorLocation();

	// No line of sight, no attack: check for world geometry between us first,
	// so neither the beam nor the shot ever ignores a wall.
	FHitResult LoSHit;
	FCollisionQueryParams LoSParams;
	LoSParams.AddIgnoredActor(this);
	LoSParams.AddIgnoredActor(Player);
	const bool bBlocked = GetWorld()->LineTraceSingleByChannel(
		LoSHit, Muzzle, Target, ECC_Visibility, LoSParams);
	if (bBlocked)
	{
		return;   // player is behind cover - hold fire
	}

	// 0.8s red telegraph beam before the shot lands.
	DrawDebugLine(GetWorld(), Muzzle, Target, FColor::Red, false, 0.8f, 0, 3.f);

	GetWorldTimerManager().SetTimer(TelegraphTimer, this, &ABossCharacter::FirePistolVolley, 0.8f, false);
}

void ABossCharacter::FirePistolVolley()
{
	if (!bActivated || IsDeadOrStunned())
	{
		return;
	}
	ASpyCharacter* Player = ResolvePlayer();
	if (!Player || Player->IsDead())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Muzzle = GetActorLocation() + FVector(0.f, 0.f, 40.f);
	// Line-trace at the player's position at fire time.
	const FVector End = Player->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (World->LineTraceSingleByChannel(Hit, Muzzle, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UGameplayStatics::ApplyDamage(HitActor, 8.f, GetController(), this, nullptr);
		}
	}
}

void ABossCharacter::SummonReinforcements()
{
	if (bReinforcementsSummoned)
	{
		return;
	}
	bReinforcementsSummoned = true;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Base = GetActorLocation();

	// Short patrol route around the throne room.
	TArray<FVector> Route;
	Route.Add(Base + FVector(400.f, 400.f, 0.f));
	Route.Add(Base + FVector(-400.f, 400.f, 0.f));
	Route.Add(Base + FVector(-400.f, -400.f, 0.f));
	Route.Add(Base + FVector(400.f, -400.f, 0.f));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < 2; ++i)
	{
		const FVector Loc = Base + FVector((i == 0 ? 300.f : -300.f), 250.f, 0.f);
		AGuardCharacter* Reinforcement = World->SpawnActor<AGuardCharacter>(
			AGuardCharacter::StaticClass(), Loc, GetActorRotation(), SpawnParams);
		if (Reinforcement)
		{
			Reinforcement->SetPatrolPoints(Route);
		}
	}

	UE_LOG(LogVeil, Log, TEXT("Boss summoned reinforcements."));
}

void ABossCharacter::UpdatePhase()
{
	const float Pct = GetBossHealthPct();

	int32 NewPhase = 1;
	if (Pct <= 0.33f)      { NewPhase = 3; }
	else if (Pct <= 0.66f) { NewPhase = 2; }

	if (NewPhase != CurrentPhase)
	{
		CurrentPhase = NewPhase;

		if (CurrentPhase >= 2)
		{
			SummonReinforcements(); // one-time (guarded)
		}
		if (CurrentPhase >= 3)
		{
			if (UCharacterMovementComponent* Move = GetCharacterMovement())
			{
				Move->MaxWalkSpeed = BaseWalkSpeed * 1.5f;
			}
		}
	}
}

void ABossCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bActivated || IsDeadOrStunned())
	{
		return;
	}

	UpdatePhase();

	if (ChargeCooldown > 0.f)
	{
		ChargeCooldown -= DeltaSeconds;
	}

	// Phase 3: charge the player and slam on contact.
	if (CurrentPhase >= 3)
	{
		ASpyCharacter* Player = ResolvePlayer();
		if (Player && !Player->IsDead())
		{
			const FVector PlayerLoc = Player->GetActorLocation();
			const FVector Dir = (PlayerLoc - GetActorLocation()).GetSafeNormal2D();
			if (!Dir.IsNearlyZero())
			{
				AddMovementInput(Dir);
			}

			const float Dist = FVector::Dist(GetActorLocation(), PlayerLoc);
			float ContactRange = 150.f;
			if (const UCapsuleComponent* Capsule = GetCapsuleComponent())
			{
				ContactRange = Capsule->GetScaledCapsuleRadius() + 90.f;
			}

			if (Dist <= ContactRange && ChargeCooldown <= 0.f)
			{
				UGameplayStatics::ApplyDamage(Player, 35.f, GetController(), this, nullptr);
				ChargeCooldown = 4.f;
			}
		}
	}
}

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsActivated())
	{
		// Bodyguards, ceremony, plot armor - call it what you like: the General
		// cannot be killed before his confrontation. Attacking him early only
		// starts the fight if the campaign has reached the throne room act.
		bool bThroneAct = false;
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMissionSubsystem* Missions = GI->GetSubsystem<UMissionSubsystem>())
			{
				bThroneAct = Missions->GetCurrentMissionIndex() >= 2;
			}
		}
		if (bThroneAct)
		{
			Activate();
		}
		return 0.f;
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABossCharacter::KnifeKill(bool bFromBehind)
{
	// A knife hurts the General badly but never one-shots him.
	if (!IsActivated())
	{
		return;
	}
	FDamageEvent Ev;
	TakeDamage(140.f, Ev, nullptr, nullptr);
}

void ABossCharacter::Die()
{
	if (IsDeadOrStunned())
	{
		return;
	}

	// Stop combat cadence.
	GetWorldTimerManager().ClearTimer(VolleyTimer);
	GetWorldTimerManager().ClearTimer(TelegraphTimer);

	// Fake-ragdoll like guards.
	Super::Die();

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UMissionSubsystem* Mission = GI->GetSubsystem<UMissionSubsystem>())
		{
			Mission->NotifyBossDefeated();
		}
	}

	UE_LOG(LogVeil, Log, TEXT("Boss defeated."));
}
