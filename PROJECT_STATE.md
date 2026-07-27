# VEIL OF SUCCESSION — Project State (v1.1, 2026-07-20)
A third-person stealth-action thriller. Political FICTION: all characters,
offices and organizations are invented. Built in Unreal Engine 5.7.4, pure C++.

===============================================================================
## 1. WHAT EXISTS TODAY
===============================================================================

### It is a real, shippable product
- Standalone game: `C:\Users\hamid\Documents\VeilGame\Windows\VeilOfSuccession.exe`
  Runs with no editor, no Epic launcher. Zip that `Windows` folder to share it.
- Source project: `C:\Users\hamid\Documents\VeilOfSuccession`  (this is the ONE
  that builds — the Claude outputs folder is a mirror, not the build target)
- ~4,500 lines of C++ across 14 classes. Zero Blueprint logic.
- Benchmark score: 82/100 against a study of the top-grossing games.

### Systems that work
MOVEMENT   walk / sprint (L3) / crouch (O) / jump (X) / dodge-roll with 0.4s
           i-frames (X key / R3), aim-slow, sprint FOV kick
COMBAT     3-hit knife combo chain w/ lunge, parry-counter on a 0.45s enemy
           windup (55 dmg + 1.6s stagger), behind-the-back silent kill with
           death animations, hit-stop slow-mo on kills/counters
WEAPONS    Tranq (silent, 50 dmg, regenerates, headshots one-shot),
           Guard Pistol (95 dmg, 0.85s between shots), Rifle (16 dmg, full-auto
           ~9/sec, wide spread). Guards drop weapons; wheel/D-pad to switch.
DEFENSE    Ballistic shield (L1) absorbs before health, drains + recharges
STEALTH    Gradual suspicion (not binary), crouch visibility 0.45x, noise from
           sprint/gunfire, breadcrumb search when you break line of sight,
           wall-slide steering, no vision/damage through walls, corpse discovery
CAMERA     Over-shoulder, ADS aim pose, target lock-on (Tab / D-pad Up) with
           brackets, damage-direction indicator, low-health screen pulse
MISSIONS   3 acts, FOLLOW waypoints w/ distance, auto-collect checkpoints,
           button prompts, mission title splashes, opening title card
BOSS       General Azadi: 3 phases w/ announcements, telegraphed volleys,
           line-of-sight gated (no shooting through walls), reinforcements,
           charge attack, immune while dormant, cannot be stealth-killed
FLOW       Death respawns at MISSION start (not campaign), doors you already
           opened stay open, guards you already killed stay dead on the floor

### Documentation in `Docs/`
- `STORY_BIBLE.md`   — canon: Yasaman Rahimi, General Azadi, the Qanat, tone
- `GDD.md`           — full 12-mission / 10-hour campaign design (615 lines)
- `MISSION_SCRIPTS.md` — dialogue scripts for the 3 built missions
- `ROADMAP.md`       — honest production plan to a full game
- `BENCHMARKS_100.md` — 100 benchmarks from top-grossing games + our audit
- `QA_BENCHMARKS.md` — the 15-bug ledger with root causes

===============================================================================
## 2. WHAT IS MISSING (the honest gap to "a real game like The Last of Us")
===============================================================================
Ranked by impact on the player's experience.

TIER 1 — THE BIG THREE (this is what separates it from a demo)
 1. AUDIO. There is literally none. Zero sound references in 4,500 lines — in a
    STEALTH game whose systems already simulate hearing. Gunshots, footsteps,
    detection stings, knife impacts, ambient night, a santur-and-electronics
    score. Biggest single perceptual win available.
 2. CHARACTER ART. Grey mannequins. Needs MetaHuman faces/bodies for Yasaman,
    Azadi, Hossein, Darya + guard variants.
 3. ENVIRONMENT ART. The palace is a blockout of coloured boxes. Needs Quixel/
    Fab: marble, carpets, rose garden, chandeliers, the Peacock Throne.

TIER 2 — FEEL & CONTENT
 4. Animations the pack lacks: crouch-walk, sprint, synced two-character
    takedowns, real reloads, weapon-specific locomotion.
 5. Weapon models (currently primitive boxes).
 6. Missions 4-12 from the GDD (only 3 of 12 are built).
 7. Save system (single-session runs only).
 8. Difficulty modes.

TIER 3 — POLISH
 9. Distraction throwable (the classic Hitman coin), 10. UI art pass beyond
 canvas text, 11. DualSense support (needs Steam Input or DS4Windows — UE on
 Windows is XInput-only), 12. Cutscenes/cinematic camera, 13. Performance pass
 (HUD does 6 world scans/frame), 14. Refactor SpyCharacter (923-line god class).

===============================================================================
## 3. HOW THE BUILD PIPELINE WORKS (learned the hard way)
===============================================================================
1. CLOSE THE GAME AND EDITOR FIRST. A running .exe or editor locks the DLL and
   the build fails with a Live Coding mutex error. Alt+F4 the game window.
2. Edit source in the Claude outputs mirror.
3. COPY to `C:\Users\hamid\Documents\VeilOfSuccession\Source\VeilOfSuccession\`
4. VERIFY the copy landed (findstr for a unique new string). Non-negotiable —
   a "fix" that never reached the build copy cost hours twice.
5. BUILD: `"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat"
   VeilOfSuccessionEditor Win64 Development -project="...\VeilOfSuccession.uproject" -WaitMutex`
   Check `%LOCALAPPDATA%\UnrealBuildTool\Log.txt` for "Result: Succeeded".
   A ~1s build time means NOTHING COMPILED — the sync failed. 5-30s = real.
6. PACKAGE: `RunUAT.bat BuildCookRun -project=... -platform=Win64
   -clientconfig=Development -build -cook -stage -pak -archive
   -archivedirectory="C:\Users\hamid\Documents\VeilGame" -unattended -noP4`
7. TEST by launching the exe and LOOKING at it. Screenshot proof, not inference.

### Hard-won engine gotchas
- Additive animations (MM_Pistol_Fire) collapse the rig to a T-pose if played
  as single-node. VERIFY BY SCANNING THE .uasset for AAT_RotationOffsetMeshSpace
  before using any animation. MF_Pistol_Idle_ADS and all Unarmed/Attack anims
  are non-additive and safe.
- Default Pawn capsules IGNORE ECC_Visibility — traces pass through. Guard
  meshes are set to block it (this is what enables per-bone headshots).
- SpawnActor runs BeginPlay BEFORE the spawner sets properties; call a
  ConfigureVisual()-style re-init afterward.
- GameInstance subsystems survive OpenLevel — reset state in GameMode::BeginPlay.
- UE modal dialogs often ignore synthetic clicks; the human clicks those.
