// Veil of Succession - fictional political thriller. All characters fictitious.
#include "InteractableActor.h"
#include "MissionSubsystem.h"
#include "SpyCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(Root);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	CylinderMeshRef = CylFinder.Succeeded() ? CylFinder.Object : nullptr;
	if (SphereFinder.Succeeded())
	{
		VisualMesh->SetStaticMesh(SphereFinder.Object);
	}
	// Cube mesh kept for zone/marker visuals via SetStaticMesh in BeginPlay.
	CubeMeshRef = CubeFinder.Succeeded() ? CubeFinder.Object : nullptr;
	SphereMeshRef = SphereFinder.Succeeded() ? SphereFinder.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseMaterial = MatFinder.Object;
	}

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(Root);
	TriggerSphere->SetSphereRadius(200.f);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractableActor::ConfigureVisual()
{
	// Pickups = small bobbing sphere; walk-through objectives = modest floor disc.
	const bool bPickup = (Type == EInteractableType::Keycard || Type == EInteractableType::Intel);
	if (bPickup)
	{
		if (SphereMeshRef) { VisualMesh->SetStaticMesh(SphereMeshRef); }
		VisualMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));
		VisualMesh->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	}
	else
	{
		if (CylinderMeshRef) { VisualMesh->SetStaticMesh(CylinderMeshRef); }
		VisualMesh->SetRelativeScale3D(FVector(1.3f, 1.3f, 0.04f));
		VisualMesh->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	}

	SetGlowColor();
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	ConfigureVisual();

	// Trigger radius: larger for zones/markers so overlap auto-fires; smaller for pickups/doors.
	if (Type == EInteractableType::EavesdropZone)
	{
		TriggerSphere->SetSphereRadius(300.f);
	}
	else if (Type == EInteractableType::Marker)
	{
		TriggerSphere->SetSphereRadius(200.f);
	}
	else
	{
		TriggerSphere->SetSphereRadius(220.f);
	}

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);
}

void AInteractableActor::SetGlowColor()
{
	if (!BaseMaterial)
	{
		return;
	}
	if (!MID)
	{
		MID = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		VisualMesh->SetMaterial(0, MID);
	}

	// BasicShapeMaterial has no emissive parameter, so use a bright color to fake a glow.
	FLinearColor C = FLinearColor(0.2f, 0.8f, 1.0f);
	switch (Type)
	{
	case EInteractableType::Keycard:      C = FLinearColor(0.1f, 1.0f, 0.9f); break;   // cyan
	case EInteractableType::Intel:        C = FLinearColor(1.0f, 0.85f, 0.1f); break;  // gold
	case EInteractableType::EavesdropZone:C = FLinearColor(0.6f, 0.3f, 1.0f); break;   // violet
	case EInteractableType::Door:         C = FLinearColor(0.9f, 0.4f, 0.1f); break;   // amber
	case EInteractableType::Marker:       C = FLinearColor(0.2f, 0.9f, 0.4f); break;   // green
	default: break;
	}
	MID->SetVectorParameterValue(TEXT("Color"), C);
}

void AInteractableActor::ForceOpen()
{
	if (Type != EInteractableType::Door)
	{
		return;
	}
	bCompleted = true;
	if (AActor* Door = LinkedDoor.Get())
	{
		Door->SetActorLocation(Door->GetActorLocation()
			+ Door->GetActorRightVector() * DoorSlideDistance);
	}
	VisualMesh->SetVisibility(false);
}

void AInteractableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Door slide interpolation (no timeline).
	if (bDoorSliding)
	{
		DoorSlideTime += DeltaSeconds;
		const float Alpha = FMath::Clamp(DoorSlideTime / DoorSlideDuration, 0.f, 1.f);
		if (AActor* Door = LinkedDoor.Get())
		{
			Door->SetActorLocation(FMath::Lerp(DoorStartLoc, DoorEndLoc, Alpha));
		}
		if (Alpha >= 1.f)
		{
			bDoorSliding = false;
		}
	}

	if (bCompleted)
	{
		return;
	}

	// Only the CURRENT objective is shown; completed/future ones stay hidden,
	// so collected checkpoints visibly disappear and there is one clear goal.
	const bool bShouldShow = IsActiveObjective();
	if (VisualMesh->IsVisible() != bShouldShow)
	{
		VisualMesh->SetVisibility(bShouldShow);
	}

	// Pickups: gentle bob, no spinning.
	const bool bPickup = (Type == EInteractableType::Keycard || Type == EInteractableType::Intel);
	if (bPickup && bShouldShow)
	{
		BobTime += DeltaSeconds;
		const float BobZ = FMath::Sin(BobTime * 2.f) * 6.f;
		VisualMesh->SetRelativeLocation(FVector(0.f, 0.f, 40.f + BobZ));
	}

	// Eavesdrop accumulation while the player stands inside and it is the active objective.
	if (Type == EInteractableType::EavesdropZone && bPlayerInside && IsActiveObjective())
	{
		EavesdropAccum += DeltaSeconds;
		if (EavesdropAccum >= EavesdropRequired)
		{
			CompleteSelf();
		}
	}
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	ASpyCharacter* Player = Cast<ASpyCharacter>(OtherActor);
	if (!Player)
	{
		return;
	}
	bPlayerInside = true;

	if (bCompleted || !IsActiveObjective())
	{
		return;
	}

	// Markers AND pickups auto-complete on touch - standing on the keycard and
	// wondering why nothing happens was the worst moment in the whole game.
	// Doors still need a deliberate press; eavesdrop accumulates in Tick.
	if (Type == EInteractableType::Marker
		|| Type == EInteractableType::Keycard
		|| Type == EInteractableType::Intel)
	{
		CompleteSelf();
	}
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ASpyCharacter>(OtherActor))
	{
		bPlayerInside = false;
	}
}

void AInteractableActor::Interact(ASpyCharacter* Player)
{
	if (bCompleted || !IsActiveObjective())
	{
		return;
	}

	switch (Type)
	{
	case EInteractableType::Door:
	{
		// Slide the linked blocking cube 400uu sideways over 1s.
		if (AActor* Door = LinkedDoor.Get())
		{
			DoorStartLoc = Door->GetActorLocation();
			DoorEndLoc = DoorStartLoc + Door->GetActorRightVector() * DoorSlideDistance;
			DoorSlideTime = 0.f;
			bDoorSliding = true;
		}
		CompleteSelf();
		break;
	}
	case EInteractableType::Keycard:
	case EInteractableType::Intel:
		// Pickups require the Interact press; complete + hide.
		CompleteSelf();
		break;
	case EInteractableType::Marker:
	case EInteractableType::EavesdropZone:
		// These auto-trigger; pressing interact inside a marker still completes it.
		CompleteSelf();
		break;
	default:
		break;
	}
}

void AInteractableActor::CompleteSelf()
{
	if (bCompleted)
	{
		return;
	}
	bCompleted = true;

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UMissionSubsystem* Mission = GI->GetSubsystem<UMissionSubsystem>())
		{
			Mission->CompleteObjective(ObjectiveId);
		}
	}

	UE_LOG(LogVeil, Log, TEXT("Interactable %s completed objective %s"),
		*GetName(), *ObjectiveId.ToString());

	// Hide self (visual + trigger). Doors keep their linked cube; the slide already moved it.
	SetActorHiddenInGame(true);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetVisibility(false);
}

bool AInteractableActor::IsActiveObjective() const
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		if (const UMissionSubsystem* Mission = GI->GetSubsystem<UMissionSubsystem>())
		{
			return Mission->GetCurrentObjectiveId() == ObjectiveId && !ObjectiveId.IsNone();
		}
	}
	return false;
}

float AInteractableActor::GetEavesdropProgress() const
{
	if (Type != EInteractableType::EavesdropZone)
	{
		return 0.f;
	}
	return FMath::Clamp(EavesdropAccum / EavesdropRequired, 0.f, 1.f);
}
