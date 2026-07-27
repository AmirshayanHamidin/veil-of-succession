// Veil of Succession - fictional political thriller. All characters fictitious.
#include "StealthComponent.h"
#include "GameFramework/Character.h"

UStealthComponent::UStealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UStealthComponent::GetVisibilityMultiplier() const
{
	const ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar)
	{
		return 1.0f;
	}

	// Crouching is the quietest/least visible posture.
	if (OwnerChar->bIsCrouched)
	{
		return 0.45f;
	}

	// Classify by planar ground speed: fast movement counts as sprinting.
	const float Speed = OwnerChar->GetVelocity().Size2D();
	if (Speed > SprintSpeedThreshold)
	{
		return 1.6f;
	}

	return 1.0f;
}
