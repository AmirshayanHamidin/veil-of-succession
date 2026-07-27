# THE 10-MINUTE LOOP — framework for every new chat on this project
Paste the "SESSION OPENER" below into any fresh chat. It gets a cold assistant
productive in under a minute and prevents every failure mode we hit.

===============================================================================
## SESSION OPENER (copy-paste this)
===============================================================================
> Continue my Unreal game "Veil of Succession".
> Read these first: C:\Users\hamid\Documents\VeilOfSuccession\PROJECT_STATE.md
> and WORK_FRAMEWORK.md in the same folder.
> Today's task: <ONE THING FROM THE BACKLOG>
> Follow the pipeline in PROJECT_STATE section 3 exactly, including the
> verify-the-copy step and the screenshot proof at the end.

===============================================================================
## THE LOOP (one cycle ≈ 10 minutes)
===============================================================================
  1. PICK ONE     — a single item. Not three. One.
  2. MEASURE      — read the actual code/asset before changing it. Never assume.
  3. CHANGE       — smallest edit that delivers the item.
  4. VERIFY WRITE — re-read the file; confirm the new string is present.
  5. SYNC + VERIFY— copy to Documents\...; findstr to prove it landed.
  6. BUILD        — must say "Result: Succeeded" AND take >3 seconds.
  7. PACKAGE      — only when you want a new playable exe (~40s).
  8. LOOK AT IT   — launch, screenshot, confirm with EYES. No "should work".
  9. LOG          — one line in CHANGELOG.md: what changed, what proved it.

### The three rules that came from real failures
  RULE 1  A fix isn't done until a screenshot shows it. Compiling ≠ working.
  RULE 2  If the build takes ~1 second, nothing compiled. Your sync failed.
  RULE 3  Close the running game before building, always.

===============================================================================
## BACKLOG — ordered. Take the top unchecked item each session.
===============================================================================

### PHASE A — AUDIO (biggest perceptual win; ~6-8 sessions)
[ ] A1  Add a `UVeilAudio` component; wire USoundBase* properties on the spy
[ ] A2  Source ~12 free CC0 sounds (freesound.org / Fab): pistol, rifle, tranq,
        knife impact, footstep x2, detection sting, alert, death, door, ambience
[ ] A3  Import to Content/Audio; hook gunfire + dry-fire
[ ] A4  Footsteps (anim notify or timer, tied to walk/crouch/sprint state)
[ ] A5  Detection stings: rising cue on suspicion, hard sting on chase
[ ] A6  Knife/takedown/death impacts + guard barks (even simple grunts)
[ ] A7  Ambient night bed + interior/exterior variation
[ ] A8  Score: santur + electronic drone; stealth vs combat layer swap

### PHASE B — CHARACTER ART (~4-6 sessions, needs downloads)
[ ] B1  MetaHuman Creator: build Yasaman (29, Iranian, dark hair, spy elegance)
[ ] B2  Build Azadi (58, weathered general), Hossein (father), Darya (handler)
[ ] B3  Import via Quixel Bridge; retarget ABP_Unarmed onto MetaHuman skeleton
[ ] B4  Guard variants: 3 body/uniform variations so they aren't clones
[ ] B5  Swap SKM_Quinn/Manny references in SpyCharacter/GuardCharacter ctors

### PHASE C — ENVIRONMENT ART (~6-10 sessions)
[ ] C1  Fab/Quixel: marble floors, sandstone walls, carpet, wood doors
[ ] C2  Replace PalaceLevelBuilder primitives with static meshes (keep the
        procedural layout; swap the meshes it spawns)
[ ] C3  Rose garden: foliage, hedges, fountain
[ ] C4  Throne room set piece: dais, columns, chandeliers, the Peacock Throne
[ ] C5  Lighting pass: practical lights, moonlight shafts, volumetric fog
[ ] C6  Post-process: colour grade for the night-infiltration mood

### PHASE D — STORY (your part; do it in parallel, it needs no build)
[ ] D1  Rewrite the story bible in YOUR voice — the version you actually want
[ ] D2  The eavesdrop scene: what the Council actually says (the game's heart)
[ ] D3  Darya's earpiece dialogue for all 3 missions
[ ] D4  Yasaman & Azadi's confrontation before the boss fight
[ ] D5  Ending: what "the succession is safe — for now" costs her
[ ] D6  Father/daughter beats — the emotional spine (TLOU's real lesson)
[ ] D7  Wire the text into MissionSubsystem + a subtitle system

### PHASE E — CONTENT & SYSTEMS (~10+ sessions)
[ ] E1  Save/load (USaveGame: mission, weapons, health)
[ ] E2  Distraction throwable (coin/bottle — the missing Hitman verb)
[ ] E3  Difficulty modes (detection speed, damage, ammo scarcity)
[ ] E4  Missions 4-6 from the GDD (safehouse, ministry archives, seminary)
[ ] E5  Missions 7-9 (state TV, prison break, mountain retreat)
[ ] E6  Missions 10-12 (counter-coup, palace finale, epilogue)
[ ] E7  Enemy archetypes: rifleman, captain, dog/drone
[ ] E8  Cutscene camera + dialogue delivery

### PHASE F — POLISH & SHIP
[ ] F1  Refactor SpyCharacter into components (923 lines is too many)
[ ] F2  Cache HUD actor lookups (6 world scans/frame today)
[ ] F3  UMG UI pass (replace canvas text)
[ ] F4  DualSense via Steam Input wrapper
[ ] F5  Main menu, options, pause screen
[ ] F6  Full playthrough QA + Shipping-config build
[ ] F7  Store page / itch.io / demo trailer

===============================================================================
## WHAT I (CLAUDE) CAN AND CANNOT DO — read this before asking
===============================================================================
CAN, fully autonomous: all C++ gameplay code, build + package pipeline,
level-layout code, HUD, AI, tuning, docs, story drafting, benchmark audits,
launching and screenshotting the game to verify.

CANNOT without you: download assets (MetaHuman, Fab, sounds) — those need your
account and clicks; make taste calls (voice of the story, art direction);
run overnight (my session ends when the chat does); create mocap/audio content
that doesn't exist.

BEST DIVISION OF LABOUR: you download and choose; I wire, build, verify, ship.
A session where you arrive with assets already downloaded is worth five without.

===============================================================================
## HONEST TIMELINE
===============================================================================
At ~1 backlog item per session: Phase A+B (audio + faces) ≈ 12 sessions and the
game stops feeling like a prototype. All phases ≈ 40-60 sessions to a complete,
polished 3-mission vertical slice with your story — which is the right shape for
a demo. The full 12-mission campaign at Last of Us fidelity is a studio-years
project; the vertical slice is the honest, achievable, genuinely impressive goal.
