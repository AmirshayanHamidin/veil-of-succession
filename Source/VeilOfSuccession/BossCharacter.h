// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GuardCharacter.h"
#include "BossCharacter.generated.h"

class ASpyCharacter;

// General Farrokh Azadi (fictional). Dormant boss until Activate() is called.
UCLASS()
class VEILOFSUCCESSION_API ABossCharacter : public AGuardCharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable) float GetBossHealthPct() const;
	UFUNCTION(BlueprintCallable) bool IsActivated() const;

	void Activate();

	// The General cannot be cheesed: dormant = immune (shots taunt him awake
	// once the throne-room act begins), and a knife is a wound, not a kill.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void KnifeKill(bool bFromBehind) override;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual FLinearColor GetBodyColor() const override;

private:
	void UpdatePhase();
	void SummonReinforcements();

	// P1 telegraphed pistol volley.
	UFUNCTION() void StartVolleyTelegraph();
	UFUNCTION() void FirePistolVolley();

	ASpyCharacter* ResolvePlayer() const;

	UPROPERTY() bool bActivated = false;
	UPROPERTY() int32 CurrentPhase = 1;
	UPROPERTY() bool bReinforcementsSummoned = false;

	float BaseWalkSpeed = 300.f;
	float ChargeCooldown = 0.f;

	FTimerHandle VolleyTimer;
	FTimerHandle TelegraphTimer;
};
