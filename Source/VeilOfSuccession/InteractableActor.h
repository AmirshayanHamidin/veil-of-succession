// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class ASpyCharacter;
class UStaticMeshComponent;
class USphereComponent;
class UMaterialInstanceDynamic;

UENUM()
enum class EInteractableType : uint8
{
	Keycard,
	Intel,
	EavesdropZone,
	Door,
	Marker
};

UCLASS()
class VEILOFSUCCESSION_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Interact") EInteractableType Type = EInteractableType::Marker;
	UPROPERTY(EditAnywhere, Category="Interact") FName ObjectiveId;
	UPROPERTY(EditAnywhere, Category="Interact") FText Prompt;

	// Builder assigns the blocking door mesh actor for Door-type interactables.
	UPROPERTY(EditAnywhere, Category="Interact") TWeakObjectPtr<AActor> LinkedDoor;

	// Re-applies mesh/scale/colour for the CURRENT Type. Needed because the
	// level builder sets Type after SpawnActor (so after BeginPlay) ran.
	void ConfigureVisual();

	void Interact(class ASpyCharacter* Player);   // completes ObjectiveId via subsystem, handles type logic
	bool IsActiveObjective() const;               // true only when ObjectiveId == subsystem GetCurrentObjectiveId()
	float GetEavesdropProgress() const;
	void ForceOpen();   // instantly open a door without player interaction           // 0..1, only meaningful for EavesdropZone

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category="Interact") USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="Interact") UStaticMeshComponent* VisualMesh;
	UPROPERTY(VisibleAnywhere, Category="Interact") USphereComponent* TriggerSphere;
	UPROPERTY() UMaterialInstanceDynamic* MID;

	UPROPERTY() class UStaticMesh* CubeMeshRef;
	UPROPERTY() class UStaticMesh* CylinderMeshRef;
	UPROPERTY() class UStaticMesh* SphereMeshRef;
	UPROPERTY() class UMaterialInterface* BaseMaterial;

private:
	void CompleteSelf();
	void SetGlowColor();

	UPROPERTY() bool bCompleted = false;
	UPROPERTY() bool bPlayerInside = false;
	UPROPERTY() float EavesdropAccum = 0.f;
	UPROPERTY() float BobTime = 0.f;

	// Door slide state
	UPROPERTY() bool bDoorSliding = false;
	UPROPERTY() float DoorSlideTime = 0.f;
	UPROPERTY() FVector DoorStartLoc = FVector::ZeroVector;
	UPROPERTY() FVector DoorEndLoc = FVector::ZeroVector;

	static constexpr float EavesdropRequired = 20.f;
	static constexpr float DoorSlideDuration = 1.f;
	static constexpr float DoorSlideDistance = 400.f;
};
