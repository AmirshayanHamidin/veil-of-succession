// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VeilHUD.generated.h"

class ASpyCharacter;
class ABossCharacter;
class UMissionSubsystem;

UCLASS()
class VEILOFSUCCESSION_API AVeilHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	int32 LastMissionIndex = -1;
	float MissionSplashTime = -100.f;
	FText MissionSplashText;
	int32 LastBossPhase = 0;
	float PhaseSplashTime = -100.f;
	void DrawBar(float X, float Y, float W, float H, float Fraction,
		const FLinearColor& FillColor, const FLinearColor& BgColor);
	void DrawLabel(const FString& Text, float X, float Y, const FLinearColor& Color, bool bLarge);
	void DrawCenteredLabel(const FString& Text, float CenterX, float Y, const FLinearColor& Color, bool bLarge);

	class AInteractableActor* FindActiveEavesdrop() const;

	TWeakObjectPtr<ABossCharacter> CachedBoss;
};
