// Veil of Succession - fictional political thriller. All characters fictitious.
#include "SpyPlayerController.h"
#include "SpyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputModifiers.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogVeilInput, Log, All);

ASpyPlayerController::ASpyPlayerController()
{
	MappingContext = nullptr;
	IA_Move = nullptr;
	IA_Look = nullptr;
	IA_Sprint = nullptr;
	IA_Crouch = nullptr;
	IA_Aim = nullptr;
	IA_Fire = nullptr;
	IA_Interact = nullptr;
	IA_Restart = nullptr;
	IA_Takedown = nullptr;
	IA_Shield = nullptr;
	IA_Jump = nullptr;
	IA_Knife = nullptr;
	IA_WeaponNext = nullptr;
	IA_WeaponPrev = nullptr;
	IA_Dodge = nullptr;
	IA_LockOn = nullptr;
	bActionsBuilt = false;
}

void ASpyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	BuildInputActions();
	RegisterMappingContext();
}

void ASpyPlayerController::BuildInputActions()
{
	if (bActionsBuilt)
	{
		return;
	}

	IA_Move = NewObject<UInputAction>(this, TEXT("IA_Move"));
	IA_Move->ValueType = EInputActionValueType::Axis2D;

	IA_Look = NewObject<UInputAction>(this, TEXT("IA_Look"));
	IA_Look->ValueType = EInputActionValueType::Axis2D;

	IA_Sprint = NewObject<UInputAction>(this, TEXT("IA_Sprint"));
	IA_Sprint->ValueType = EInputActionValueType::Boolean;

	IA_Crouch = NewObject<UInputAction>(this, TEXT("IA_Crouch"));
	IA_Crouch->ValueType = EInputActionValueType::Boolean;

	IA_Aim = NewObject<UInputAction>(this, TEXT("IA_Aim"));
	IA_Aim->ValueType = EInputActionValueType::Boolean;

	IA_Fire = NewObject<UInputAction>(this, TEXT("IA_Fire"));
	IA_Fire->ValueType = EInputActionValueType::Boolean;

	IA_Interact = NewObject<UInputAction>(this, TEXT("IA_Interact"));
	IA_Interact->ValueType = EInputActionValueType::Boolean;

	IA_Restart = NewObject<UInputAction>(this, TEXT("IA_Restart"));
	IA_Restart->ValueType = EInputActionValueType::Boolean;

	IA_Takedown = NewObject<UInputAction>(this, TEXT("IA_Takedown"));
	IA_Takedown->ValueType = EInputActionValueType::Boolean;

	IA_Shield = NewObject<UInputAction>(this, TEXT("IA_Shield"));
	IA_Shield->ValueType = EInputActionValueType::Boolean;

	IA_Jump = NewObject<UInputAction>(this, TEXT("IA_Jump"));
	IA_Jump->ValueType = EInputActionValueType::Boolean;

	IA_Knife = NewObject<UInputAction>(this, TEXT("IA_Knife"));
	IA_Knife->ValueType = EInputActionValueType::Boolean;

	IA_WeaponNext = NewObject<UInputAction>(this, TEXT("IA_WeaponNext"));
	IA_WeaponNext->ValueType = EInputActionValueType::Boolean;

	IA_WeaponPrev = NewObject<UInputAction>(this, TEXT("IA_WeaponPrev"));
	IA_WeaponPrev->ValueType = EInputActionValueType::Boolean;

	IA_Dodge = NewObject<UInputAction>(this, TEXT("IA_Dodge"));
	IA_Dodge->ValueType = EInputActionValueType::Boolean;

	IA_LockOn = NewObject<UInputAction>(this, TEXT("IA_LockOn"));
	IA_LockOn->ValueType = EInputActionValueType::Boolean;

	MappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Spy"));

	// Helper: map a key and append the requested modifiers.
	auto AddSwizzle = [this](FEnhancedActionKeyMapping& Mapping)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(this);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		Mapping.Modifiers.Add(Swizzle);
	};
	auto AddNegate = [this](FEnhancedActionKeyMapping& Mapping)
	{
		Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(this));
	};

	// --- Move (Axis2D): X = right, Y = forward ---
	{
		FEnhancedActionKeyMapping& MW = MappingContext->MapKey(IA_Move, EKeys::W);
		AddSwizzle(MW);                                   // forward (+Y)

		FEnhancedActionKeyMapping& MS = MappingContext->MapKey(IA_Move, EKeys::S);
		AddSwizzle(MS); AddNegate(MS);                    // back (-Y)

		MappingContext->MapKey(IA_Move, EKeys::D);        // right (+X)

		FEnhancedActionKeyMapping& MA = MappingContext->MapKey(IA_Move, EKeys::A);
		AddNegate(MA);                                    // left (-X)

		MappingContext->MapKey(IA_Move, EKeys::Gamepad_LeftX);   // right (+X)

		FEnhancedActionKeyMapping& MGY = MappingContext->MapKey(IA_Move, EKeys::Gamepad_LeftY);
		AddSwizzle(MGY);                                  // forward (+Y)
	}

	// --- Look (Axis2D): X = yaw, Y = pitch ---
	{
		// UE reports mouse-up as +Y, and AddControllerPitchInput(+) looks DOWN,
		// so the mouse needs a Y-only negate (the gamepad stick gets one below).
		FEnhancedActionKeyMapping& LM = MappingContext->MapKey(IA_Look, EKeys::Mouse2D);
		UInputModifierNegate* NegY = NewObject<UInputModifierNegate>(this);
		NegY->bX = false;
		NegY->bY = true;
		NegY->bZ = false;
		LM.Modifiers.Add(NegY);

		MappingContext->MapKey(IA_Look, EKeys::Gamepad_RightX);  // yaw (+X)

		FEnhancedActionKeyMapping& LGY = MappingContext->MapKey(IA_Look, EKeys::Gamepad_RightY);
		AddSwizzle(LGY); AddNegate(LGY);                 // pitch, negated for stick-up = look up
	}

	// --- Sprint (hold): Shift / L3 - X now belongs to Jump ---
	MappingContext->MapKey(IA_Sprint, EKeys::LeftShift);
	MappingContext->MapKey(IA_Sprint, EKeys::Gamepad_LeftThumbstick);

	// --- Jump: Space / X ---
	MappingContext->MapKey(IA_Jump, EKeys::SpaceBar);
	MappingContext->MapKey(IA_Jump, EKeys::Gamepad_FaceButton_Bottom);

	// --- Knife: V / R1 (stealth kill from behind, slash up front) ---
	MappingContext->MapKey(IA_Knife, EKeys::V);
	MappingContext->MapKey(IA_Knife, EKeys::Gamepad_RightShoulder);

	// --- Weapon cycling: mouse wheel / D-pad ---
	MappingContext->MapKey(IA_WeaponNext, EKeys::MouseScrollUp);
	MappingContext->MapKey(IA_WeaponNext, EKeys::Gamepad_DPad_Right);
	MappingContext->MapKey(IA_WeaponPrev, EKeys::MouseScrollDown);
	MappingContext->MapKey(IA_WeaponPrev, EKeys::Gamepad_DPad_Left);

	// --- Dodge roll: X key / R3 ---
	MappingContext->MapKey(IA_Dodge, EKeys::X);
	MappingContext->MapKey(IA_Dodge, EKeys::Gamepad_RightThumbstick);

	// --- Target lock: Tab / D-pad up ---
	MappingContext->MapKey(IA_LockOn, EKeys::Tab);
	MappingContext->MapKey(IA_LockOn, EKeys::Gamepad_DPad_Up);

	// --- Crouch (toggle) ---
	MappingContext->MapKey(IA_Crouch, EKeys::C);
	MappingContext->MapKey(IA_Crouch, EKeys::Gamepad_FaceButton_Right);

	// --- Aim (hold) ---
	MappingContext->MapKey(IA_Aim, EKeys::RightMouseButton);
	MappingContext->MapKey(IA_Aim, EKeys::B);   // keyboard hold-to-aim alternative
	MappingContext->MapKey(IA_Aim, EKeys::Gamepad_LeftTrigger);

	// --- Fire ---
	MappingContext->MapKey(IA_Fire, EKeys::LeftMouseButton);
	MappingContext->MapKey(IA_Fire, EKeys::Gamepad_RightTrigger);

	// --- Interact ---
	MappingContext->MapKey(IA_Interact, EKeys::E);
	MappingContext->MapKey(IA_Interact, EKeys::Gamepad_FaceButton_Left);

	// --- Restart ---
	MappingContext->MapKey(IA_Restart, EKeys::R);
	MappingContext->MapKey(IA_Restart, EKeys::Gamepad_Special_Right);

	// --- Silent takedown (Triangle / F) ---
	MappingContext->MapKey(IA_Takedown, EKeys::F);
	MappingContext->MapKey(IA_Takedown, EKeys::Gamepad_FaceButton_Top);

	// --- Ballistic shield (L1 / Q) ---
	MappingContext->MapKey(IA_Shield, EKeys::Q);
	MappingContext->MapKey(IA_Shield, EKeys::Gamepad_LeftShoulder);

	bActionsBuilt = true;
}

void ASpyPlayerController::RegisterMappingContext()
{
	if (!MappingContext)
	{
		return;
	}
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void ASpyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BuildInputActions();

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EIC)
	{
		UE_LOG(LogVeilInput, Error, TEXT("SpyPlayerController requires UEnhancedInputComponent."));
		return;
	}

	// Controller-owned action.
	EIC->BindAction(IA_Restart, ETriggerEvent::Started, this, &ASpyPlayerController::HandleRestart);
}

void ASpyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BuildInputActions();
	RegisterMappingContext();

	ASpyCharacter* Spy = Cast<ASpyCharacter>(InPawn);
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (!Spy || !EIC)
	{
		return;
	}

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, Spy, &ASpyCharacter::HandleMove);
	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, Spy, &ASpyCharacter::HandleLook);

	EIC->BindAction(IA_Sprint, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleSprintStart);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, Spy, &ASpyCharacter::HandleSprintStop);

	EIC->BindAction(IA_Crouch, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleCrouchToggle);

	EIC->BindAction(IA_Aim, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleAimStart);
	EIC->BindAction(IA_Aim, ETriggerEvent::Completed, Spy, &ASpyCharacter::HandleAimStop);

	EIC->BindAction(IA_Fire, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleFire);
	EIC->BindAction(IA_Fire, ETriggerEvent::Completed, Spy, &ASpyCharacter::HandleFireStop);
	EIC->BindAction(IA_Interact, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleInteract);
	EIC->BindAction(IA_Takedown, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleTakedown);
	EIC->BindAction(IA_Shield, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleShieldStart);
	EIC->BindAction(IA_Shield, ETriggerEvent::Completed, Spy, &ASpyCharacter::HandleShieldStop);
	EIC->BindAction(IA_Jump, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleJumpStart);
	EIC->BindAction(IA_Jump, ETriggerEvent::Completed, Spy, &ASpyCharacter::HandleJumpStop);
	EIC->BindAction(IA_Knife, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleKnife);
	EIC->BindAction(IA_WeaponNext, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleWeaponNext);
	EIC->BindAction(IA_WeaponPrev, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleWeaponPrev);
	EIC->BindAction(IA_Dodge, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleDodge);
	EIC->BindAction(IA_LockOn, ETriggerEvent::Started, Spy, &ASpyCharacter::HandleLockOn);
}

void ASpyPlayerController::HandleRestart(const FInputActionValue& Value)
{
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(this, FName(*World->GetName()));
	}
}
