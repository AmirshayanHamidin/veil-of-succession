# VEIL OF SUCCESSION
Third-person stealth thriller (UE 5.3, pure C++ — zero content assets required).
A work of fiction. All characters, offices, and organizations are fictitious.

You play Yasaman "Yas" Rahimi, daughter of a Council of Custodians member,
infiltrating the palace on the night the succession is decided.
3 playable missions: infiltrate the gardens, eavesdrop on the secret Council
session and steal the succession dossier, then face General Azadi at the
Peacock Throne and escape.

## Requirements
- Unreal Engine 5.3+ (Epic Games Launcher). If you use 5.4/5.5, just pick that
  version when prompted — no code changes needed in most cases.
- Visual Studio 2022 with "Game development with C++" workload (Windows).
- PS5 DualSense: plug in via USB (recognized as a standard gamepad on Windows;
  for best results install Steam or DS4Windows if buttons don't register).

## How to open and play (5 steps)
1. Right-click `VeilOfSuccession.uproject` → **Generate Visual Studio project files**.
2. Open `VeilOfSuccession.sln` → build the **Development Editor | Win64** config
   (or just double-click the .uproject and click **Yes** when asked to rebuild).
3. Editor opens. **File → New Level → Empty Level.**
4. Press **Play** (the game mode is set project-wide; the whole palace, guards,
   boss, and missions are spawned by code at runtime).
5. Full HD: play in **New Editor Window**, or package (Platforms → Windows →
   Package Project) and run the .exe — it defaults to your desktop resolution.

## Controls (DualSense / keyboard)
Move LS / WASD · Look RS / mouse · Sprint X / Shift · Crouch O / C
Aim L2 / RMB · Tranq dart R2 / LMB · Interact Square / E · Restart Options / R

## What's in the box
- `Source/` — 13 gameplay classes: stealth character, dynamic Enhanced Input,
  guard AI (sight+hearing perception, patrol/suspicious/chase), 3-phase boss,
  mission/objective subsystem, procedural palace builder, canvas HUD.
- `Docs/STORY_BIBLE.md` — canon: characters, factions, tone.
- `Docs/GDD.md` — full design for the 10-hour, 12-mission campaign this
  prototype grows into (enemy roster, gadgets, skill tree, DualSense haptics).
- `Docs/MISSION_SCRIPTS.md` — full dialogue scripts for the 3 playable missions.
- `Docs/ROADMAP.md` — honest production plan from this blockout to the full game.
- `SPEC.md` — the engineering contract the code was built against.

## Honest note on scope
This build is a playable blockout prototype (~30-60 min): geometric palace,
capsule characters, debug-line effects. The 10 hours of gameplay exist as a
complete, buildable design in Docs/GDD.md — turning it into shipped content is
a multi-year team effort (see ROADMAP.md).
