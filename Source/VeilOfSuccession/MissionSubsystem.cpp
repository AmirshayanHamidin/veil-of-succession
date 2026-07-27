// Veil of Succession - fictional political thriller. All characters fictitious.
#include "MissionSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

#define LOCTEXT_NAMESPACE "Veil"

void UMissionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Objectives.Empty();

	// M0 "The Wall of Roses"
	const FText M0 = LOCTEXT("M0Title", "The Wall of Roses");
	Objectives.Add(FVeilObjective(TEXT("Obj_EnterGrounds"), 0, M0,
		LOCTEXT("Obj_EnterGrounds", "Slip past the gate into the rose-garden courtyard.")));
	Objectives.Add(FVeilObjective(TEXT("Obj_Keycard"), 0, M0,
		LOCTEXT("Obj_Keycard", "Lift a security keycard from the courtyard patrol.")));
	Objectives.Add(FVeilObjective(TEXT("Obj_EnterPalace"), 0, M0,
		LOCTEXT("Obj_EnterPalace", "Use the keycard to enter the palace.")));

	// M1 "The Custodians Convene"
	const FText M1 = LOCTEXT("M1Title", "The Custodians Convene");
	Objectives.Add(FVeilObjective(TEXT("Obj_Eavesdrop"), 1, M1,
		LOCTEXT("Obj_Eavesdrop", "Eavesdrop on the meeting chamber (stay hidden, listen 20s).")));
	Objectives.Add(FVeilObjective(TEXT("Obj_StealDossier"), 1, M1,
		LOCTEXT("Obj_StealDossier", "Steal the succession dossier from the chamber.")));
	Objectives.Add(FVeilObjective(TEXT("Obj_ReachThroneRoom"), 1, M1,
		LOCTEXT("Obj_ReachThroneRoom", "Reach the Peacock Throne room.")));

	// M2 "The Peacock Throne"
	const FText M2 = LOCTEXT("M2Title", "The Peacock Throne");
	Objectives.Add(FVeilObjective(TEXT("Obj_DefeatAzadi"), 2, M2,
		LOCTEXT("Obj_DefeatAzadi", "Subdue General Farrokh Azadi.")));
	Objectives.Add(FVeilObjective(TEXT("Obj_Escape"), 2, M2,
		LOCTEXT("Obj_Escape", "Escape to the extraction point at the gate.")));

	CurrentIndex = 0;
	bCampaignComplete = false;
	bPlayerDead = false;

	UE_LOG(LogVeil, Log, TEXT("MissionSubsystem initialized with %d objectives."), Objectives.Num());
}

void UMissionSubsystem::CompleteObjective(FName ObjectiveId)
{
	if (bCampaignComplete || bPlayerDead)
	{
		return;
	}
	if (!Objectives.IsValidIndex(CurrentIndex))
	{
		return;
	}
	// Only advance if the id matches the current objective (out-of-order ignored).
	if (Objectives[CurrentIndex].Id != ObjectiveId)
	{
		return;
	}

	UE_LOG(LogVeil, Log, TEXT("Objective completed: %s"), *ObjectiveId.ToString());

	const bool bWasFinal = (CurrentIndex == Objectives.Num() - 1);
	++CurrentIndex;

	if (bWasFinal)
	{
		bCampaignComplete = true;
		CurrentIndex = Objectives.Num() - 1;
	}
}

FText UMissionSubsystem::GetCurrentObjectiveText() const
{
	if (bCampaignComplete)
	{
		return NSLOCTEXT("Veil", "Complete", "Mission complete.");
	}
	if (Objectives.IsValidIndex(CurrentIndex))
	{
		return Objectives[CurrentIndex].Text;
	}
	return FText::GetEmpty();
}

int32 UMissionSubsystem::GetCurrentMissionIndex() const
{
	if (Objectives.IsValidIndex(CurrentIndex))
	{
		return Objectives[CurrentIndex].MissionIndex;
	}
	return 0;
}

FText UMissionSubsystem::GetCurrentMissionTitle() const
{
	if (Objectives.IsValidIndex(CurrentIndex))
	{
		return Objectives[CurrentIndex].MissionTitle;
	}
	return FText::GetEmpty();
}

FName UMissionSubsystem::GetCurrentObjectiveId() const
{
	if (Objectives.IsValidIndex(CurrentIndex))
	{
		return Objectives[CurrentIndex].Id;
	}
	return NAME_None;
}

void UMissionSubsystem::NotifyBossDefeated()
{
	// Boss defeat completes Obj_DefeatAzadi (only if it is current).
	CompleteObjective(TEXT("Obj_DefeatAzadi"));
}

void UMissionSubsystem::NotifyPlayerDied()
{
	bPlayerDead = true;
	UE_LOG(LogVeil, Log, TEXT("Player died - mission failed."));
}

void UMissionSubsystem::ResetToMissionStart()
{
	const int32 Mission = GetCurrentMissionIndex();
	for (int32 i = 0; i < Objectives.Num(); ++i)
	{
		if (Objectives[i].MissionIndex == Mission)
		{
			CurrentIndex = i;
			break;
		}
	}
	bPlayerDead = false;
	bCampaignComplete = false;
	UE_LOG(LogVeil, Log, TEXT("Respawn at mission %d start."), Mission);
}

void UMissionSubsystem::ResetCampaign()
{
	CurrentIndex = 0;
	bCampaignComplete = false;
	bPlayerDead = false;
	UE_LOG(LogVeil, Log, TEXT("Campaign state reset."));
}

bool UMissionSubsystem::IsCampaignComplete() const
{
	return bCampaignComplete;
}

bool UMissionSubsystem::IsPlayerDead() const
{
	return bPlayerDead;
}

#undef LOCTEXT_NAMESPACE
