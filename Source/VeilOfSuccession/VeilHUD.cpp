// Veil of Succession - fictional political thriller. All characters fictitious.
#include "VeilHUD.h"
#include "GuardCharacter.h"
#include "InteractableActor.h"
#include "EngineUtils.h"
#include "GuardAIController.h"
#include "MissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "MissionSubsystem.h"
#include "SpyCharacter.h"
#include "BossCharacter.h"
#include "InteractableActor.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "GuardAIController.h"
#include "MissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);

void AVeilHUD::DrawBar(float X, float Y, float W, float H, float Fraction,
	const FLinearColor& FillColor, const FLinearColor& BgColor)
{
	Fraction = FMath::Clamp(Fraction, 0.f, 1.f);
	DrawRect(BgColor, X, Y, W, H);
	DrawRect(FillColor, X, Y, W * Fraction, H);
}

void AVeilHUD::DrawLabel(const FString& Text, float X, float Y, const FLinearColor& Color, bool bLarge)
{
	UFont* Font = bLarge ? GEngine->GetLargeFont() : GEngine->GetMediumFont();
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Text), Font, Color);
	Item.EnableShadow(FLinearColor::Black);
	Canvas->DrawItem(Item);
}

void AVeilHUD::DrawCenteredLabel(const FString& Text, float CenterX, float Y, const FLinearColor& Color, bool bLarge)
{
	UFont* Font = bLarge ? GEngine->GetLargeFont() : GEngine->GetMediumFont();
	float TW = 0.f, TH = 0.f;
	GetTextSize(Text, TW, TH, Font);
	DrawLabel(Text, CenterX - TW * 0.5f, Y, Color, bLarge);
}

AInteractableActor* AVeilHUD::FindActiveEavesdrop() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}
	for (TActorIterator<AInteractableActor> It(World); It; ++It)
	{
		AInteractableActor* A = *It;
		if (A && A->Type == EInteractableType::EavesdropZone && A->IsActiveObjective())
		{
			return A;
		}
	}
	return nullptr;
}

void AVeilHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas || !GEngine)
	{
		return;
	}

	const float SW = Canvas->SizeX;
	const float SH = Canvas->SizeY;

	UMissionSubsystem* Mission = nullptr;
	if (UGameInstance* GI = GetGameInstance())
	{
		Mission = GI->GetSubsystem<UMissionSubsystem>();
	}

	ASpyCharacter* Player = Cast<ASpyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	const FLinearColor White(1.f, 1.f, 1.f, 1.f);
	const FLinearColor Faded(0.85f, 0.85f, 0.85f, 1.f);

	// Full-screen states first.
	const bool bDead = (Mission && Mission->IsPlayerDead()) || (Player && Player->IsDead());
	const bool bVictory = Mission && Mission->IsCampaignComplete();

	if (bDead)
	{
		DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.75f), 0, 0, SW, SH);
		DrawCenteredLabel(TEXT("COVER BLOWN"), SW * 0.5f, SH * 0.42f, FLinearColor(0.9f, 0.1f, 0.1f), true);
		DrawCenteredLabel(TEXT("Yasaman's mission ends here. Press R to restart"),
			SW * 0.5f, SH * 0.42f + 40.f, White, false);
		return;
	}
	if (bVictory)
	{
		DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.7f), 0, 0, SW, SH);
		DrawCenteredLabel(TEXT("THE SUCCESSION IS SAFE - for now."),
			SW * 0.5f, SH * 0.45f, FLinearColor(0.9f, 0.75f, 0.2f), true);
		return;
	}

	// Mission title + current objective, top-left.
	if (Mission)
	{
		DrawLabel(Mission->GetCurrentMissionTitle().ToString(), 40.f, 36.f, FLinearColor(0.9f, 0.8f, 0.4f), true);
		DrawLabel(Mission->GetCurrentObjectiveText().ToString(), 40.f, 66.f, Faded, false);
	}

	// Health bar bottom-left + dart count under it.
	if (Player)
	{
		const float BarX = 40.f;
		const float BarW = 260.f;
		const float BarH = 22.f;
		const float HealthY = SH - 90.f;
		const float HealthFrac = Player->GetMaxHealth() > 0.f ? Player->GetHealth() / Player->GetMaxHealth() : 0.f;
		DrawLabel(TEXT("HEALTH"), BarX, HealthY - 22.f, White, false);
		DrawBar(BarX, HealthY, BarW, BarH, HealthFrac,
			FLinearColor(0.2f, 0.85f, 0.3f), FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));

		// Shield bar sits under health, dimmed while recharging.
		const float ShieldY = HealthY + BarH + 6.f;
		const float ShieldFrac = Player->GetMaxShield() > 0.f ? Player->GetShield() / Player->GetMaxShield() : 0.f;
		const FLinearColor ShieldCol = Player->IsShieldActive()
			? FLinearColor(0.30f, 0.70f, 1.00f)
			: FLinearColor(0.16f, 0.34f, 0.50f);
		DrawBar(BarX, ShieldY, BarW, 12.f, ShieldFrac, ShieldCol, FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
		DrawLabel(Player->IsShieldActive() ? TEXT("SHIELD  UP") : TEXT("SHIELD"),
			BarX, ShieldY + 14.f, ShieldCol, false);

		const FString DartText = FString::Printf(TEXT("%s  %d   (D-PAD / WHEEL to switch)"),
			*Player->GetWeaponName(), Player->GetCurrentWeaponAmmo());
		DrawLabel(DartText, BarX, ShieldY + 34.f, FLinearColor(1.f, 0.9f, 0.5f), false);

		// Dry fire: make "out of darts" impossible to miss.
		if (Player->GetTimeSinceEmptyFire() < 0.8f)
		{
			DrawLabel(TEXT("NO DARTS - RECHARGING"), BarX + 90.f, ShieldY + 34.f,
				FLinearColor(1.f, 0.25f, 0.2f), false);
		}

		// Movement state - without this, crouch and sprint give no feedback at
		// all, because the mannequin's anim blueprint has no poses for them.
		FString StateText;
		FLinearColor StateCol = White;
		if (Player->IsCrouching())      { StateText = TEXT("CROUCHED  -  quiet");  StateCol = FLinearColor(0.4f, 0.9f, 0.6f); }
		else if (Player->IsSprinting()) { StateText = TEXT("SPRINTING  -  loud");  StateCol = FLinearColor(1.0f, 0.6f, 0.2f); }
		else                            { StateText = TEXT("WALKING"); StateCol = FLinearColor(0.7f, 0.7f, 0.7f); }
		DrawLabel(StateText, BarX, ShieldY + 54.f, StateCol, false);

		// Hit marker: four ticks around the centre for a quarter second.
		if (Player->GetTimeSinceHitMarker() < 0.25f)
		{
			const FLinearColor HM(0.5f, 0.95f, 1.0f);
			const float CX = SW * 0.5f, CY = SH * 0.5f;
			DrawRect(HM, CX - 14.f, CY - 1.f, 8.f, 2.f);
			DrawRect(HM, CX + 6.f,  CY - 1.f, 8.f, 2.f);
			DrawRect(HM, CX - 1.f, CY - 14.f, 2.f, 8.f);
			DrawRect(HM, CX - 1.f, CY + 6.f,  2.f, 8.f);
		}

		// Interact prompt: near an active pickup/door, say which button to press.
		for (TActorIterator<AInteractableActor> It(GetWorld()); It; ++It)
		{
			AInteractableActor* A = *It;
			if (!A || !A->IsActiveObjective())
			{
				continue;
			}
			const bool bNeedsPress = (A->Type == EInteractableType::Keycard
				|| A->Type == EInteractableType::Intel || A->Type == EInteractableType::Door);
			if (!bNeedsPress)
			{
				continue;
			}
			if (FVector::Dist(Player->GetActorLocation(), A->GetActorLocation()) < 260.f)
			{
				const FString P = FString::Printf(TEXT("[E / SQUARE]  %s"), *A->Prompt.ToString().ToUpper());
				float PW2, PH2;
				GetTextSize(P, PW2, PH2, GEngine->GetLargeFont());
				DrawLabel(P, SW * 0.5f - PW2 * 0.5f, SH * 0.68f, FLinearColor(0.5f, 1.0f, 0.6f), true);
			}
			break;
		}

		// Takedown prompt when behind an unaware guard.
		if (Player->FindTakedownTarget())
		{
			const FString Prompt = TEXT("[V / R1]  KNIFE  -  SILENT KILL");
			float PW, PH;
			GetTextSize(Prompt, PW, PH, GEngine->GetLargeFont());
			DrawLabel(Prompt, SW * 0.5f - PW * 0.5f, SH * 0.62f, FLinearColor(1.0f, 0.85f, 0.25f), true);
		}
	}

	// Detection meter top-center (red > 0.7).
	if (Player)
	{
		const float Det = Player->GetDetection();
		const float MW = 300.f;
		const float MH = 16.f;
		const float MX = SW * 0.5f - MW * 0.5f;
		const float MY = 30.f;
		const FLinearColor Fill = (Det > 0.7f) ? FLinearColor(0.9f, 0.15f, 0.1f) : FLinearColor(0.9f, 0.8f, 0.2f);
		DrawCenteredLabel(TEXT("DETECTION"), SW * 0.5f, MY - 20.f, White, false);
		DrawBar(MX, MY, MW, MH, Det, Fill, FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
	}

	// Eavesdrop progress bar when in the active zone.
	if (AInteractableActor* Zone = FindActiveEavesdrop())
	{
		const float Prog = Zone->GetEavesdropProgress();
		if (Prog > 0.f && Prog < 1.f)
		{
			const float MW = 300.f;
			const float MH = 16.f;
			const float MX = SW * 0.5f - MW * 0.5f;
			const float MY = SH * 0.5f + 60.f;
			DrawCenteredLabel(TEXT("LISTENING..."), SW * 0.5f, MY - 20.f, White, false);
			DrawBar(MX, MY, MW, MH, Prog, FLinearColor(0.6f, 0.3f, 1.0f), FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
		}
	}

	// Boss health bar top-center when boss activated and alive.
	if (!CachedBoss.IsValid())
	{
		for (TActorIterator<ABossCharacter> It(GetWorld()); It; ++It)
		{
			CachedBoss = *It;
			break;
		}
	}
	if (ABossCharacter* Boss = CachedBoss.Get())
	{
		if (Boss->IsActivated() && !Boss->IsDeadOrStunned())
		{
			const float BW = 480.f;
			const float BH = 20.f;
			const float BX = SW * 0.5f - BW * 0.5f;
			const float BY = 66.f;
			DrawCenteredLabel(TEXT("GENERAL FARROKH AZADI"), SW * 0.5f, BY - 22.f, FLinearColor(0.9f, 0.2f, 0.2f), false);
			DrawBar(BX, BY, BW, BH, Boss->GetBossHealthPct(),
				FLinearColor(0.85f, 0.1f, 0.12f), FLinearColor(0.1f, 0.1f, 0.1f, 0.85f));
		}
	}

	// Waypoint: a FOLLOW sign projected over the current objective, with
	// distance, clamped to the screen edge when the goal is behind a wall.
	if (Player)
	{
		for (TActorIterator<AInteractableActor> It(GetWorld()); It; ++It)
		{
			AInteractableActor* Obj = *It;
			if (!Obj || !Obj->IsActiveObjective())
			{
				continue;
			}

			FVector Screen = Project(Obj->GetActorLocation() + FVector(0.f, 0.f, 150.f));
			const bool bBehind = Screen.Z <= 0.f;
			float MX2 = Screen.X, MY2 = Screen.Y;
			if (bBehind) { MX2 = SW - MX2; MY2 = SH * 0.5f; }
			MX2 = FMath::Clamp(MX2, 90.f, SW - 90.f);
			MY2 = FMath::Clamp(MY2, 70.f, SH - 120.f);

			const float DistM = FVector::Dist(Player->GetActorLocation(), Obj->GetActorLocation()) / 100.f;
			const FLinearColor WayCol(1.0f, 0.8f, 0.2f);
			const FString WayText = FString::Printf(TEXT("FOLLOW  %.0f m"), DistM);
			float WW, WH;
			GetTextSize(WayText, WW, WH, GEngine->GetLargeFont());
			DrawLabel(WayText, MX2 - WW * 0.5f, MY2 - WH - 14.f, WayCol, true);
			// Down-pointing chevron under the label.
			DrawRect(WayCol, MX2 - 9.f, MY2 - 10.f, 18.f, 4.f);
			DrawRect(WayCol, MX2 - 5.f, MY2 - 5.f,  10.f, 4.f);
			DrawRect(WayCol, MX2 - 2.f, MY2,        4.f,  4.f);
			break;
		}
	}

	// ----- Lock-on brackets around the marked target -----
	if (Player)
	{
		if (AGuardCharacter* T = Player->GetLockTarget())
		{
			const FVector S = Project(T->GetActorLocation() + FVector(0.f, 0.f, 20.f));
			if (S.Z > 0.f)
			{
				const FLinearColor LC(1.f, 0.55f, 0.1f);
				DrawRect(LC, S.X - 26.f, S.Y - 26.f, 12.f, 3.f);
				DrawRect(LC, S.X - 26.f, S.Y - 26.f, 3.f, 12.f);
				DrawRect(LC, S.X + 14.f, S.Y - 26.f, 12.f, 3.f);
				DrawRect(LC, S.X + 23.f, S.Y - 26.f, 3.f, 12.f);
				DrawRect(LC, S.X - 26.f, S.Y + 23.f, 12.f, 3.f);
				DrawRect(LC, S.X - 26.f, S.Y + 14.f, 3.f, 12.f);
				DrawRect(LC, S.X + 14.f, S.Y + 23.f, 12.f, 3.f);
				DrawRect(LC, S.X + 23.f, S.Y + 14.f, 3.f, 12.f);
			}
		}

		// Successful parry: big, brief, unmistakable.
		if (Player->GetTimeSinceCounter() < 1.0f)
		{
			const FString CT = TEXT("COUNTER!");
			float CW, CH;
			GetTextSize(CT, CW, CH, GEngine->GetLargeFont(), 1.8f);
			DrawText(CT, FLinearColor(0.4f, 1.f, 0.5f), SW * 0.5f - CW * 0.5f, SH * 0.33f,
				GEngine->GetLargeFont(), 1.8f);
		}

		// Teach the parry at the moment it matters: enemy winding up in range.
		for (TActorIterator<AGuardCharacter> It(GetWorld()); It; ++It)
		{
			AGuardCharacter* G = *It;
			if (G && !G->IsDeadOrStunned() && G->IsWindingUp()
				&& FVector::Dist(Player->GetActorLocation(), G->GetActorLocation()) < 320.f)
			{
				const FString PT = TEXT("[V / R1]  COUNTER");
				float PW4, PH4;
				GetTextSize(PT, PW4, PH4, GEngine->GetLargeFont());
				DrawLabel(PT, SW * 0.5f - PW4 * 0.5f, SH * 0.56f, FLinearColor(1.f, 0.9f, 0.3f), true);
				break;
			}
		}
	}

	// ----- Guard alert pips: "?" while suspicious, "!" while chasing (#54) -----
	if (Player)
	{
		for (TActorIterator<AGuardCharacter> It(GetWorld()); It; ++It)
		{
			AGuardCharacter* G = *It;
			if (!G || G->IsDeadOrStunned())
			{
				continue;
			}
			const AGuardAIController* AI = Cast<AGuardAIController>(G->GetController());
			if (!AI || AI->GetState() == EGuardState::Patrol)
			{
				continue;
			}
			const FVector S = Project(G->GetActorLocation() + FVector(0.f, 0.f, 210.f));
			if (S.Z <= 0.f)
			{
				continue;
			}
			const bool bChase = AI->GetState() == EGuardState::Chasing;
			DrawLabel(bChase ? TEXT("!") : TEXT("?"), S.X - 4.f, S.Y - 12.f,
				bChase ? FLinearColor(1.f, 0.15f, 0.1f) : FLinearColor(1.f, 0.85f, 0.2f), true);
		}
	}

	// ----- Damage direction indicator (#41) -----
	if (Player && Player->GetTimeSinceDamaged() < 0.9f && !Player->IsDead())
	{
		const FVector DmgDir = Player->GetLastDamageDirection();
		FVector CamFwd = FVector::ForwardVector;
		FVector CamRight = FVector::RightVector;
		if (APlayerController* PC = GetOwningPlayerController())
		{
			const FRotator CamRot(0.f, PC->GetControlRotation().Yaw, 0.f);
			CamFwd = CamRot.Vector();
			CamRight = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);
		}
		const float Fwd = FVector::DotProduct(CamFwd, DmgDir);
		const float Right = FVector::DotProduct(CamRight, DmgDir);
		const float Ang = FMath::Atan2(Right, Fwd);   // 0 = ahead
		const float R = 130.f;
		const float PX = SW * 0.5f + FMath::Sin(Ang) * R;
		const float PY = SH * 0.5f - FMath::Cos(Ang) * R;
		DrawRect(FLinearColor(1.f, 0.1f, 0.08f, 0.85f), PX - 12.f, PY - 4.f, 24.f, 8.f);
	}

	// ----- Low health pulse (#42) -----
	if (Player && !Player->IsDead() && Player->GetMaxHealth() > 0.f
		&& Player->GetHealth() / Player->GetMaxHealth() < 0.35f)
	{
		const float Pulse = 0.25f + 0.2f * FMath::Sin(GetWorld()->GetTimeSeconds() * 6.f);
		const FLinearColor Blood(0.6f, 0.02f, 0.02f, Pulse);
		DrawRect(Blood, 0, 0, SW, 26.f);
		DrawRect(Blood, 0, SH - 26.f, SW, 26.f);
		DrawRect(Blood, 0, 0, 26.f, SH);
		DrawRect(Blood, SW - 26.f, 0, 26.f, SH);
	}

	// ----- Mission splash on act change (#28) + opening title card (#80) -----
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UMissionSubsystem* Missions = GI->GetSubsystem<UMissionSubsystem>())
		{
			const int32 M = Missions->GetCurrentMissionIndex();
			if (M != LastMissionIndex)
			{
				LastMissionIndex = M;
				MissionSplashText = Missions->GetCurrentMissionTitle();
				MissionSplashTime = GetWorld()->GetTimeSeconds();
			}
		}
	}
	const float SinceSplash = GetWorld()->GetTimeSeconds() - MissionSplashTime;
	if (SinceSplash < 3.5f)
	{
		const FString T = MissionSplashText.ToString().ToUpper();
		float TW2, TH2;
		GetTextSize(T, TW2, TH2, GEngine->GetLargeFont(), 2.f);
		DrawText(T, FLinearColor(1.f, 0.85f, 0.4f), SW * 0.5f - TW2 * 0.5f, SH * 0.22f,
			GEngine->GetLargeFont(), 2.f);
	}
	if (GetWorld()->GetTimeSeconds() < 5.f)
	{
		const FString Title = TEXT("VEIL OF SUCCESSION");
		float TW3, TH3;
		GetTextSize(Title, TW3, TH3, GEngine->GetLargeFont(), 3.f);
		DrawText(Title, FLinearColor(0.9f, 0.9f, 1.f), SW * 0.5f - TW3 * 0.5f, SH * 0.4f,
			GEngine->GetLargeFont(), 3.f);
		DrawLabel(TEXT("A work of political fiction. All characters are fictitious."),
			SW * 0.5f - 190.f, SH * 0.4f + TH3 + 8.f, FLinearColor(0.7f, 0.7f, 0.75f), false);
	}

	// ----- Boss phase banner (#39) -----
	{
		ABossCharacter* Boss = nullptr;
		for (TActorIterator<ABossCharacter> It(GetWorld()); It; ++It) { Boss = *It; break; }
		if (Boss && Boss->IsActivated() && !Boss->IsDeadOrStunned())
		{
			const float Pct = Boss->GetBossHealthPct();
			const int32 Phase = Pct > 0.66f ? 1 : (Pct > 0.33f ? 2 : 3);
			if (Phase != LastBossPhase)
			{
				LastBossPhase = Phase;
				PhaseSplashTime = GetWorld()->GetTimeSeconds();
			}
			if (GetWorld()->GetTimeSeconds() - PhaseSplashTime < 2.5f && Phase > 1)
			{
				const FString PT = Phase == 2
					? TEXT("PHASE 2 - THE GENERAL CALLS HIS GUARD")
					: TEXT("PHASE 3 - NOTHING LEFT TO LOSE");
				float PW3, PH3;
				GetTextSize(PT, PW3, PH3, GEngine->GetLargeFont(), 1.4f);
				DrawText(PT, FLinearColor(1.f, 0.25f, 0.15f), SW * 0.5f - PW3 * 0.5f, SH * 0.28f,
					GEngine->GetLargeFont(), 1.4f);
			}
		}
	}

	// ----- Version stamp (#95) -----
	DrawLabel(TEXT("v0.9"), SW - 52.f, SH - 58.f, FLinearColor(0.5f, 0.5f, 0.55f, 0.6f), false);

	// Crosshair: faint dot at the hip, solid dot when aimed (shots are true).
	if (Player && !Player->IsDead())
	{
		const bool bAim = Player->IsAiming();
		const float Dot = bAim ? 6.f : 4.f;
		const FLinearColor CrossCol = bAim ? White : FLinearColor(1.f, 1.f, 1.f, 0.35f);
		DrawRect(CrossCol, SW * 0.5f - Dot * 0.5f, SH * 0.5f - Dot * 0.5f, Dot, Dot);
	}

	// Controls hint bottom-right.
	{
		const FString Hint = TEXT("LS move  RS look  L3 sprint  X jump  O crouch  L2 aim  R2 fire  R1 knife  R3/X dodge  Tab/D-up lock  Square interact  L1 shield");
		float TW = 0.f, TH = 0.f;
		GetTextSize(Hint, TW, TH, GEngine->GetMediumFont());
		DrawLabel(Hint, SW - TW - 30.f, SH - 34.f, FLinearColor(0.8f, 0.8f, 0.8f, 0.9f), false);
	}
}
