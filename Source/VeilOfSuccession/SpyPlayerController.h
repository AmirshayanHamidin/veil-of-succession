// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASpyCharacter;

UCLASS()
class VEILOFSUCCESSION_API ASpyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpyPlayerController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	UFUNCTION() void HandleRestart(const struct FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

private:
	void BuildInputActions();
	void RegisterMappingContext();

	UPROPERTY() UInputMappingContext* MappingContext;

	UPROPERTY() UInputAction* IA_Move;
	UPROPERTY() UInputAction* IA_Look;
	UPROPERTY() UInputAction* IA_Sprint;
	UPROPERTY() UInputAction* IA_Crouch;
	UPROPERTY() UInputAction* IA_Aim;
	UPROPERTY() UInputAction* IA_Fire;
	UPROPERTY() UInputAction* IA_Interact;
	UPROPERTY() UInputAction* IA_Restart;
	UPROPERTY() UInputAction* IA_Takedown;
	UPROPERTY() UInputAction* IA_Shield;
	UPROPERTY() UInputAction* IA_Jump;
	UPROPERTY() UInputAction* IA_Knife;
	UPROPERTY() UInputAction* IA_WeaponNext;
	UPROPERTY() UInputAction* IA_WeaponPrev;
	UPROPERTY() UInputAction* IA_Dodge;
	UPROPERTY() UInputAction* IA_LockOn;

	UPROPERTY() bool bActionsBuilt;
};
