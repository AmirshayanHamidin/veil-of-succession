// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS()
class VEILOFSUCCESSION_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGuardCharacter();

	void SetPatrolPoints(const TArray<FVector>& Points);
	const TArray<FVector>& GetPatrolPoints() const;

	UFUNCTION(BlueprintCallable) bool IsDeadOrStunned() const;

	// Knife kill: animated death instead of the tranq ragdoll.
	virtual void KnifeKill(bool bFromBehind);

	// Brief flinch when a dart lands but does not drop the guard.
	void PlayHitReact();

	// Telegraphed melee: windup first, damage lands ~0.45s later, and a
	// well-timed player knife inside the windup = counter.
	void BeginMeleeWindup();
	bool IsWindingUp() const;
	void Stagger(float Seconds);
	bool IsStaggered() const;

	// Visible melee swing; called by the AI just before damage lands.
	void PlayMeleeSwing();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category="Guard") float MaxHealth = 100.f;

protected:
	UPROPERTY() class UAnimSequence* DeathBackAnim;
	UPROPERTY() class UAnimSequence* DeathFrontAnim;
	UPROPERTY() class UAnimSequence* HitReactAnim;
	UPROPERTY() class UAnimSequence* MeleeAnim;
	UPROPERTY() class UAnimSequence* MeleeAnimB;
	UPROPERTY() TSubclassOf<class UAnimInstance> GuardAnimClass;
	FTimerHandle ReactTimer;
	bool bWindingUp = false;
	float WindupUntil = 0.f;
	float StaggerUntil = 0.f;
	void DropWeapon();

protected:
	virtual void BeginPlay() override;

	// Fake-ragdoll death: disable collision/movement/AI, tilt the body over.
	virtual void Die();

	// Overridable so ABossCharacter can use a different body tint.
	virtual FLinearColor GetBodyColor() const;

	void ApplyBlockoutColors();

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* LeftEpaulette;

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* RightEpaulette;

	UPROPERTY()
	UMaterialInterface* BaseMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* BodyMID;

	UPROPERTY()
	UMaterialInstanceDynamic* HeadMID;

	UPROPERTY()
	UMaterialInstanceDynamic* EpauletteMID;

	UPROPERTY() float Health;
	UPROPERTY() bool bDeadOrStunned;

	UPROPERTY() TArray<FVector> PatrolPoints;
};
