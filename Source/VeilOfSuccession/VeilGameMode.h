// Veil of Succession - fictional political thriller. All characters fictitious.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VeilGameMode.generated.h"

class APalaceLevelBuilder;

UCLASS()
class VEILOFSUCCESSION_API AVeilGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVeilGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY() APalaceLevelBuilder* Builder;
};
