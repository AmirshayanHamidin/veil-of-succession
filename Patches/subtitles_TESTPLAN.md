# D7 — Subtitle / dialogue-line system: NEEDS-PC test plan

Companion to `Patches/subtitles.diff`. Queue item **Q7** of `AUTOMATION_AGENDA.md`.

> **This patch has never been compiled and the system has never been seen
> running.** It was written in a headless session with no Unreal Engine, no
> build, and no screenshots. Every claim below about behaviour is *intent*, not
> observation. Nothing here is verified until you build it and look at it.

---

## 0. What the patch actually contains

| | |
|---|---|
| **Data model** | `FVeilDialogueCue : FTableRowBase` — CueId, Speaker, Line, Priority, Repeat, BankId, NextCueId, CooldownSeconds, DurationOverride |
| **Registry** | 84 cues built into `VeilDialogue.cpp`, plus the identical set in `Content/Data/VeilDialogueCues.csv` for import as a `UDataTable` |
| **Channel** | One voice channel in `UMissionSubsystem`: active line + a 2-deep priority-sorted queue |
| **Timing** | `FTickableGameObject` on the subsystem; duration derived as `words / 2.5 + 0.4s`, clamped to `[1.4s, 9.0s]` |
| **Rendering** | `AVeilHUD::DrawSubtitles` — greedy word wrap at 62% screen width, speaker name above in the speaker's colour, black scrim, 0.25s fade in/out, block bottom at `SH * 0.88` |

Both the C++ registry and the CSV are generated from one source list, so they
cannot drift apart. All 84 lines were script-checked **verbatim** against
`Docs/MISSION_SCRIPTS.md` and `Docs/drafts/darya_earpiece.md` (0 mismatches)
after folding typographic punctuation to ASCII.

---

## 1. Build it

```
git apply Patches/subtitles.diff
```

Then regenerate project files and build. Two new files land in
`Source/VeilOfSuccession/`, so the `.uproject` needs a "Generate Visual Studio
project files" pass before the build, or UBT will not see them.

**If it does not compile, the most likely culprits, in order:**

1. `FTickableGameObject` on a `UGameInstanceSubsystem`. It is the standard
   idiom and the CDO is guarded (`IsTickable()` returns `!IsTemplate()`), but
   it is the single riskiest line in the patch. If UHT or the linker objects,
   the fallback is to delete the tick override and instead call a new
   `UMissionSubsystem::AdvanceDialogue(DeltaTime)` from the top of
   `AVeilHUD::DrawHUD()` — the whole system only needs *a* clock.
2. A missing include for `RETURN_QUICK_DECLARE_CYCLE_STAT` — add
   `#include "Stats/Stats.h"` to `MissionSubsystem.h` if the stat macro is
   not found.
3. `UDataTable::GetAllRows<FVeilDialogueCue>` — if the template is not visible,
   `#include "Engine/DataTable.h"` is already in `VeilDialogue.h`; check the
   include order in `MissionSubsystem.cpp`.

A Sonnet subagent reviewed all six files for compile/UHT defects and reported
none. **That is a code read, not a build.** It proves nothing.

---

## 2. What to look at — six checks, in this order

Each check names what you should see and what a failure looks like.

**T1 — Cold open fires.**
Start a fresh run. Darya's five-line cold open should play as a chain, one
line at a time, bottom centre, DARYA/YAS alternating in different colours.
*Failure:* nothing appears (tick never runs), or all five overlap (chain logic
broken), or the block sits on top of the opening title card.

**T2 — Objective cues.**
Complete `Obj_EnterGrounds`. You should hear/see `M0_EG_DONE` first, then
`M0_KC_START` and its two chained follow-ups.
*Failure:* the start cue plays before the done cue (queue ordering), or the
done cue is skipped entirely.

**T3 — Layout collisions.** This is the check most likely to fail.
Trigger a subtitle while each of these is on screen: the interact prompt
(`SH*0.68`), the takedown prompt (`SH*0.62`), the counter prompt (`SH*0.56`),
the bottom-right controls hint (`SH-34`), the health/shield stack (bottom
left), and the low-health blood vignette. The subtitle block is anchored at
`SH*0.88` and is centred, so it *should* clear all of them — but this was
laid out from source coordinates, not from a screenshot.
*Failure:* overlap with the controls hint at low resolutions, or the block
running off the bottom on an ultrawide.

**T4 — Death and restart.**
Die mid-mission. Subtitles must vanish instantly with the COVER BLOWN screen.
Restart: you should get one of the two `Mx_RETRY` lines, and you must **not**
get the mission's original opener again.
*Failure:* a subtitle drawn over the death screen, or the whole opening chain
replaying (the once-flags leaked).

**T5 — Interrupt behaviour.**
Complete an objective while a long conditional line is playing. The P1 cue
should cut it off. Then do it while a *short* line (under 2.5s) is playing —
that one should be allowed to finish first.
*Failure:* every line gets cut mid-word, which reads as a bug rather than as
urgency. See open question Q-1.

**T6 — Readability.**
Stand in front of the brightest wall in the palace with the longest line
playing (`M1_EV_START`, 44 words, capped at 9s). Can you read it?
*Failure:* the scrim is too faint, the wrap produces four or five lines, or
the 9s cap truncates the read for slow readers.

---

## 3. What is wired, and what is NOT

**Wired by this patch (fires with no further work):**

- Cold open, on `ResetCampaign()` from `AVeilGameMode::BeginPlay`
- Every objective start and complete cue, from `CompleteObjective()`
- `M2_BOSS_DONE`, via `NotifyBossDefeated()` → `CompleteObjective()`
- `M2_CLOSE`, on campaign completion
- `Mx_RETRY` re-entry banks, from `ResetToMissionStart()`
- Channel teardown on death, from `NotifyPlayerDied()`

**Present in the table but NOT wired — these are data waiting for a caller.**
Each needs exactly one line at the site named. This is deliberate: the trigger
volumes and state hooks they need do not exist yet, and inventing them
headlessly would have been guesswork.

| Cues | Call | Where |
|---|---|---|
| `RX_DET_UP_*` / `RX_DET_RED_*` / `RX_DET_COOL_*` | `PlayCueBank("RX_DET_UP")` etc. | `ASpyCharacter::AddDetection` (`SpyCharacter.cpp:343`) and the cooldown path at `SpyCharacter.cpp:251`, on threshold crossings at ~0.5 and ~0.7 |
| `RX_DART_*` / `RX_KILL_*` | `PlayCueBank("RX_DART")` / `PlayCueBank("RX_KILL")` | `AGuardCharacter::Die()` — needs a lethal/non-lethal flag the class does not currently carry |
| `RX_INTEL_*` | `PlayCueBank("RX_INTEL")` | `AInteractableActor::Interact` for `EInteractableType::Intel` |
| `RX_STALL_*` (P3, the hint ladder's soft step) | `PlayCueBank("RX_STALL")` | needs a per-objective stall timer; canon ladder is 30s / 60s / 90s then a full cycle of silence |
| `M0_EG_SPOT`, `M0_EG_GUESTS`, `M0_EG_SPRINT`, `M0_KC_TURN`, `M0_KC_STALL`, `M0_EP_OVERWATCH`, `M0_EP_HEAT`, `M1_EV_ROT`, `M1_EV_PROBE`, `M1_EV_LEAVE`, `M1_SD_SWEEP`, `M1_SD_HINT`, `M1_TR_MID`, `M2_ESC_ALERT`, `M2_ESC_MID`, `M2_ESC_SPOT` | `PlayCue("<id>")` | trigger volumes / proximity checks in `APalaceLevelBuilder`, per the trigger map in `Docs/drafts/darya_earpiece.md` |
| `M2_BOSS_P2`, `M2_BOSS_HEART` | `PlayCue` / `PlayCueBank` | `ABossCharacter` phase transition and the low-health beat |
| `M1_TR_COVER` | `PlayCue("M1_TR_COVER")` | inside the throne-room P0 exchange, which is not implemented yet |

`PlayCue` and `PlayCueBank` are safe to call every frame — they return `false`
and do nothing when a repeat or priority rule refuses.

**Also not done:** the 60s/90s directional and explicit hint-ladder steps from
the D3 draft are not in the table at all. They are per-objective, not per-cue,
and want a different data shape. Left for a follow-up item.

---

## 4. Open questions for you (taste calls I should not make)

- **Q-1 — the short-cue grace.** `ShortCueGraceSeconds = 2.5f` lets any line
  under 2.5s finish rather than be cut by a higher-priority one. The D3 draft
  flagged this as a guess. Play it both ways (set it to `0.f` to disable) and
  pick. One knob, `VeilDialogue.h`.
- **Q-2 — reading speed.** 2.5 words/sec is a subtitle-industry middle. Slower
  (2.2) is kinder; faster (3.0) keeps Darya clipped and professional. Her
  voice is dry and economical, so the fast end may suit her better.
- **Q-3 — speaker labels at all.** Currently every line is prefixed with
  DARYA / YAS. Once there is real VO, on-screen names may be redundant and
  cheapen it. Deleting them is a two-line change in `DrawSubtitles`.
- **Q-4 — the 9s clamp.** Exactly two cues hit `MaxLineSeconds`: `M1_EV_START`
  (44 words, wants 18.0s) and `M0_CO_1` (37 words, wants 15.2s). Both are
  therefore displayed at roughly half their natural reading time. Either raise
  the cap or split those two into chains — splitting is the better fix, costs
  nothing, and matches how the rest of the long beats are already authored.
- **Q-5 — DataTable or code?** The CSV is there so you can import it, edit
  lines in the editor, and call `RegisterCueTable`. But nothing calls
  `RegisterCueTable` yet, so today the built-in registry is what runs. If you
  want the DataTable path, the hook goes in `AVeilGameMode::BeginPlay`.

---

## 5. Known limitations, stated plainly

- **No audio.** This is subtitles only. There is still zero sound in the
  project (`PROJECT_STATE.md` Tier 1, item 1). The cue IDs are designed so that
  a `USoundBase` column can be added to `FVeilDialogueCue` later and the
  timing switched from derived to actual clip length.
- **Pause behaviour is untested.** `FTickableGameObject` does not tick while
  paused by default, which is what we want, but nobody has confirmed it here.
- **Alpha on `FCanvasTextItem`.** The fade multiplies the text colour's alpha.
  If canvas text ignores alpha in this engine version the lines will pop
  instead of fade — cosmetic, and the fix is a per-item blend mode.
- **Localisation.** Lines are `LOCTEXT` with the CueId as the key, so they are
  gatherable, but ASCII-folded. Typographic punctuation comes back at your
  voice pass, ideally in the DataTable rather than in C++.
- **`MissionSubsystem.cpp` roughly triples in size** (160 → ~550 lines). That
  is a lot of dialogue logic inside a mission-state class. If Q11's
  decomposition work happens, this channel is a clean candidate to lift out
  into its own subsystem.
