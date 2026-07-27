// Veil of Succession - fictional political thriller. All characters fictitious.
#include "GuardCharacter.h"
#include "GuardAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "TimerManager.h"
#include "WeaponPickup.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

AGuardCharacter::AGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Possessed by the C++ state-machine controller whether placed or spawned.
	AIControllerClass = AGuardAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->InitCapsuleSize(38.f, 90.f);

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
		Move->RotationRate = FRotator(0.f, 540.f, 0.f);
		Move->MaxWalkSpeed = 220.f;
	}
	bUseControllerRotationYaw = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	BaseMaterial = BasicMat.Succeeded() ? BasicMat.Object : nullptr;

	// Guards use the male mannequin so the player can read them at a glance.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> GuardABP(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));

	// Darts trace ECC_Visibility. The default Pawn capsule IGNORES that
	// channel, so shots passed straight through guards - the "no damage" bug.
	// Route hits through the skeletal mesh instead for per-bone (headshot) hits.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	if (MannyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannyMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		if (GuardABP.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(GuardABP.Class);
			GuardAnimClass = GuardABP.Class;
		}
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathBack(
		TEXT("/Game/Characters/Mannequins/Anims/Death/MM_Death_Back_01.MM_Death_Back_01"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathFront(
		TEXT("/Game/Characters/Mannequins/Anims/Death/MM_Death_Front_01.MM_Death_Front_01"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> MeleeSwing(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_03.MM_Attack_03"));
	MeleeAnim = MeleeSwing.Succeeded() ? MeleeSwing.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> MeleeSwingB(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_01.MM_Attack_01"));
	MeleeAnimB = MeleeSwingB.Succeeded() ? MeleeSwingB.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> HitReact(
		TEXT("/Game/Characters/Mannequins/Anims/Rifle/HitReact/MM_HitReact_Front_Med_01.MM_HitReact_Front_Med_01"));
	DeathBackAnim = DeathBack.Succeeded() ? DeathBack.Object : nullptr;
	DeathFrontAnim = DeathFront.Succeeded() ? DeathFront.Object : nullptr;
	HitReactAnim = HitReact.Succeeded() ? HitReact.Object : nullptr;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	if (CylinderMesh.Succeeded()) { BodyMesh->SetStaticMesh(CylinderMesh.Object); }
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	BodyMesh->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.8f));
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	if (SphereMesh.Succeeded()) { HeadMesh->SetStaticMesh(SphereMesh.Object); }
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 78.f));
	HeadMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftEpaulette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftEpaulette"));
	LeftEpaulette->SetupAttachment(GetCapsuleComponent());
	if (CubeMesh.Succeeded()) { LeftEpaulette->SetStaticMesh(CubeMesh.Object); }
	LeftEpaulette->SetRelativeLocation(FVector(0.f, -32.f, 40.f));
	LeftEpaulette->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.12f));
	LeftEpaulette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightEpaulette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightEpaulette"));
	RightEpaulette->SetupAttachment(GetCapsuleComponent());
	if (CubeMesh.Succeeded()) { RightEpaulette->SetStaticMesh(CubeMesh.Object); }
	RightEpaulette->SetRelativeLocation(FVector(0.f, 32.f, 40.f));
	RightEpaulette->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.12f));
	RightEpaulette->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	bDeadOrStunned = false;
	ApplyBlockoutColors();

	if (GetMesh() && GetMesh()->GetSkeletalMeshAsset())
	{
		if (BodyMesh)        { BodyMesh->SetVisibility(false); }
		if (HeadMesh)        { HeadMesh->SetVisibility(false); }
		if (LeftEpaulette)   { LeftEpaulette->SetVisibility(false); }
		if (RightEpaulette)  { RightEpaulette->SetVisibility(false); }
	}
}

FLinearColor AGuardCharacter::GetBodyColor() const
{
	// Crimson-red guard body.
	return FLinearColor(0.55f, 0.06f, 0.08f);
}

void AGuardCharacter::ApplyBlockoutColors()
{
	if (!BaseMaterial)
	{
		return;
	}

	const FLinearColor Gold(0.85f, 0.65f, 0.13f);

	BodyMID = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	if (BodyMID)
	{
		BodyMID->SetVectorParameterValue(TEXT("Color"), GetBodyColor());
		BodyMesh->SetMaterial(0, BodyMID);
	}

	HeadMID = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	if (HeadMID)
	{
		HeadMID->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.9f, 0.78f, 0.68f));
		HeadMesh->SetMaterial(0, HeadMID);
	}

	EpauletteMID = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	if (EpauletteMID)
	{
		EpauletteMID->SetVectorParameterValue(TEXT("Color"), Gold);
		LeftEpaulette->SetMaterial(0, EpauletteMID);
		RightEpaulette->SetMaterial(0, EpauletteMID);
	}
}

void AGuardCharacter::SetPatrolPoints(const TArray<FVector>& Points)
{
	PatrolPoints = Points;
}

const TArray<FVector>& AGuardCharacter::GetPatrolPoints() const
{
	return PatrolPoints;
}

bool AGuardCharacter::IsDeadOrStunned() const
{
	return bDeadOrStunned;
}

float AGuardCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bDeadOrStunned)
	{
		return Applied;
	}

	Health -= DamageAmount;
	if (Health <= 0.f)
	{
		Health = 0.f;
		Die();
	}
	else
	{
		PlayHitReact();
	}
	return Applied;
}

void AGuardCharacter::DropWeapon()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWeaponPickup* Drop = World->SpawnActor<AWeaponPickup>(AWeaponPickup::StaticClass(),
		GetActorLocation() + FVector(60.f, 0.f, -60.f), FRotator::ZeroRotator, Params);
	if (Drop)
	{
		// Most guards carry pistols; the occasional rifleman is a jackpot.
		const bool bRifle = FMath::FRand() < 0.3f;
		Drop->WeaponType = bRifle ? EVeilWeapon::AssaultRifle : EVeilWeapon::GuardPistol;
		Drop->AmmoAmount = bRifle ? 20 : 10;
	}
}

void AGuardCharacter::BeginMeleeWindup()
{
	if (bDeadOrStunned || IsStaggered())
	{
		return;
	}
	bWindingUp = true;
	WindupUntil = GetWorld() ? GetWorld()->GetTimeSeconds() + 0.45f : 0.f;
	PlayMeleeSwing();
}

bool AGuardCharacter::IsWindingUp() const
{
	return bWindingUp && GetWorld() && GetWorld()->GetTimeSeconds() < WindupUntil;
}

void AGuardCharacter::Stagger(float Seconds)
{
	if (bDeadOrStunned)
	{
		return;
	}
	bWindingUp = false;
	StaggerUntil = GetWorld() ? GetWorld()->GetTimeSeconds() + Seconds : 0.f;
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->StopMovementImmediately();
	}
	PlayHitReact();
}

bool AGuardCharacter::IsStaggered() const
{
	return GetWorld() && GetWorld()->GetTimeSeconds() < StaggerUntil;
}

void AGuardCharacter::PlayMeleeSwing()
{
	if (bDeadOrStunned || !MeleeAnim || !GetMesh())
	{
		return;
	}
	UAnimSequence* Swing = (MeleeAnimB && FMath::RandBool()) ? MeleeAnimB : MeleeAnim;
	GetMesh()->PlayAnimation(Swing, false);
	GetWorldTimerManager().SetTimer(ReactTimer, [this]()
	{
		if (!bDeadOrStunned && GetMesh() && GuardAnimClass)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(GuardAnimClass);
		}
	}, 0.8f, false);
}

void AGuardCharacter::PlayHitReact()
{
	if (bDeadOrStunned || !HitReactAnim || !GetMesh())
	{
		return;
	}
	GetMesh()->PlayAnimation(HitReactAnim, false);
	GetWorldTimerManager().SetTimer(ReactTimer, [this]()
	{
		if (!bDeadOrStunned && GetMesh() && GuardAnimClass)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(GuardAnimClass);
		}
	}, 0.7f, false);
}

void AGuardCharacter::KnifeKill(bool bFromBehind)
{
	if (bDeadOrStunned)
	{
		return;
	}
	bDeadOrStunned = true;
	Health = 0.f;

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->DisableMovement();
	}

	// Animated fall - back-stab and frontal kills read differently. The
	// single-node player holds the final frame, leaving the body on the floor.
	UAnimSequence* DeathAnim = bFromBehind ? DeathBackAnim : DeathFrontAnim;
	if (DeathAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(DeathAnim, false);
	}

	DropWeapon();
	UE_LOG(LogVeil, Log, TEXT("%s knifed."), *GetName());
}

void AGuardCharacter::Die()
{
	if (bDeadOrStunned)
	{
		return;
	}
	bDeadOrStunned = true;

	// Fake ragdoll: no capsule collision, tip the body over, stop moving.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// Prefer a real ragdoll on the skeletal mesh; fall back to tipping the
	// blockout cylinder if the mannequin has no physics asset.
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (SkelMesh && SkelMesh->GetSkeletalMeshAsset() && SkelMesh->GetPhysicsAsset())
	{
		SkelMesh->SetCollisionProfileName(TEXT("Ragdoll"));
		SkelMesh->SetAllBodiesSimulatePhysics(true);
		SkelMesh->SetSimulatePhysics(true);
		SkelMesh->WakeAllRigidBodies();
	}
	else if (BodyMesh)
	{
		BodyMesh->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	}
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->DisableMovement();
	}

	DropWeapon();

	// The GuardAIController halts all logic once IsDeadOrStunned() is true.
	UE_LOG(LogVeil, Log, TEXT("%s is down."), *GetName());
}
