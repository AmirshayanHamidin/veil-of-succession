// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponPickup.h"
#include "InputActionValue.h"
#include "SpyCharacter.generated.h"

class UStealthComponent;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USkeletalMesh;
class UAnimSequence;
class UAnimInstance;
class UMaterialInstanceDynamic;

UCLASS()
class VEILOFSUCCESSION_API ASpyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpyCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable) float GetHealth() const;
	UFUNCTION(BlueprintCallable) float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable) bool IsDead() const;
	UFUNCTION(BlueprintCallable) float GetNoiseLevel() const;      // 0 crouch-still .. 1 sprint
	UFUNCTION(BlueprintCallable) float GetDetection() const;       // 0..1 global exposure
	UFUNCTION(BlueprintCallable) int32 GetAmmo() const;
	UFUNCTION(BlueprintCallable) bool IsAiming() const;
	UFUNCTION(BlueprintCallable) bool IsSprinting() const;
	UFUNCTION(BlueprintCallable) bool IsCrouching() const;

	// Ballistic shield
	UFUNCTION(BlueprintCallable) float GetShield() const;
	UFUNCTION(BlueprintCallable) float GetMaxShield() const;
	UFUNCTION(BlueprintCallable) bool IsShieldActive() const;

	// Silent takedown: returns the guard currently takedown-able, or null.
	class AGuardCharacter* FindTakedownTarget() const;

	// Seconds since a dart last connected; the HUD draws a hit marker under 0.25.
	UFUNCTION(BlueprintCallable) float GetTimeSinceHitMarker() const;
	UFUNCTION(BlueprintCallable) float GetTimeSinceEmptyFire() const;
	UFUNCTION(BlueprintCallable) float GetTimeSinceDamaged() const;
	UFUNCTION(BlueprintCallable) FVector GetLastDamageDirection() const;

	// ---- Inventory ----
	void GiveWeapon(EVeilWeapon Weapon, int32 AmmoAmount);
	UFUNCTION(BlueprintCallable) EVeilWeapon GetCurrentWeapon() const;
	UFUNCTION(BlueprintCallable) FString GetWeaponName() const;
	UFUNCTION(BlueprintCallable) int32 GetCurrentWeaponAmmo() const;
	UFUNCTION(BlueprintCallable) bool HasWeapon(EVeilWeapon Weapon) const;
	UFUNCTION() void HandleWeaponNext(const FInputActionValue& Value);
	UFUNCTION() void HandleWeaponPrev(const FInputActionValue& Value);

	void AddDetection(float Amount);                               // clamped 0..1, guards call this
	UStealthComponent* GetStealthComponent() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	// Input handlers bound dynamically by ASpyPlayerController.
	UFUNCTION() void HandleMove(const FInputActionValue& Value);
	UFUNCTION() void HandleLook(const FInputActionValue& Value);
	UFUNCTION() void HandleSprintStart(const FInputActionValue& Value);
	UFUNCTION() void HandleSprintStop(const FInputActionValue& Value);
	UFUNCTION() void HandleCrouchToggle(const FInputActionValue& Value);
	UFUNCTION() void HandleAimStart(const FInputActionValue& Value);
	UFUNCTION() void HandleAimStop(const FInputActionValue& Value);
	UFUNCTION() void HandleFire(const FInputActionValue& Value);
	UFUNCTION() void HandleFireStop(const FInputActionValue& Value);
	UFUNCTION() void HandleInteract(const FInputActionValue& Value);
	UFUNCTION() void HandleTakedown(const FInputActionValue& Value);
	UFUNCTION() void HandleKnife(const FInputActionValue& Value);
	UFUNCTION() void HandleDodge(const FInputActionValue& Value);
	UFUNCTION() void HandleLockOn(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable) class AGuardCharacter* GetLockTarget() const;
	UFUNCTION(BlueprintCallable) float GetTimeSinceCounter() const;
	UFUNCTION() void HandleJumpStart(const FInputActionValue& Value);
	UFUNCTION() void HandleJumpStop(const FInputActionValue& Value);
	UFUNCTION() void HandleShieldStart(const FInputActionValue& Value);
	UFUNCTION() void HandleShieldStop(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, Category="Blockout")
	UStaticMeshComponent* HeadMesh;

	// Tranquilliser pistol, socketed to the mannequin's right hand.
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UStaticMeshComponent* PistolSlide;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UStaticMeshComponent* PistolGrip;

	// Deployable ballistic shield, hidden until raised.
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere, Category="Stealth")
	UStealthComponent* StealthComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* BodyMID;

	UPROPERTY()
	UMaterialInstanceDynamic* HeadMID;

private:
	void ApplyTranqFire();
	void UpdateNoiseLevel();

	UPROPERTY() float Health;
	UPROPERTY() float MaxHealth;
	UPROPERTY() float NoiseLevel;
	UPROPERTY() float Detection;
	UPROPERTY() int32 Ammo;
	UPROPERTY() bool bIsAiming;
	UPROPERTY() bool bIsSprinting;
	UPROPERTY() bool bDead;
	UPROPERTY() float Shield;
	UPROPERTY() float MaxShield;
	UPROPERTY() bool bShieldActive;
	UPROPERTY() UAnimSequence* AimPose;
	UPROPERTY() TSubclassOf<UAnimInstance> DefaultAnimClass;
	float LastHitMarkerTime = -100.f;
	float LastEmptyFireTime = -100.f;
	float LastDamagedTime = -100.f;
	float LastFireTime = -100.f;
	bool bFireHeld = false;
	FVector LastDamageDir = FVector::ForwardVector;
	float DartRegenTimer = 0.f;
	// Inventory: tranq is always owned; the rest come off dead guards.
	EVeilWeapon CurrentWeapon = EVeilWeapon::TranqPistol;
	bool bHasGuardPistol = false;
	bool bHasRifle = false;
	int32 PistolAmmo = 0;
	int32 RifleAmmo = 0;
	void CycleWeapon(int32 Dir);
	void ApplyWeaponVisual();
	UPROPERTY() class UPointLightComponent* MuzzleLight;
	FTimerHandle MuzzleFlashTimer;
	FTimerHandle FireAnimTimer;
	UPROPERTY() UAnimSequence* KnifeAnimA;
	UPROPERTY() UAnimSequence* KnifeAnimB;
	UPROPERTY() UAnimSequence* KnifeAnimC;
	UPROPERTY() UAnimSequence* DashAnim;
	UPROPERTY() UAnimSequence* StealthKillAnim;
	int32 ComboIndex = 0;
	float LastKnifeTime = -100.f;
	bool bDodging = false;
	float LastDodgeTime = -100.f;
	FTimerHandle DodgeTimer;
	TWeakObjectPtr<class AGuardCharacter> LockTarget;
	float LastCounterTime = -100.f;
	FTimerHandle KnifeTimer;
	bool bKnifing = false;
	void PlayFullBodyAnim(UAnimSequence* Anim, float RestoreAfter);

	// Movement tuning
	float WalkSpeed;
	float SprintSpeed;
	float CrouchSpeed;

	// Camera arm targets
	float DefaultArmLength;
	float AimArmLength;
};
