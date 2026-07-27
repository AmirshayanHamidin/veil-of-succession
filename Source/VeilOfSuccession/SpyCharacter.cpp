// Veil of Succession - fictional political thriller. All characters fictitious.
#include "SpyCharacter.h"

#include "StealthComponent.h"
#include "InteractableActor.h"
#include "GuardCharacter.h"
#include "BossCharacter.h"
#include "MissionSubsystem.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Hearing.h"
#include "Components/PointLightComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "InputActionValue.h"
#include "CollisionQueryParams.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"


ASpyCharacter::ASpyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.f;
	Health = MaxHealth;
	NoiseLevel = 0.f;
	Detection = 0.f;
	Ammo = 12;
	bIsAiming = false;
	bIsSprinting = false;
	bDead = false;

	WalkSpeed = 380.f;
	SprintSpeed = 750.f;   // clear, readable difference from a walk
	CrouchSpeed = 175.f;   // slow and obviously deliberate

	MaxShield = 100.f;
	Shield = MaxShield;
	bShieldActive = false;

	DefaultArmLength = 380.f;
	AimArmLength = 180.f;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Movement config
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 540.f, 0.f);
	Move->MaxWalkSpeed = WalkSpeed;
	Move->MaxWalkSpeedCrouched = CrouchSpeed;
	Move->NavAgentProps.bCanCrouch = true;
	Move->JumpZVelocity = 560.f;    // clears the courtyard cover blocks
	Move->AirControl = 0.35f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Spring arm + camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->SocketOffset = FVector(0.f, 60.f, 70.f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Blockout visuals
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	// --- Real character: Unreal's rigged mannequin + locomotion anim blueprint.
	// Verified paths from the Third Person content pack in this project.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> QuinnMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedABP(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));

	if (QuinnMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(QuinnMesh.Object);
		// Feet on the capsule floor, facing +X.
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		if (UnarmedABP.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(UnarmedABP.Class);
			DefaultAnimClass = UnarmedABP.Class;
		}
	}

	// Pistol aim-down-sights pose (female mannequin variant, ships with 5.7).
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ADSPose(
		TEXT("/Game/Characters/Mannequins/Anims/Pistol/MF_Pistol_Idle_ADS.MF_Pistol_Idle_ADS"));
	AimPose = ADSPose.Succeeded() ? ADSPose.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> KnifeA(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_01.MM_Attack_01"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> KnifeB(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_02.MM_Attack_02"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> KnifeStealth(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_ChargedAttack.MM_ChargedAttack"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> KnifeC(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_03.MM_Attack_03"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Dash(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/Jump/MM_Dash.MM_Dash"));
	KnifeAnimA = KnifeA.Succeeded() ? KnifeA.Object : nullptr;
	KnifeAnimB = KnifeB.Succeeded() ? KnifeB.Object : nullptr;
	KnifeAnimC = KnifeC.Succeeded() ? KnifeC.Object : nullptr;
	DashAnim = Dash.Succeeded() ? Dash.Object : nullptr;
	StealthKillAnim = KnifeStealth.Succeeded() ? KnifeStealth.Object : nullptr;

	// Tranq pistol built from primitives, socketed to the skeleton's right hand.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GunCube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GunMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	PistolSlide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolSlide"));
	PistolSlide->SetupAttachment(GetMesh(), TEXT("hand_r"));
	if (GunCube.Succeeded()) { PistolSlide->SetStaticMesh(GunCube.Object); }
	PistolSlide->SetRelativeLocation(FVector(-9.f, 3.f, 0.5f));
	PistolSlide->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	PistolSlide->SetRelativeScale3D(FVector(0.22f, 0.045f, 0.05f));
	PistolSlide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PistolSlide->SetCastShadow(true);

	PistolGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolGrip"));
	PistolGrip->SetupAttachment(GetMesh(), TEXT("hand_r"));
	if (GunCube.Succeeded()) { PistolGrip->SetStaticMesh(GunCube.Object); }
	PistolGrip->SetRelativeLocation(FVector(-2.5f, 3.f, -2.6f));
	PistolGrip->SetRelativeRotation(FRotator(18.f, 0.f, 0.f));
	PistolGrip->SetRelativeScale3D(FVector(0.05f, 0.04f, 0.075f));
	PistolGrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Muzzle flash light, flicked on for a frame per shot.
	MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleLight"));
	MuzzleLight->SetupAttachment(PistolSlide);
	MuzzleLight->SetRelativeLocation(FVector(60.f, 0.f, 0.f));
	MuzzleLight->SetIntensity(0.f);
	MuzzleLight->SetLightColor(FLinearColor(1.f, 0.75f, 0.35f));
	MuzzleLight->SetAttenuationRadius(420.f);
	MuzzleLight->SetCastShadows(false);

	// Ballistic shield on the left arm; only visible while raised.
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(GetMesh(), TEXT("hand_l"));
	if (GunCube.Succeeded()) { ShieldMesh->SetStaticMesh(GunCube.Object); }
	ShieldMesh->SetRelativeLocation(FVector(-12.f, 0.f, 0.f));
	ShieldMesh->SetRelativeScale3D(FVector(0.06f, 0.55f, 0.7f));
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldMesh->SetVisibility(false);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	if (CylinderMesh.Succeeded())
	{
		BodyMesh->SetStaticMesh(CylinderMesh.Object);
	}
	BodyMesh->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.9f));
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (BaseMat.Succeeded())
	{
		BodyMesh->SetMaterial(0, BaseMat.Object);
	}

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	if (SphereMesh.Succeeded())
	{
		HeadMesh->SetStaticMesh(SphereMesh.Object);
	}
	HeadMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (BaseMat.Succeeded())
	{
		HeadMesh->SetMaterial(0, BaseMat.Object);
	}

	// Stealth component
	StealthComponent = CreateDefaultSubobject<UStealthComponent>(TEXT("StealthComponent"));
}

void ASpyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// The mannequin replaces the capsule-and-sphere stand-in; keep the blockout
	// only as a fallback if the Third Person content is ever missing.
	if (GetMesh() && GetMesh()->GetSkeletalMeshAsset())
	{
		if (BodyMesh) { BodyMesh->SetVisibility(false); }
		if (HeadMesh) { HeadMesh->SetVisibility(false); }
	}

	// Dark-teal body, lighter head.
	if (UMaterialInterface* BodyBase = BodyMesh ? BodyMesh->GetMaterial(0) : nullptr)
	{
		BodyMID = UMaterialInstanceDynamic::Create(BodyBase, this);
		if (BodyMID)
		{
			BodyMID->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.05f, 0.22f, 0.24f));
			BodyMesh->SetMaterial(0, BodyMID);
		}
	}
	if (UMaterialInterface* HeadBase = HeadMesh ? HeadMesh->GetMaterial(0) : nullptr)
	{
		HeadMID = UMaterialInstanceDynamic::Create(HeadBase, this);
		if (HeadMID)
		{
			HeadMID->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.15f, 0.42f, 0.44f));
			HeadMesh->SetMaterial(0, HeadMID);
		}
	}
}

void ASpyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateNoiseLevel();

	// Detection decays when no guard is contributing this frame.
	if (Detection > 0.f)
	{
		Detection = FMath::Clamp(Detection - 0.15f * DeltaSeconds, 0.f, 1.f);
	}

	// Darts slowly refabricate - being detected never permanently disarms you.
	if (Ammo < 12)
	{
		DartRegenTimer += DeltaSeconds;
		if (DartRegenTimer >= 4.f)
		{
			DartRegenTimer = 0.f;
			++Ammo;
		}
	}

	// Shield drains while raised and slowly recovers once lowered.
	if (bShieldActive)
	{
		Shield = FMath::Clamp(Shield - 12.f * DeltaSeconds, 0.f, MaxShield);
		if (Shield <= 0.f)
		{
			bShieldActive = false;
			if (ShieldMesh) { ShieldMesh->SetVisibility(false); }
			if (!bIsCrouched)
			{
				GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
			}
		}
	}
	else if (Shield < MaxShield)
	{
		Shield = FMath::Clamp(Shield + 5.f * DeltaSeconds, 0.f, MaxShield);
	}

	// Target lock: camera glides to keep the marked guard centered; breaks on
	// death or distance (souls-like rule).
	if (LockTarget.IsValid())
	{
		AGuardCharacter* T = LockTarget.Get();
		const float Dist = FVector::Dist(GetActorLocation(), T->GetActorLocation());
		if (T->IsDeadOrStunned() || Dist > 2800.f)
		{
			LockTarget = nullptr;
		}
		else if (Controller)
		{
			const FRotator Want = (T->GetActorLocation() + FVector(0.f, 0.f, 40.f)
				- (Camera ? Camera->GetComponentLocation() : GetActorLocation())).Rotation();
			const FRotator Cur = Controller->GetControlRotation();
			Controller->SetControlRotation(FMath::RInterpTo(Cur, Want, DeltaSeconds, 7.f));
		}
	}

	// Full-auto: the rifle keeps firing while the trigger is held.
	if (bFireHeld && !bDead && CurrentWeapon == EVeilWeapon::AssaultRifle)
	{
		const float Now2 = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
		if (Now2 - LastFireTime >= 0.11f)
		{
			LastFireTime = Now2;
			ApplyTranqFire();
		}
	}

	// FOV: wide sprint kick, tight aim focus - speed you can feel (CoD rule).
	if (Camera)
	{
		float TargetFOV = 90.f;
		if (bIsAiming) { TargetFOV = 70.f; }
		else if (IsSprinting()) { TargetFOV = 97.f; }
		Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaSeconds, 8.f));
	}

	// Smoothly interpolate camera arm toward aim/normal length.
	if (SpringArm)
	{
		const float Target = bIsAiming ? AimArmLength : DefaultArmLength;
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, Target, DeltaSeconds, 10.f);
	}
}

float ASpyCharacter::GetHealth() const { return Health; }
float ASpyCharacter::GetMaxHealth() const { return MaxHealth; }
bool ASpyCharacter::IsDead() const { return bDead; }
float ASpyCharacter::GetNoiseLevel() const { return NoiseLevel; }
float ASpyCharacter::GetDetection() const { return Detection; }
int32 ASpyCharacter::GetAmmo() const { return Ammo; }
bool ASpyCharacter::IsAiming() const { return bIsAiming; }

UStealthComponent* ASpyCharacter::GetStealthComponent() const { return StealthComponent; }

void ASpyCharacter::AddDetection(float Amount)
{
	Detection = FMath::Clamp(Detection + Amount, 0.f, 1.f);
}

void ASpyCharacter::UpdateNoiseLevel()
{
	if (bDead)
	{
		NoiseLevel = 0.f;
		return;
	}

	const float Speed2D = GetVelocity().Size2D();
	const bool bMoving = Speed2D > 50.f;
	const bool bCrouched = bIsCrouched;

	if (bIsSprinting && bMoving && !bCrouched)
	{
		NoiseLevel = 1.0f;
	}
	else if (bCrouched && bMoving)
	{
		NoiseLevel = 0.15f;
	}
	else if (bMoving)
	{
		NoiseLevel = 0.6f;
	}
	else
	{
		NoiseLevel = 0.0f;
	}
}

float ASpyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bDead)
	{
		return 0.f;
	}

	// Mid-dodge = invulnerable. The roll is a real defensive verb, not decoration.
	if (bDodging)
	{
		return 0.f;
	}

	// Camera flinch so getting hit is felt, not just read off the health bar.
	AddControllerPitchInput(FMath::FRandRange(-0.8f, -0.3f));
	AddControllerYawInput(FMath::FRandRange(-0.5f, 0.5f));

	// Remember where the hit came from for the HUD direction indicator.
	if (DamageCauser)
	{
		LastDamageDir = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		LastDamagedTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	}

	// Raised shield soaks damage until it breaks, then health takes the rest.
	float Remaining = DamageAmount;
	if (bShieldActive && Shield > 0.f)
	{
		const float Absorbed = FMath::Min(Shield, Remaining);
		Shield -= Absorbed;
		Remaining -= Absorbed;
		if (Shield <= 0.f)
		{
			Shield = 0.f;
			bShieldActive = false;
			if (ShieldMesh) { ShieldMesh->SetVisibility(false); }
		}
	}

	Health = FMath::Clamp(Health - Remaining, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		bDead = true;

		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMissionSubsystem* Mission = GI->GetSubsystem<UMissionSubsystem>())
			{
				Mission->NotifyPlayerDied();
			}
		}

		if (AController* C = GetController())
		{
			DisableInput(Cast<APlayerController>(C));
		}
		GetCharacterMovement()->DisableMovement();
	}

	return Applied;
}

void ASpyCharacter::HandleMove(const FInputActionValue& Value)
{
	if (bDead || !Controller)
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Axis.Y);
	AddMovementInput(Right, Axis.X);
}

void ASpyCharacter::HandleLook(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void ASpyCharacter::HandleSprintStart(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}
	bIsSprinting = true;
	if (!bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASpyCharacter::HandleSprintStop(const FInputActionValue& Value)
{
	bIsSprinting = false;
	if (!bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ASpyCharacter::HandleCrouchToggle(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}
	if (bIsCrouched)
	{
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
		if (GetMesh()) { GetMesh()->SetRelativeScale3D(FVector(1.f, 1.f, 1.f)); }
	}
	else
	{
		Crouch();
		// The mannequin pack ships no crouch animation, so compress the pose -
		// a stand-in until a real crouch anim asset is added.
		if (GetMesh()) { GetMesh()->SetRelativeScale3D(FVector(1.f, 1.f, 0.72f)); }
	}
}

void ASpyCharacter::HandleAimStart(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}
	bIsAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	// MF_Pistol_Idle_ADS verified NON-additive (asset scan 2026-07-20):
	// full-body two-handed aim stance. MM_Pistol_Fire verified additive and
	// must never be single-noded.
	if (AimPose && GetMesh())
	{
		GetMesh()->PlayAnimation(AimPose, true);
	}

	// Precision costs mobility.
	if (!bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = 210.f;
	}
}

void ASpyCharacter::HandleAimStop(const FInputActionValue& Value)
{
	bIsAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	if (GetMesh() && DefaultAnimClass)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
	}
	if (!bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	}
}

void ASpyCharacter::HandleFire(const FInputActionValue& Value)
{
	// Previously this required bIsAiming, so a click without holding aim did
	// nothing at all and read as "shooting is broken". Hip fire is allowed now;
	// aiming just tightens the shot and zooms the camera.
	if (bDead)
	{
		return;
	}
	const UWorld* W = GetWorld();
	const float Now = W ? W->GetTimeSeconds() : 0.f;
	// Per-weapon cadence: pistol is slow & heavy, rifle is fast & light.
	float Interval = 0.9f;                                     // tranq
	if (CurrentWeapon == EVeilWeapon::GuardPistol) { Interval = 0.85f; }
	else if (CurrentWeapon == EVeilWeapon::AssaultRifle) { Interval = 0.11f; }
	if (Now - LastFireTime < Interval)
	{
		return;
	}
	LastFireTime = Now;
	bFireHeld = true;
	ApplyTranqFire();
}

void ASpyCharacter::HandleFireStop(const FInputActionValue& Value)
{
	bFireHeld = false;
}

bool ASpyCharacter::IsSprinting() const
{
	return bIsSprinting && !bIsCrouched && GetVelocity().SizeSquared2D() > 100.f;
}

bool ASpyCharacter::IsCrouching() const
{
	return bIsCrouched;
}

float ASpyCharacter::GetTimeSinceEmptyFire() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() - LastEmptyFireTime : 1000.f;
}

float ASpyCharacter::GetTimeSinceDamaged() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() - LastDamagedTime : 1000.f;
}

FVector ASpyCharacter::GetLastDamageDirection() const { return LastDamageDir; }

float ASpyCharacter::GetTimeSinceHitMarker() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() - LastHitMarkerTime : 1000.f;
}

float ASpyCharacter::GetShield() const { return Shield; }
float ASpyCharacter::GetMaxShield() const { return MaxShield; }
bool ASpyCharacter::IsShieldActive() const { return bShieldActive && Shield > 0.f; }

AGuardCharacter* ASpyCharacter::FindTakedownTarget() const
{
	UWorld* World = GetWorld();
	if (!World || bDead)
	{
		return nullptr;
	}

	const FVector MyLoc = GetActorLocation();
	AGuardCharacter* Best = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	for (TActorIterator<AGuardCharacter> It(World); It; ++It)
	{
		AGuardCharacter* Guard = *It;
		if (!Guard || Guard->IsDeadOrStunned() || Cast<ABossCharacter>(Guard))
		{
			continue;
		}

		const FVector ToMe = MyLoc - Guard->GetActorLocation();
		const float DistSq = ToMe.SizeSquared();
		if (DistSq > FMath::Square(220.f))
		{
			continue;
		}

		// Must be behind the guard: their forward points away from us.
		const FVector GuardFwd = Guard->GetActorForwardVector().GetSafeNormal2D();
		if (FVector::DotProduct(GuardFwd, ToMe.GetSafeNormal2D()) > -0.25f)
		{
			continue;
		}

		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			Best = Guard;
		}
	}
	return Best;
}

void ASpyCharacter::PlayFullBodyAnim(UAnimSequence* Anim, float RestoreAfter)
{
	if (!Anim || !GetMesh())
	{
		return;
	}

	// Prefer a blended dynamic montage (0.2s in/out through the ABP slot) -
	// the closest to motion-matched feel this animation set allows. Falls back
	// to hard single-node playback if the ABP has no slot.
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		UAnimMontage* Montage = UAnimMontage::CreateSlotAnimationAsDynamicMontage(
			Anim, TEXT("DefaultSlot"), 0.2f, 0.2f);
		if (Montage && AnimInst->Montage_Play(Montage) > 0.f)
		{
			GetWorldTimerManager().SetTimer(KnifeTimer, [this]() { bKnifing = false; },
				RestoreAfter, false);
			return;
		}
	}

	GetMesh()->PlayAnimation(Anim, false);
	GetWorldTimerManager().SetTimer(KnifeTimer, [this]()
	{
		bKnifing = false;
		if (GetMesh() && DefaultAnimClass)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
		}
	}, RestoreAfter, false);
}

void ASpyCharacter::HandleTakedown(const FInputActionValue& Value)
{
	HandleKnife(Value);
}

void ASpyCharacter::HandleKnife(const FInputActionValue& Value)
{
	if (bDead || bKnifing)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// COUNTER: knife pressed while a nearby guard is mid-windup = parry.
	// His attack dies, he staggers, and the riposte hits for triple damage.
	for (TActorIterator<AGuardCharacter> It(World); It; ++It)
	{
		AGuardCharacter* G = *It;
		if (!G || G->IsDeadOrStunned() || !G->IsWindingUp())
		{
			continue;
		}
		if (FVector::DistSquared(GetActorLocation(), G->GetActorLocation()) > FMath::Square(300.f))
		{
			continue;
		}
		G->Stagger(1.6f);   // longer opening instead of a kill
		bKnifing = true;
		PlayFullBodyAnim(KnifeAnimB, 0.55f);
		// A counter is a strong riposte, not an execution: ~half a guard's
		// health, and the long stagger lets you follow up with the combo.
		UGameplayStatics::ApplyDamage(G, 55.f, GetController(), this, nullptr);
		LastCounterTime = World->GetTimeSeconds();
		// Hit-stop: a beat of slow motion sells the impact (God of War / AC juice).
		UGameplayStatics::SetGlobalTimeDilation(this, 0.35f);
		FTimerHandle HS; GetWorldTimerManager().SetTimer(HS,
			[this]() { UGameplayStatics::SetGlobalTimeDilation(this, 1.f); }, 0.05f, false);
		LastHitMarkerTime = LastCounterTime;
		return;
	}

	if (AGuardCharacter* Target = FindTakedownTarget())
	{
		// Stealth kill from behind: charged strike, guard folds with a death
		// animation, zero noise.
		bKnifing = true;
		PlayFullBodyAnim(StealthKillAnim, 1.1f);

		TWeakObjectPtr<AGuardCharacter> WeakTarget = Target;
		FTimerHandle KillDelay;
		GetWorldTimerManager().SetTimer(KillDelay, [WeakTarget]()
		{
			if (WeakTarget.IsValid())
			{
				WeakTarget->KnifeKill(true);
			}
		}, 0.35f, false);
		UGameplayStatics::SetGlobalTimeDilation(this, 0.4f);
		FTimerHandle HS2; GetWorldTimerManager().SetTimer(HS2,
			[this]() { UGameplayStatics::SetGlobalTimeDilation(this, 1.f); }, 0.08f, false);
		return;
	}

	// Frontal knife: three-hit combo chain (AC-style). Attacking again inside
	// the chain window advances the combo instead of repeating one swing.
	const float Now = World->GetTimeSeconds();
	ComboIndex = (Now - LastKnifeTime < 1.4f) ? (ComboIndex + 1) % 3 : 0;
	LastKnifeTime = Now;
	UAnimSequence* ComboAnims[3] = { KnifeAnimA, KnifeAnimB, KnifeAnimC };

	bKnifing = true;
	PlayFullBodyAnim(ComboAnims[ComboIndex], 0.55f);

	// Lunge into the strike - closes distance and sells the momentum.
	LaunchCharacter(GetActorForwardVector() * 420.f + FVector(0.f, 0.f, 60.f), true, false);

	const FVector MyLoc = GetActorLocation();
	const FVector Fwd = GetActorForwardVector().GetSafeNormal2D();
	for (TActorIterator<AGuardCharacter> It(World); It; ++It)
	{
		AGuardCharacter* Guard = *It;
		if (!Guard || Guard->IsDeadOrStunned())
		{
			continue;
		}
		const FVector To = Guard->GetActorLocation() - MyLoc;
		if (To.SizeSquared() > FMath::Square(200.f))
		{
			continue;
		}
		if (FVector::DotProduct(Fwd, To.GetSafeNormal2D()) < 0.35f)
		{
			continue;
		}
		UGameplayStatics::ApplyDamage(Guard, 70.f, GetController(), this, nullptr);
		LastHitMarkerTime = World->GetTimeSeconds();
		break;
	}
	UAISense_Hearing::ReportNoiseEvent(World, MyLoc, 0.7f, this, 1200.f);
}

void ASpyCharacter::HandleJumpStart(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}
	if (bIsCrouched)
	{
		UnCrouch();   // stand first; second press jumps
		return;
	}
	Jump();
}

void ASpyCharacter::HandleJumpStop(const FInputActionValue& Value)
{
	StopJumping();
}

void ASpyCharacter::HandleDodge(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (bDead || bDodging || !World || World->GetTimeSeconds() - LastDodgeTime < 0.8f)
	{
		return;
	}
	LastDodgeTime = World->GetTimeSeconds();
	bDodging = true;

	if (bIsCrouched)
	{
		UnCrouch();
	}

	// Dodge toward the stick/keys; with no input, roll away from where you face.
	FVector Dir = GetCharacterMovement()->GetLastInputVector().GetSafeNormal2D();
	if (Dir.IsNearlyZero())
	{
		Dir = -GetActorForwardVector().GetSafeNormal2D();
	}
	LaunchCharacter(Dir * 1450.f + FVector(0.f, 0.f, 120.f), true, true);
	PlayFullBodyAnim(DashAnim, 0.55f);

	// I-frames: untouchable for the first 0.4s of the roll.
	GetWorldTimerManager().SetTimer(DodgeTimer, [this]() { bDodging = false; }, 0.4f, false);
}

void ASpyCharacter::HandleLockOn(const FInputActionValue& Value)
{
	if (LockTarget.IsValid())
	{
		LockTarget = nullptr;   // toggle off
		return;
	}
	UWorld* World = GetWorld();
	if (!World || bDead)
	{
		return;
	}
	float BestScore = TNumericLimits<float>::Max();
	AGuardCharacter* Best = nullptr;
	const FVector Fwd = GetActorForwardVector();
	for (TActorIterator<AGuardCharacter> It(World); It; ++It)
	{
		AGuardCharacter* G = *It;
		if (!G || G->IsDeadOrStunned())
		{
			continue;
		}
		const FVector To = G->GetActorLocation() - GetActorLocation();
		const float Dist = To.Size();
		if (Dist > 2500.f || FVector::DotProduct(Fwd, To.GetSafeNormal()) < 0.1f)
		{
			continue;
		}
		if (Dist < BestScore)
		{
			BestScore = Dist;
			Best = G;
		}
	}
	LockTarget = Best;
}

AGuardCharacter* ASpyCharacter::GetLockTarget() const
{
	return LockTarget.IsValid() ? LockTarget.Get() : nullptr;
}

float ASpyCharacter::GetTimeSinceCounter() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() - LastCounterTime : 1000.f;
}

void ASpyCharacter::HandleShieldStart(const FInputActionValue& Value)
{
	if (bDead || Shield <= 0.f)
	{
		return;
	}
	bShieldActive = true;
	if (ShieldMesh) { ShieldMesh->SetVisibility(true); }
	// Shield is heavy: you cannot sprint behind it.
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}

void ASpyCharacter::HandleShieldStop(const FInputActionValue& Value)
{
	bShieldActive = false;
	if (ShieldMesh) { ShieldMesh->SetVisibility(false); }
	if (!bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	}
}

void ASpyCharacter::GiveWeapon(EVeilWeapon Weapon, int32 AmmoAmount)
{
	switch (Weapon)
	{
	case EVeilWeapon::GuardPistol:
		bHasGuardPistol = true;
		PistolAmmo += AmmoAmount;
		break;
	case EVeilWeapon::AssaultRifle:
		bHasRifle = true;
		RifleAmmo += AmmoAmount;
		break;
	default:
		Ammo = FMath::Min(Ammo + AmmoAmount, 12);
		break;
	}
	LastHitMarkerTime = GetWorld() ? GetWorld()->GetTimeSeconds() : LastHitMarkerTime;
}

EVeilWeapon ASpyCharacter::GetCurrentWeapon() const { return CurrentWeapon; }
bool ASpyCharacter::HasWeapon(EVeilWeapon Weapon) const
{
	switch (Weapon)
	{
	case EVeilWeapon::GuardPistol: return bHasGuardPistol;
	case EVeilWeapon::AssaultRifle: return bHasRifle;
	default: return true;
	}
}

FString ASpyCharacter::GetWeaponName() const
{
	switch (CurrentWeapon)
	{
	case EVeilWeapon::GuardPistol: return TEXT("GUARD PISTOL");
	case EVeilWeapon::AssaultRifle: return TEXT("RIFLE");
	default: return TEXT("TRANQ");
	}
}

int32 ASpyCharacter::GetCurrentWeaponAmmo() const
{
	switch (CurrentWeapon)
	{
	case EVeilWeapon::GuardPistol: return PistolAmmo;
	case EVeilWeapon::AssaultRifle: return RifleAmmo;
	default: return Ammo;
	}
}

void ASpyCharacter::CycleWeapon(int32 Dir)
{
	const EVeilWeapon Order[3] = { EVeilWeapon::TranqPistol, EVeilWeapon::GuardPistol, EVeilWeapon::AssaultRifle };
	int32 Idx = static_cast<int32>(CurrentWeapon);
	for (int32 i = 0; i < 3; ++i)
	{
		Idx = (Idx + Dir + 3) % 3;
		if (HasWeapon(Order[Idx]))
		{
			CurrentWeapon = Order[Idx];
			ApplyWeaponVisual();
			return;
		}
	}
}

void ASpyCharacter::ApplyWeaponVisual()
{
	if (!PistolSlide)
	{
		return;
	}
	// Rifle = long barrel; pistols = short.
	const bool bRifle = (CurrentWeapon == EVeilWeapon::AssaultRifle);
	PistolSlide->SetRelativeScale3D(bRifle ? FVector(0.45f, 0.045f, 0.05f) : FVector(0.22f, 0.045f, 0.05f));
}

void ASpyCharacter::HandleWeaponNext(const FInputActionValue& Value) { CycleWeapon(1); }
void ASpyCharacter::HandleWeaponPrev(const FInputActionValue& Value) { CycleWeapon(-1); }

void ASpyCharacter::ApplyTranqFire()
{
	if (!Camera)
	{
		return;
	}

	int32* AmmoPool = &Ammo;
	if (CurrentWeapon == EVeilWeapon::GuardPistol) { AmmoPool = &PistolAmmo; }
	else if (CurrentWeapon == EVeilWeapon::AssaultRifle) { AmmoPool = &RifleAmmo; }

	if (*AmmoPool <= 0)
	{
		// Dry fire: tell the HUD so "no more shooting" is never a mystery.
		if (const UWorld* W = GetWorld()) { LastEmptyFireTime = W->GetTimeSeconds(); }
		return;
	}

	--(*AmmoPool);

	const FVector Start = Camera->GetComponentLocation();
	// Aimed shots are true; hip fire scatters. Full-auto rifle sprays wider.
	float SpreadDeg = bIsAiming ? 0.35f : 4.5f;
	if (CurrentWeapon == EVeilWeapon::AssaultRifle) { SpreadDeg += bIsAiming ? 1.4f : 3.0f; }
	const FVector ShotDir = FMath::VRandCone(Camera->GetForwardVector(), FMath::DegreesToRadians(SpreadDeg));
	const FVector End = Start + ShotDir * 3000.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(TranqFire), true, this);
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	UWorld* World = GetWorld();
	const bool bHit = World && World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	const FVector BeamEnd = bHit ? Hit.ImpactPoint : End;

	// Tracer leaves the gun muzzle, not the camera, and reads as gunfire.
	const FVector Muzzle = PistolSlide ? PistolSlide->GetComponentLocation() : Start;
	DrawDebugLine(World, Muzzle, BeamEnd, FColor(255, 214, 130), false, 0.08f, 0, 0.9f);

	// Recoil: a small, quick kick the player feels.
	AddControllerPitchInput(-0.45f);
	AddControllerYawInput(FMath::FRandRange(-0.15f, 0.15f));

	// One-frame muzzle flash.
	if (MuzzleLight)
	{
		MuzzleLight->SetIntensity(6000.f);
		GetWorldTimerManager().SetTimer(MuzzleFlashTimer, [this]()
		{
			if (MuzzleLight) { MuzzleLight->SetIntensity(0.f); }
		}, 0.06f, false);
	}

	// Impact sparks: short radial streaks instead of the old debug sphere.
	if (bHit)
	{
		for (int32 i = 0; i < 5; ++i)
		{
			const FVector Spark = Hit.ImpactPoint +
				(Hit.ImpactNormal + FMath::VRand() * 0.6f).GetSafeNormal() * FMath::FRandRange(8.f, 26.f);
			DrawDebugLine(World, Hit.ImpactPoint, Spark, FColor(255, 190, 90), false, 0.15f, 0, 0.7f);
		}
	}

	if (bHit && Hit.GetActor())
	{
		// Per-bone hits: a dart in the head or neck drops a guard instantly.
		float Damage = 50.f;                                        // tranq (bodyshot ~2)
		if (CurrentWeapon == EVeilWeapon::GuardPistol) { Damage = 95.f; }   // near one-shot, but slow
		else if (CurrentWeapon == EVeilWeapon::AssaultRifle) { Damage = 16.f; } // chip fast
		const FString Bone = Hit.BoneName.ToString().ToLower();
		if (Bone.Contains(TEXT("head")) || Bone.Contains(TEXT("neck")))
		{
			Damage = 100.f;
		}
		UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetController(), this, nullptr);

		if (Cast<AGuardCharacter>(Hit.GetActor()))
		{
			LastHitMarkerTime = World->GetTimeSeconds();
		}
	}

	// Firearms are LOUD; the tranq is a whisper by comparison.
	const bool bLoud = (CurrentWeapon != EVeilWeapon::TranqPistol);
	UAISense_Hearing::ReportNoiseEvent(World, bLoud ? GetActorLocation() : BeamEnd,
		bLoud ? 1.0f : 0.4f, this, bLoud ? 3000.f : 1600.f);
}

void ASpyCharacter::HandleInteract(const FInputActionValue& Value)
{
	if (bDead)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpyInteract), false, this);
	const FVector Origin = GetActorLocation();

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	World->OverlapMultiByObjectType(
		Overlaps, Origin, FQuat::Identity, ObjParams,
		FCollisionShape::MakeSphere(250.f), Params);

	AInteractableActor* Nearest = nullptr;
	float NearestDistSq = TNumericLimits<float>::Max();

	for (const FOverlapResult& Result : Overlaps)
	{
		AInteractableActor* Interactable = Cast<AInteractableActor>(Result.GetActor());
		if (!Interactable || !Interactable->IsActiveObjective())
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(Origin, Interactable->GetActorLocation());
		if (DistSq < NearestDistSq)
		{
			NearestDistSq = DistSq;
			Nearest = Interactable;
		}
	}

	if (Nearest)
	{
		Nearest->Interact(this);
	}
}
