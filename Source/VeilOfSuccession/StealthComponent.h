// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEILOFSUCCESSION_API UStealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStealthComponent();

	// crouched .45, sprinting 1.6, else 1.0 (reads owner ACharacter state)
	UFUNCTION(BlueprintCallable, Category="Stealth")
	float GetVisibilityMultiplier() const;

	// Speed above which the owner is considered sprinting/loud.
	UPROPERTY(EditAnywhere, Category="Stealth")
	float SprintSpeedThreshold = 500.f;
};
