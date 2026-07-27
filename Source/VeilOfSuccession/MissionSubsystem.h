// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionSubsystem.generated.h"

USTRUCT()
struct FVeilObjective
{
	GENERATED_BODY()

	UPROPERTY() FName Id;
	UPROPERTY() int32 MissionIndex = 0;
	UPROPERTY() FText MissionTitle;
	UPROPERTY() FText Text;

	FVeilObjective() {}
	FVeilObjective(FName InId, int32 InMissionIndex, const FText& InTitle, const FText& InText)
		: Id(InId), MissionIndex(InMissionIndex), MissionTitle(InTitle), Text(InText) {}
};

UCLASS()
class VEILOFSUCCESSION_API UMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable) void CompleteObjective(FName ObjectiveId);
	UFUNCTION(BlueprintCallable) FText GetCurrentObjectiveText() const;
	UFUNCTION(BlueprintCallable) int32 GetCurrentMissionIndex() const;   // 0..2
	UFUNCTION(BlueprintCallable) FText GetCurrentMissionTitle() const;
	UFUNCTION(BlueprintCallable) FName GetCurrentObjectiveId() const;

	void NotifyBossDefeated();
	void NotifyPlayerDied();

	// GameInstance subsystems survive OpenLevel, so campaign state must be
	// reset explicitly on every level load or the death screen persists.
	void ResetCampaign();

	// Death costs the mission, not the campaign (checkpoint standard).
	void ResetToMissionStart();

	UFUNCTION(BlueprintCallable) bool IsCampaignComplete() const;
	UFUNCTION(BlueprintCallable) bool IsPlayerDead() const;

private:
	UPROPERTY() TArray<FVeilObjective> Objectives;
	UPROPERTY() int32 CurrentIndex = 0;
	UPROPERTY() bool bCampaignComplete = false;
	UPROPERTY() bool bPlayerDead = false;
};
