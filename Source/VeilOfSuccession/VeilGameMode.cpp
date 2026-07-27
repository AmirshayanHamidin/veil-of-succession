// Veil of Succession - fictional political thriller. All characters fictitious.
#include "VeilGameMode.h"
#include "PalaceLevelBuilder.h"
#include "VeilHUD.h"
#include "SpyCharacter.h"
#include "SpyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MissionSubsystem.h"
#include "Engine/GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

AVeilGameMode::AVeilGameMode()
{
	DefaultPawnClass = ASpyCharacter::StaticClass();
	PlayerControllerClass = ASpyPlayerController::StaticClass();
	HUDClass = AVeilHUD::StaticClass();
}

void AVeilGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Died? Resume at the mission you fell in. Fresh run or victory? Full reset.
	int32 SpawnMission = 0;
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UMissionSubsystem* Missions = GI->GetSubsystem<UMissionSubsystem>())
		{
			if (Missions->IsPlayerDead() && !Missions->IsCampaignComplete())
			{
				Missions->ResetToMissionStart();
				SpawnMission = Missions->GetCurrentMissionIndex();
			}
			else
			{
				Missions->ResetCampaign();
			}
		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Builder = World->SpawnActor<APalaceLevelBuilder>(APalaceLevelBuilder::StaticClass(),
		FTransform::Identity, Params);

	if (Builder)
	{
		Builder->BuildLevel(SpawnMission);

		const FTransform Spawn = Builder->GetPlayerSpawnForMission(SpawnMission);
		if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0))
		{
			Pawn->TeleportTo(Spawn.GetLocation(), Spawn.Rotator());
			if (AController* C = Pawn->GetController())
			{
				C->SetControlRotation(Spawn.Rotator());
			}
			UE_LOG(LogVeil, Log, TEXT("Teleported player to spawn %s"), *Spawn.GetLocation().ToString());
		}
	}
}
