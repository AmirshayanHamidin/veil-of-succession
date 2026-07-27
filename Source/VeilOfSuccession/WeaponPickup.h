// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"

UENUM()
enum class EVeilWeapon : uint8
{
	TranqPistol,
	GuardPistol,
	AssaultRifle
};

// A weapon dropped by a dead guard. Auto-collects on touch.
UCLASS()
class VEILOFSUCCESSION_API AWeaponPickup : public AActor
{
	GENERATED_BODY()

public:
	AWeaponPickup();

	UPROPERTY(EditAnywhere, Category="Pickup") EVeilWeapon WeaponType = EVeilWeapon::GuardPistol;
	UPROPERTY(EditAnywhere, Category="Pickup") int32 AmmoAmount = 12;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep);

	UPROPERTY(VisibleAnywhere) USceneComponent* Root;
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* GunMesh;
	UPROPERTY(VisibleAnywhere) class USphereComponent* Trigger;
	UPROPERTY() class UMaterialInstanceDynamic* MID;
	float BobTime = 0.f;
};
