// Veil of Succession - fictional political thriller. All characters fictitious.
#include "PalaceLevelBuilder.h"
#include "InteractableActor.h"
#include "MissionSubsystem.h"
#include "GuardCharacter.h"
#include "BossCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Components/LightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyLight.h"
#include "Engine/PostProcessVolume.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/Scene.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

#define LOCTEXT_NAMESPACE "Veil"

APalaceLevelBuilder::APalaceLevelBuilder()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	// Player spawn: safe spot just outside the gate, facing north (+X into the compound).
	PlayerSpawn = FTransform(FRotator(0.f, 0.f, 0.f), FVector(-3800.f, 0.f, 120.f));
}

AStaticMeshActor* APalaceLevelBuilder::SpawnBlock(FVector Center, FVector Extent, FLinearColor Color)
{
	UWorld* World = GetWorld();
	if (!World) { return nullptr; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
		Center, FRotator::ZeroRotator, Params);
	if (!Block) { return nullptr; }

	Block->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* Comp = Block->GetStaticMeshComponent();
	if (Comp && CubeMesh)
	{
		Comp->SetStaticMesh(CubeMesh);
		Comp->SetWorldScale3D(Extent / 50.f); // engine cube is 100uu, so half-extent = Extent
		if (BaseMaterial)
		{
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, Block);
			MID->SetVectorParameterValue(TEXT("Color"), Color);
			Comp->SetMaterial(0, MID);
		}
		Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Comp->SetCollisionProfileName(TEXT("BlockAll"));
	}
	return Block;
}

AStaticMeshActor* APalaceLevelBuilder::SpawnColumn(FVector Center, float Radius, float HalfHeight, FLinearColor Color)
{
	UWorld* World = GetWorld();
	if (!World) { return nullptr; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* Col = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
		Center, FRotator::ZeroRotator, Params);
	if (!Col) { return nullptr; }

	Col->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* Comp = Col->GetStaticMeshComponent();
	if (Comp && CylinderMesh)
	{
		Comp->SetStaticMesh(CylinderMesh);
		// Engine cylinder is 100uu diameter, 100uu tall.
		Comp->SetWorldScale3D(FVector(Radius / 50.f, Radius / 50.f, HalfHeight / 50.f));
		if (BaseMaterial)
		{
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, Col);
			MID->SetVectorParameterValue(TEXT("Color"), Color);
			Comp->SetMaterial(0, MID);
		}
		Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Comp->SetCollisionProfileName(TEXT("BlockAll"));
	}
	return Col;
}

// Which mission zone a guard belongs to, by where it patrols.
static int32 ZoneForX(float X)
{
	if (X < 1900.f) return 0;   // courtyard  = M0
	if (X < 4100.f) return 1;   // hall/chamber = M1
	return 2;                    // throne approach = M2
}

AGuardCharacter* APalaceLevelBuilder::SpawnGuard(const TArray<FVector>& Points)
{
	UWorld* World = GetWorld();
	if (!World || Points.Num() == 0) { return nullptr; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Loc = Points[0];
	Loc.Z = 120.f;
	AGuardCharacter* Guard = World->SpawnActor<AGuardCharacter>(AGuardCharacter::StaticClass(),
		Loc, FRotator::ZeroRotator, Params);
	if (Guard)
	{
		Guard->SpawnDefaultController();
		Guard->SetPatrolPoints(Points);
	}
	return Guard;
}

AInteractableActor* APalaceLevelBuilder::SpawnInteractable(EInteractableType Type, FName ObjectiveId,
	const FVector& Location, const FText& Prompt)
{
	UWorld* World = GetWorld();
	if (!World) { return nullptr; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AInteractableActor* Item = World->SpawnActor<AInteractableActor>(AInteractableActor::StaticClass(),
		Location, FRotator::ZeroRotator, Params);
	if (Item)
	{
		Item->Type = Type;
		Item->ObjectiveId = ObjectiveId;
		Item->Prompt = Prompt;
		Item->ConfigureVisual();   // BeginPlay ran before Type was set
	}
	return Item;
}

FTransform APalaceLevelBuilder::GetPlayerSpawnForMission(int32 MissionIndex) const
{
	switch (MissionIndex)
	{
	case 1:  return FTransform(FRotator(0.f, 0.f, 0.f), FVector(2300.f, 0.f, 150.f));   // inside the palace doors
	case 2:  return FTransform(FRotator(0.f, 0.f, 0.f), FVector(4200.f, 0.f, 150.f));   // approach to the throne room
	default: return GetPlayerSpawn();                                                    // the garden gate
	}
}

void APalaceLevelBuilder::BuildLevel(int32 RespawnMission)
{
	UE_LOG(LogVeil, Log, TEXT("PalaceLevelBuilder::BuildLevel start"));
	BuildGroundAndLights();
	BuildOuterWalls();
	BuildCourtyard();
	BuildPalace();
	SpawnActorsAndObjectives(RespawnMission);
	UE_LOG(LogVeil, Log, TEXT("PalaceLevelBuilder::BuildLevel done"));
}

FTransform APalaceLevelBuilder::GetPlayerSpawn() const
{
	return PlayerSpawn;
}

void APalaceLevelBuilder::BuildGroundAndLights()
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	// Ground: 200x200m sand plane (flat cube), top at Z=0.
	SpawnBlock(FVector(2000.f, 0.f, -10.f), FVector(10000.f, 10000.f, 10.f), FLinearColor(0.80f, 0.73f, 0.55f));

	// Warm directional light.
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// If the map already ships a sun (e.g. the Basic level template), reuse it
	// instead of spawning a second one - two directional lights trip the
	// forward-shading warning that was plastered across the screen.
	bool bHasSun = false;
	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		bHasSun = true;
		if (It->GetLightComponent())
		{
			It->GetLightComponent()->SetIntensity(0.6f);
			It->GetLightComponent()->SetLightColor(FLinearColor(0.55f, 0.68f, 1.0f));
			It->SetActorRotation(FRotator(-50.f, 35.f, 0.f));
		}
		break;
	}

	ADirectionalLight* Sun = bHasSun ? nullptr :
		World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(),
		FVector(0.f, 0.f, 2000.f), FRotator(-50.f, 35.f, 0.f), Params);
	if (Sun && Sun->GetLightComponent())
	{
		Sun->GetLightComponent()->SetMobility(EComponentMobility::Movable);
		// Moonlight: cool, low-intensity key light for the night infiltration.
		Sun->GetLightComponent()->SetIntensity(0.6f);
		Sun->GetLightComponent()->SetLightColor(FLinearColor(0.55f, 0.68f, 1.0f));
		Sun->GetLightComponent()->SetCastShadows(true);
	}

	ASkyLight* Sky = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(),
		FVector(0.f, 0.f, 2000.f), FRotator::ZeroRotator, Params);
	if (Sky && Sky->GetLightComponent())
	{
		Sky->GetLightComponent()->SetMobility(EComponentMobility::Movable);
		// SLS_SpecifiedCubemap fails with no cubemap asset; use a constant
		// ambient colour so the blockout reads at night without a captured sky.
		Sky->GetLightComponent()->SetLightColor(FLinearColor(0.16f, 0.22f, 0.38f));
		Sky->GetLightComponent()->SetIntensity(0.5f);
	}

	// Fixed exposure: auto-exposure blows the blockout materials out to white.
	APostProcessVolume* PPV = World->SpawnActor<APostProcessVolume>(
		APostProcessVolume::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (PPV)
	{
		PPV->bUnbound = true;
		// Lock eye adaptation by pinning the histogram clamps to the same value.
		// (AEM_Manual ignores these clamps and reads camera ISO/shutter instead,
		// which is why the scene blew out to white.)
		PPV->Settings.bOverride_AutoExposureMethod = true;
		PPV->Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Histogram;
		PPV->Settings.bOverride_AutoExposureMinBrightness = true;
		PPV->Settings.AutoExposureMinBrightness = 1.0f;
		PPV->Settings.bOverride_AutoExposureMaxBrightness = true;
		PPV->Settings.AutoExposureMaxBrightness = 1.0f;
		PPV->Settings.bOverride_AutoExposureBias = true;
		PPV->Settings.AutoExposureBias = 0.0f;
		PPV->Settings.bOverride_BloomIntensity = true;
		PPV->Settings.BloomIntensity = 0.35f;
	}

	AExponentialHeightFog* Fog = World->SpawnActor<AExponentialHeightFog>(AExponentialHeightFog::StaticClass(),
		FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Params);
	if (Fog && Fog->GetComponent())
	{
		Fog->GetComponent()->SetFogDensity(0.004f);
		Fog->GetComponent()->SetFogInscatteringColor(FLinearColor(0.05f, 0.07f, 0.14f));
	}
}

void APalaceLevelBuilder::BuildOuterWalls()
{
	// Compound: X [-3000..7000], Y [-4000..4000]. Walls 500 tall, 60 thick.
	const float WallHalfH = 250.f;   // 500 tall
	const float WallHalfT = 30.f;    // 60 thick
	const float WallZ = 250.f;       // center so base at 0

	// North wall (X=7000) full width.
	SpawnBlock(FVector(7000.f, 0.f, WallZ), FVector(WallHalfT, 4000.f, WallHalfH), Sandstone);
	// East wall (Y=4000).
	SpawnBlock(FVector(2000.f, 4000.f, WallZ), FVector(5000.f, WallHalfT, WallHalfH), Sandstone);
	// West wall (Y=-4000).
	SpawnBlock(FVector(2000.f, -4000.f, WallZ), FVector(5000.f, WallHalfT, WallHalfH), Sandstone);

	// South wall (X=-3000) with a gate gap at Y in [-350,350].
	SpawnBlock(FVector(-3000.f, 2175.f, WallZ), FVector(WallHalfT, 1825.f, WallHalfH), Sandstone);
	SpawnBlock(FVector(-3000.f, -2175.f, WallZ), FVector(WallHalfT, 1825.f, WallHalfH), Sandstone);
	// Gate posts.
	SpawnColumn(FVector(-3000.f, 350.f, 300.f), 60.f, 300.f, Sandstone);
	SpawnColumn(FVector(-3000.f, -350.f, 300.f), 60.f, 300.f, Sandstone);
}

void APalaceLevelBuilder::BuildCourtyard()
{
	// Rose garden courtyard: X [-3000..2000]. Scatter cover blocks + hedges,
	// kept clear of the straight patrol lanes at Y = +/-2800, +/-1500, 0.
	const float HedgeZ = 100.f;

	// Hedge rows (long, thin) along X, placed BETWEEN patrol lanes.
	SpawnBlock(FVector(-1000.f, 750.f, HedgeZ), FVector(1400.f, 60.f, 100.f), HedgeGreen);
	SpawnBlock(FVector(-1000.f, -750.f, HedgeZ), FVector(1400.f, 60.f, 100.f), HedgeGreen);
	SpawnBlock(FVector(-1000.f, 2150.f, HedgeZ), FVector(1400.f, 60.f, 100.f), HedgeGreen);
	SpawnBlock(FVector(-1000.f, -2150.f, HedgeZ), FVector(1400.f, 60.f, 100.f), HedgeGreen);

	// Scattered cover blocks (crimson planters / low walls), off the lanes.
	SpawnBlock(FVector(-2000.f, 2150.f, 90.f), FVector(120.f, 120.f, 90.f), Crimson);
	SpawnBlock(FVector(500.f, 2150.f, 90.f), FVector(120.f, 120.f, 90.f), Crimson);
	SpawnBlock(FVector(-2000.f, -2150.f, 90.f), FVector(120.f, 120.f, 90.f), Crimson);
	SpawnBlock(FVector(500.f, -2150.f, 90.f), FVector(120.f, 120.f, 90.f), Crimson);
	SpawnBlock(FVector(-1500.f, 0.f, 90.f), FVector(150.f, 150.f, 90.f), Sandstone);

	// Keycard table along the Y=-1500 patrol lane.
	SpawnBlock(FVector(200.f, -1500.f, 45.f), FVector(120.f, 120.f, 45.f), FLinearColor(0.5f, 0.35f, 0.2f));
}

void APalaceLevelBuilder::BuildPalace()
{
	const float WHalfH = 250.f;  // 500 tall
	const float WHalfT = 30.f;   // 60 thick
	const float WZ = 250.f;

	// Palace footprint: X [2000..7000], Y [-2000..2000].
	// --- South palace facade (X=2000) with entrance doorway gap at Y in [-300,300].
	SpawnBlock(FVector(2000.f, 1150.f, WZ), FVector(WHalfT, 850.f, WHalfH), Sandstone);
	SpawnBlock(FVector(2000.f, -1150.f, WZ), FVector(WHalfT, 850.f, WHalfH), Sandstone);
	// Lintel above the doorway.
	SpawnBlock(FVector(2000.f, 0.f, 420.f), FVector(WHalfT, 300.f, 80.f), Sandstone);

	// East + west palace walls.
	SpawnBlock(FVector(4500.f, 2000.f, WZ), FVector(2500.f, WHalfT, WHalfH), Sandstone);
	SpawnBlock(FVector(4500.f, -2000.f, WZ), FVector(2500.f, WHalfT, WHalfH), Sandstone);

	// Grand hall: X [2000..4000]. Two column rows at Y = +/-800.
	for (int32 i = 0; i < 4; ++i)
	{
		const float X = 2400.f + i * 450.f;
		SpawnColumn(FVector(X, 800.f, 250.f), 70.f, 250.f, Sandstone);
		SpawnColumn(FVector(X, -800.f, 250.f), 70.f, 250.f, Sandstone);
	}
	// Crimson carpet strip down the central aisle.
	SpawnBlock(FVector(3000.f, 0.f, 6.f), FVector(1000.f, 250.f, 6.f), Crimson);

	// Partition wall at X=4000 dividing hall from the rear rooms.
	// Doorway to corridor at Y=1500, doorway to meeting chamber at Y=-500.
	SpawnBlock(FVector(4000.f, 1750.f, WZ), FVector(WHalfT, 250.f, WHalfH), Sandstone); // north segment
	SpawnBlock(FVector(4000.f, 550.f, WZ), FVector(WHalfT, 700.f, WHalfH), Sandstone);  // between the two doorways
	SpawnBlock(FVector(4000.f, -1250.f, WZ), FVector(WHalfT, 450.f, WHalfH), Sandstone); // south segment

	// Corridor (east side): Y [1200..2000], X [4000..6000]. Inner wall at Y=1200.
	SpawnBlock(FVector(5000.f, 1200.f, WZ), FVector(1000.f, WHalfT, WHalfH), Sandstone);

	// Meeting chamber (west portion): X [4000..6000], Y [-2000..1200].
	// Lattice screen the eavesdrop zone hides behind (thin, tall, with gaps implied).
	SpawnBlock(FVector(4900.f, -600.f, 200.f), FVector(30.f, 500.f, 200.f), FLinearColor(0.4f, 0.3f, 0.2f));
	// Meeting table + chairs (blockout).
	SpawnBlock(FVector(5200.f, -800.f, 45.f), FVector(400.f, 200.f, 45.f), FLinearColor(0.35f, 0.22f, 0.12f));
	// Intel table.
	SpawnBlock(FVector(4600.f, -1400.f, 45.f), FVector(120.f, 120.f, 45.f), FLinearColor(0.5f, 0.35f, 0.2f));

	// Rear partition at X=6000 dividing rear rooms from throne room.
	// Doorway from corridor at Y=1500, doorway from chamber at Y=0.
	SpawnBlock(FVector(6000.f, 1750.f, WZ), FVector(WHalfT, 250.f, WHalfH), Sandstone);
	SpawnBlock(FVector(6000.f, 750.f, WZ), FVector(WHalfT, 450.f, WHalfH), Sandstone);
	SpawnBlock(FVector(6000.f, -1150.f, WZ), FVector(WHalfT, 850.f, WHalfH), Sandstone);

	// Throne room: X [6000..7000]. Raised gold dais at the far end.
	SpawnBlock(FVector(6600.f, 0.f, 60.f), FVector(400.f, 500.f, 60.f), Gold);       // dais
	SpawnBlock(FVector(6800.f, 0.f, 260.f), FVector(80.f, 200.f, 140.f), Gold);       // throne back
	SpawnBlock(FVector(6300.f, 0.f, 6.f), FVector(300.f, 200.f, 6.f), Crimson);       // approach carpet

	// --- Palace entrance blocking door cube (slides aside on interact). Fills the gap.
	AStaticMeshActor* DoorCube = SpawnBlock(FVector(2000.f, 0.f, 200.f), FVector(35.f, 300.f, 200.f),
		FLinearColor(0.45f, 0.30f, 0.15f));
	// Stash for SpawnActorsAndObjectives to link.
	PendingDoorCube = DoorCube;
}

void APalaceLevelBuilder::SpawnActorsAndObjectives(int32 RespawnMission)
{
	// ---------- Objective interactables ----------
	SpawnInteractable(EInteractableType::Marker, TEXT("Obj_EnterGrounds"),
		FVector(-2500.f, 0.f, 60.f), LOCTEXT("P_Enter", "Enter the grounds"));

	SpawnInteractable(EInteractableType::Keycard, TEXT("Obj_Keycard"),
		FVector(200.f, -1500.f, 130.f), LOCTEXT("P_Keycard", "Take the keycard"));

	AInteractableActor* Door = SpawnInteractable(EInteractableType::Door, TEXT("Obj_EnterPalace"),
		FVector(2000.f, 0.f, 120.f), LOCTEXT("P_Door", "Open the palace door"));
	if (Door && PendingDoorCube)
	{
		Door->LinkedDoor = PendingDoorCube;
	}

	SpawnInteractable(EInteractableType::EavesdropZone, TEXT("Obj_Eavesdrop"),
		FVector(5200.f, -800.f, 60.f), LOCTEXT("P_Eaves", "Listen in"));

	SpawnInteractable(EInteractableType::Intel, TEXT("Obj_StealDossier"),
		FVector(4600.f, -1400.f, 130.f), LOCTEXT("P_Intel", "Take the dossier"));

	SpawnInteractable(EInteractableType::Marker, TEXT("Obj_ReachThroneRoom"),
		FVector(6300.f, 0.f, 60.f), LOCTEXT("P_Throne", "Reach the throne room"));

	SpawnInteractable(EInteractableType::Marker, TEXT("Obj_Escape"),
		FVector(-2800.f, 0.f, 60.f), LOCTEXT("P_Escape", "Escape to extraction"));

	// ---------- Guards: 6 courtyard, 2 hall, 2 corridor (straight legs) ----------
	// Courtyard lanes.
	SpawnGuard({ FVector(-2600.f, -1500.f, 0.f), FVector(1500.f, -1500.f, 0.f) });
	SpawnGuard({ FVector(-2600.f, 1500.f, 0.f), FVector(1500.f, 1500.f, 0.f) });
	SpawnGuard({ FVector(-2000.f, 0.f, 0.f), FVector(1800.f, 0.f, 0.f) });
	SpawnGuard({ FVector(-2600.f, -2800.f, 0.f), FVector(1500.f, -2800.f, 0.f) });
	SpawnGuard({ FVector(-2600.f, 2800.f, 0.f), FVector(1500.f, 2800.f, 0.f) });
	SpawnGuard({ FVector(1000.f, -2800.f, 0.f), FVector(1000.f, 2800.f, 0.f) });
	// Grand hall (central aisle, Y=0).
	SpawnGuard({ FVector(2300.f, 0.f, 0.f), FVector(3800.f, 0.f, 0.f) });
	SpawnGuard({ FVector(3800.f, 0.f, 0.f), FVector(2300.f, 0.f, 0.f) });
	// Corridor (east, Y=1500).
	SpawnGuard({ FVector(4200.f, 1500.f, 0.f), FVector(5800.f, 1500.f, 0.f) });
	SpawnGuard({ FVector(5800.f, 1500.f, 0.f), FVector(4200.f, 1500.f, 0.f) });

	// ---------- Respawn cleanup: don't make the player redo cleared ground ----------
	// Guards whose zone is BELOW the respawn mission are already dead - they lie
	// on the floor as ragdolls, exactly where the player left them.
	if (RespawnMission > 0)
	{
		for (TActorIterator<AGuardCharacter> It(GetWorld()); It; ++It)
		{
			AGuardCharacter* G = *It;
			if (G && !G->GetPatrolPoints().IsEmpty()
				&& ZoneForX(G->GetPatrolPoints()[0].X) < RespawnMission)
			{
				G->KnifeKill(false);   // animated corpse, no drop spam handled inside
			}
		}
		// Every door for prior objectives is already open - never respawn behind
		// a locked door you already unlocked.
		for (TActorIterator<AInteractableActor> It(GetWorld()); It; ++It)
		{
			AInteractableActor* A = *It;
			if (A && A->Type == EInteractableType::Door)
			{
				A->ForceOpen();
			}
		}
	}

	// ---------- Boss (dormant) on the dais ----------
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Boss = World->SpawnActor<ABossCharacter>(ABossCharacter::StaticClass(),
			FVector(6600.f, 0.f, 240.f), FRotator(0.f, 180.f, 0.f), Params);
		if (Boss)
		{
			Boss->SpawnDefaultController();
		}
	}
}

void APalaceLevelBuilder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Activate the boss once the campaign reaches mission index 2.
	if (Boss && !Boss->IsActivated())
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMissionSubsystem* Mission = GI->GetSubsystem<UMissionSubsystem>())
			{
				if (Mission->GetCurrentMissionIndex() == 2)
				{
					Boss->Activate();
					UE_LOG(LogVeil, Log, TEXT("Boss activated (mission index 2)."));
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
