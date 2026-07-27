# VEIL OF SUCCESSION — Shared Engineering Spec (v1)
UE 5.3 C++ project. Module name: `VeilOfSuccession`. API macro: `VEILOFSUCCESSION_API`.

## HARD CONSTRAINTS (all executors)
1. ZERO content assets. No .uasset, no .umap, no Blueprints, no UMG widgets, no BehaviorTrees, no anim blueprints. Everything is C++.
2. Meshes: only engine built-ins loaded in constructors via `ConstructorHelpers::FObjectFinder<UStaticMesh>` from `/Engine/BasicShapes/Cube.Cube`, `Cylinder.Cylinder`, `Sphere.Sphere`, `Plane.Plane`. Material: `/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial`, colored at runtime with `UMaterialInstanceDynamic` (parameter name `"Color"`).
3. Characters are "blockout" style: capsule collision + attached static-mesh shapes (body cylinder, head sphere). No skeletal meshes.
4. NO navmesh, NO `MoveTo*`. AI locomotion = `AddMovementInput((Target-Loc).GetSafeNormal2D())` each tick. Level layout guarantees straight-line patrol legs.
5. UI = `AHUD` canvas drawing only (`DrawText`, `DrawRect`, `GetTextSize`). Font: `GEngine->GetLargeFont()` / `GetMediumFont()`.
6. Enhanced Input created dynamically: `NewObject<UInputMappingContext>` / `NewObject<UInputAction>`, mapped with `Ctx->MapKey(...)`, modifiers via `NewObject<UInputModifierSwizzleAxis/Negate>`. Registered through `UEnhancedInputLocalPlayerSubsystem::AddMappingContext`.
7. Every header: `#pragma once`, `.generated.h` last include, UCLASS/UPROPERTY/UFUNCTION correct. Forward-declare in headers, include in .cpp.
8. Log channel: `DEFINE_LOG_CATEGORY_STATIC(LogVeil, Log, All);` in cpps that log.
9. All files under `Source/VeilOfSuccession/`. First line of every file: `// Veil of Succession - fictional political thriller. All characters fictitious.`

## FILE OWNERSHIP
- Executor A: `SpyCharacter.h/.cpp`, `SpyPlayerController.h/.cpp`
- Executor B: `StealthComponent.h/.cpp`, `GuardCharacter.h/.cpp`, `GuardAIController.h/.cpp`, `BossCharacter.h/.cpp`
- Executor C: `MissionSubsystem.h/.cpp`, `VeilGameMode.h/.cpp`, `PalaceLevelBuilder.h/.cpp`, `VeilHUD.h/.cpp`, `InteractableActor.h/.cpp`
- Planner owns: module files, Build.cs, Targets, Config, .uproject.

## CROSS-CLASS CONTRACT — copy these signatures EXACTLY

### ASpyCharacter : ACharacter  (A owns; B and C compile against)
```cpp
UCLASS()
class VEILOFSUCCESSION_API ASpyCharacter : public ACharacter {
  GENERATED_BODY()
public:
  ASpyCharacter();
  UFUNCTION(BlueprintCallable) float GetHealth() const;
  UFUNCTION(BlueprintCallable) float GetMaxHealth() const;
  UFUNCTION(BlueprintCallable) bool IsDead() const;
  UFUNCTION(BlueprintCallable) float GetNoiseLevel() const;      // 0 crouch-still .. 1 sprint
  UFUNCTION(BlueprintCallable) float GetDetection() const;       // 0..1 global exposure
  UFUNCTION(BlueprintCallable) int32 GetAmmo() const;
  UFUNCTION(BlueprintCallable) bool IsAiming() const;
  void AddDetection(float Amount);                               // clamped 0..1, guards call this
  class UStealthComponent* GetStealthComponent() const;
  virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
      class AController* EventInstigator, AActor* DamageCauser) override;
};
```
A also implements internally (internals are A's choice): Move/Look/Sprint/Crouch(toggle, use built-in Crouch()/UnCrouch())/Aim/Fire/Interact.
Tranq pistol: while aiming, fire line-traces 3000uu from camera, `UGameplayStatics::ApplyDamage(HitActor, 50.f, Controller, this, nullptr)`. 12 darts max.
Interact: sphere overlap 250uu for `AInteractableActor`, call `Interact(this)` on nearest active one.
Detection decays -0.15/s when no guard is adding to it (guards add, character decays in Tick).

### UStealthComponent : UActorComponent  (B owns; A creates it in ASpyCharacter constructor via CreateDefaultSubobject)
```cpp
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEILOFSUCCESSION_API UStealthComponent : public UActorComponent {
  GENERATED_BODY()
public:
  UStealthComponent();
  float GetVisibilityMultiplier() const; // crouched .45, sprinting 1.6, else 1.0 (reads owner ACharacter state)
};
```

### AGuardCharacter : ACharacter  (B owns; A and C compile against)
```cpp
UCLASS()
class VEILOFSUCCESSION_API AGuardCharacter : public ACharacter {
  GENERATED_BODY()
public:
  AGuardCharacter();
  void SetPatrolPoints(const TArray<FVector>& Points);
  UFUNCTION(BlueprintCallable) bool IsDeadOrStunned() const;
  virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
      class AController* EventInstigator, AActor* DamageCauser) override;
  UPROPERTY(EditAnywhere, Category="Guard") float MaxHealth = 100.f;
};
```
Guard behavior (B internal): AGuardAIController C++ state machine (Patrol/Suspicious/Chasing) + UAIPerceptionComponent configured in C++ (UAISenseConfig_Sight radius 2200, angle 70deg, lose 2600; UAISenseConfig_Hearing range 1500). Sight effectiveness scaled by player StealthComponent GetVisibilityMultiplier() and distance (implement as: on sight stimulus, accumulate suspicion; full suspicion faster when close/visible). Suspicious: walk to stimulus, wait 3s. Chasing: pursue player, melee 20 dmg at 180uu every 1.2s, call Player->AddDetection(0.5f*DeltaTime) while chasing. Guards use CharacterMovement MaxWalkSpeed 220 patrol / 420 suspicious / 600 chase.
On death (health<=0 from tranq): fake ragdoll = capsule collision off, tilt body mesh 90 deg, disable movement + AI, IsDeadOrStunned()==true, stop perception.

### ABossCharacter : AGuardCharacter  (B owns)
General Farrokh Azadi (fictional). MaxHealth 600. Dormant until `Activate()` is called (public method, C calls it when mission index reaches 2 or on throne room entry overlap). 3 phases at 100/66/33% health: P1 pistol line-trace volley every 2.5s (8 dmg, telegraph 0.8s beforehand via DrawDebugLine red beam), P2 adds one-time summon of 2 AGuardCharacter reinforcements (spawn near boss), P3 movement speed x1.5 + charge attack (35 dmg on contact, 4s cooldown).
On death: `GetGameInstance()->GetSubsystem<UMissionSubsystem>()->NotifyBossDefeated();` Public: `float GetBossHealthPct() const; bool IsActivated() const; void Activate();`

### UMissionSubsystem : UGameInstanceSubsystem  (C owns; B compiles against)
```cpp
UCLASS()
class VEILOFSUCCESSION_API UMissionSubsystem : public UGameInstanceSubsystem {
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
  UFUNCTION(BlueprintCallable) bool IsCampaignComplete() const;
  UFUNCTION(BlueprintCallable) bool IsPlayerDead() const;
};
```
Objective chain (ordered array of {FName Id, FText MissionTitle, FText Text}), completing out of order is ignored:
M0 "The Wall of Roses": Obj_EnterGrounds (reach courtyard marker), Obj_Keycard (keycard pickup), Obj_EnterPalace (palace door, requires keycard done = it is next in chain)
M1 "The Custodians Convene": Obj_Eavesdrop (stay in listening zone, 20s cumulative), Obj_StealDossier (intel pickup), Obj_ReachThroneRoom (marker)
M2 "The Peacock Throne": Obj_DefeatAzadi (via NotifyBossDefeated), Obj_Escape (extraction marker) -> IsCampaignComplete()==true.

### AInteractableActor : AActor  (C owns; A compiles against)
```cpp
UENUM() enum class EInteractableType : uint8 { Keycard, Intel, EavesdropZone, Door, Marker };
UCLASS()
class VEILOFSUCCESSION_API AInteractableActor : public AActor {
  GENERATED_BODY()
public:
  AInteractableActor();
  UPROPERTY(EditAnywhere, Category="Interact") EInteractableType Type = EInteractableType::Marker;
  UPROPERTY(EditAnywhere, Category="Interact") FName ObjectiveId;
  UPROPERTY(EditAnywhere, Category="Interact") FText Prompt;
  void Interact(class ASpyCharacter* Player);   // completes ObjectiveId via subsystem, handles type logic
  bool IsActiveObjective() const;               // true only when ObjectiveId == subsystem GetCurrentObjectiveId()
  float GetEavesdropProgress() const;           // 0..1, only meaningful for EavesdropZone
};
```
Visual: small glowing shape (sphere for pickups, flat translucent-ish box for zones/markers) that hides itself once completed. Marker + EavesdropZone auto-trigger on overlap (no Interact press needed); Eavesdrop accumulates 20s while player inside. Door type: on successful interact, moves its blocking door cube aside (the builder assigns the door mesh actor via `TWeakObjectPtr<AActor> LinkedDoor` public UPROPERTY).

### AVeilGameMode : AGameModeBase (C owns)
Constructor: `DefaultPawnClass = ASpyCharacter::StaticClass(); PlayerControllerClass = ASpyPlayerController::StaticClass(); HUDClass = AVeilHUD::StaticClass();`
BeginPlay: spawn `APalaceLevelBuilder`, call `BuildLevel()`, then teleport player pawn to `Builder->GetPlayerSpawn()`.

### APalaceLevelBuilder : AActor (C owns)
`void BuildLevel(); FTransform GetPlayerSpawn() const;`
Spawns entirely from code: ADirectionalLight (warm 4500K-ish color, pitch -50), ASkyLight, AExponentialHeightFog; 200x200m ground plane (sand color); walled compound: outer wall with gate opening, rose-garden courtyard with scattered cover blocks + hedges, palace building (grand hall with two column rows, side corridor, meeting chamber, throne room with raised gold dais at far end), interior doorways; Door-type interactable at palace entrance with its blocking cube linked; ~10 AGuardCharacter with SetPatrolPoints routes (courtyard 6, hall 2, corridor 2), 1 ABossCharacter on the dais (dormant); all AInteractableActor objectives per mission chain including extraction marker back at the gate. Colors: sandstone (.76,.70,.55), hedge green (.13,.35,.16), crimson carpet strip (.55,.06,.08), gold dais (.85,.65,.13), night-blue sky feel is optional. Walls 500uu tall, 60uu thick. Keep all patrol legs straight and unobstructed.

### AVeilHUD : AHUD (C owns)
DrawHUD(): mission title + current objective top-left; health bar bottom-left; dart count bottom-left under health; detection meter top-center (fills 0..1, turns red >0.7); eavesdrop progress bar when in zone; boss health bar top-center when boss activated and alive; dot crosshair when player aiming; full-screen states: death ("COVER BLOWN - Yasaman's mission ends here. Press R to restart") and victory ("THE SUCCESSION IS SAFE - for now."); controls hint bottom-right: "LS move  RS look  X sprint  O crouch  L2 aim  R2 fire  Square interact". Restart: C binds R key/Gamepad_Special_Right in HUD? NO - A binds Restart action (R + Gamepad_Special_Right) that calls `UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()))`.

## INPUT MAP (A implements; PS5 DualSense = standard UE gamepad keys)
Move: WASD + Gamepad_LeftX/LeftY (Axis2D with SwizzleAxis/Negate modifiers as needed)
Look: Mouse2D + Gamepad_RightX/RightY | Sprint(hold): LeftShift + Gamepad_FaceButton_Bottom
Crouch(toggle): C + Gamepad_FaceButton_Right | Aim(hold): RightMouseButton + Gamepad_LeftTrigger
Fire: LeftMouseButton + Gamepad_RightTrigger | Interact: E + Gamepad_FaceButton_Left
Restart: R + Gamepad_Special_Right
Camera: USpringArmComponent length 380, socket offset (0,60,70), bUsePawnControlRotation, aim zooms to 180 (interp speed 10). Character: bOrientRotationToMovement true except while aiming.
