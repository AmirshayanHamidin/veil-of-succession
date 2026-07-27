// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PalaceLevelBuilder.generated.h"

class AStaticMeshActor;
class AGuardCharacter;
class ABossCharacter;
class AInteractableActor;
class UStaticMesh;
class UMaterialInterface;
enum class EInteractableType : uint8;

UCLASS()
class VEILOFSUCCESSION_API APalaceLevelBuilder : public AActor
{
	GENERATED_BODY()

public:
	APalaceLevelBuilder();

	virtual void Tick(float DeltaSeconds) override;

	void BuildLevel(int32 RespawnMission = 0);
	FTransform GetPlayerSpawn() const;
	FTransform GetPlayerSpawnForMission(int32 MissionIndex) const;

private:
	// Core helper: engine Cube scaled to full box; Extent is half-extents.
	AStaticMeshActor* SpawnBlock(FVector Center, FVector Extent, FLinearColor Color);
	// Cylinder column: Radius (uu) + half-height.
	AStaticMeshActor* SpawnColumn(FVector Center, float Radius, float HalfHeight, FLinearColor Color);

	AGuardCharacter* SpawnGuard(const TArray<FVector>& Points);
	AInteractableActor* SpawnInteractable(EInteractableType Type, FName ObjectiveId,
		const FVector& Location, const FText& Prompt);

	void BuildGroundAndLights();
	void BuildOuterWalls();
	void BuildCourtyard();
	void BuildPalace();
	void SpawnActorsAndObjectives(int32 RespawnMission = 0);

	UPROPERTY() UStaticMesh* CubeMesh;
	UPROPERTY() UStaticMesh* CylinderMesh;
	UPROPERTY() UStaticMesh* PlaneMesh;
	UPROPERTY() UMaterialInterface* BaseMaterial;

	UPROPERTY() ABossCharacter* Boss;
	UPROPERTY() AStaticMeshActor* PendingDoorCube;

	FTransform PlayerSpawn;

	// Palette
	const FLinearColor Sandstone = FLinearColor(0.76f, 0.70f, 0.55f);
	const FLinearColor HedgeGreen = FLinearColor(0.13f, 0.35f, 0.16f);
	const FLinearColor Crimson = FLinearColor(0.55f, 0.06f, 0.08f);
	const FLinearColor Gold = FLinearColor(0.85f, 0.65f, 0.13f);
};
