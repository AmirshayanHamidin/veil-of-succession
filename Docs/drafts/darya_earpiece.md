# D3 DRAFT — Darya's Earpiece Dialogue, Cue-by-Cue (M0 / M1 / M2)

*A work of political fiction. All persons, offices, and organizations are fictitious.*

**Status:** DRAFT for the user's voice pass. Canon spine: `Docs/MISSION_SCRIPTS.md`
(all three prototype missions + reactive banks + hint ladder). Canon lines are
reproduced verbatim and unmarked; new material is tagged **[EXT]** so it can be
stripped without touching canon. Nothing in `MISSION_SCRIPTS.md` was modified.

**What this file adds:** (1) every Darya earpiece cue as an addressable, wireable
unit — stable cue ID, trigger, playback rule — ready to become the Q7 subtitle
data table; (2) a priority/interruption model so cues never talk over each other;
(3) [EXT] gap fills where the canon triggers exist but Darya has no line:
checkpoint-restart re-entries, leaving the eavesdrop zone, per-objective
directional/explicit hints for the escalation ladder.

**Delivery note (canon, binding):** Darya is dry, warm underneath, never shouts.
Even "Red, Yas. Move like it." is delivered flat — urgency through economy, not
volume. Where Yas answers inside a cue, her half-sentences are part of the same
audio unit and are included for recording continuity.

---

## CUE SCHEMA & PLAYBACK RULES [EXT — proposal, feeds Q7]

Each cue below carries: **ID** · **trigger** · **class** · **repeat rule**.
Data-table row name convention for Q7: `Cue_<ID>` (e.g. `Cue_M0_EG_START`).
Estimated subtitle duration = word count ÷ 2.5 + 0.4s tail (don't hand-author
durations; derive them).

**Priority classes (one voice channel; higher class wins):**

| Class | What | Interrupt rule |
|-------|------|----------------|
| P0 | Scripted story (cold open, Council scene, throne-room exchange, Azadi-down, closing) | Never interrupted; everything else queues or drops |
| P1 | Objective start/complete cues | Interrupts P2–P4; queues behind P0 |
| P2 | Conditional/ambient mission cues (spotlight, rotation, probe showing…) | Interrupts P3–P4 only |
| P3 | Hint ladder (30s soft / 60s directional / 90s explicit, then a full cycle of silence — canon) | Plays only if channel free |
| P4 | Reactive banks + Yas idles | Plays only if channel free ≥ a few seconds; no immediate repeats (canon) |

**Repeat rules:** `once` = fires a single time per mission run; `re-arm` = can
fire again after its condition clears + cooldown (default 20s unless noted);
`bank` = random pull, no immediate repeats (canon). On checkpoint restart,
`once` cues that already played do NOT replay — the [EXT] RETRY cues replace them.

---

## MISSION 0 — THE WALL OF ROSES

### Pre-gameplay

**`M0_CO` — cold open** · [level load / fade-in] · P0 · once
> **DARYA (earpiece):** You're on the wall's blind side. Cameras sweep left to right, thirty-second cycle. There's a reception breaking up in the east garden — stay to its edge and you're just another guest who's had too much tea.
>
> **YAS:** And if someone asks whose guest I am?
>
> **DARYA:** You're Custodian Rahimi's daughter. Nobody asks the Rahimi girl anything. That's the whole plan.
>
> **YAS:** Comforting.
>
> **DARYA:** It's not meant to be. Get to the palace doors before the Council rises. Your father is in that room, Yas. Move like you belong and no one will look twice.

### Obj_EnterGrounds (reach the courtyard marker)

**`M0_EG_START`** · [on objective start] · P1 · once
> **DARYA:** Over you go. Keep low along the hedge line — the spotlights don't reach the roses.

**`M0_EG_SPOT`** · [first spotlight sweep overlaps player] · P2 · once
> **DARYA:** Light incoming, right to left. Behind the fountain. Wait for it to pass.

**`M0_EG_GUESTS`** · [player lingers near reception guests] · P2 · re-arm (30s)
> **DARYA:** Those are guests, not guards. Don't dart them, don't spook them. A screaming undersecretary is worse than any alarm.

**`M0_EG_SPRINT`** · [player sprints inside a sentry's radius] · P2 · re-arm (20s)
> **DARYA:** Slow down. A guest strolls. Only the guilty run.

**`M0_EG_DONE`** · [on objective complete] · P1 · once
> **DARYA:** Good. You're inside the garden proper now. That's the last easy part of the night.

### Obj_Keycard (lift Captain Shirazi's keycard)

**`M0_KC_START`** · [on objective start] · P1 · once
> **DARYA:** See the captain by the cypress? Omid Shirazi. Palace guard, off the clock, sneaking a cigarette he swore to his wife he'd quit. His card opens the inner doors.
>
> **YAS:** You want me to ask him nicely.
>
> **DARYA:** I want you to take it off him without him ever knowing it's gone. Come up behind while he's still. Ease — don't grab.

**`M0_KC_TURN`** · [Shirazi turns / player mistimes approach] · P2 · re-arm (10s)
> **DARYA:** He's turning — break off, break off. Be a woman admiring the roses.

**`M0_KC_STALL`** · [stall timer near Shirazi] · P2 · once
> **DARYA:** He finishes that cigarette in ninety seconds and walks. Behind him, close, patient.

**`M0_KC_DONE`** · [on objective complete — pickpocket success] · P1 · once
> **YAS:** *(under breath)* Sorry, Captain. Give my regards to your wife.
>
> **DARYA:** Clean. He'll pat his pocket at the gate and blame the tailor. Card's live — get to the doors.

### Obj_EnterPalace (reach the palace doors)

**`M0_EP_START`** · [on objective start] · P1 · once
> **DARYA:** Main doors, top of the steps. There's an overwatch on the colonnade roof — stay under the arches and he never sees you.

**`M0_EP_OVERWATCH`** · [player enters the overwatch cone's approach] · P2 · re-arm (30s)
> **DARYA:** Rooftop spotter, eleven o'clock. He calls, he doesn't chase. Don't give him a reason to call.

**`M0_EP_HEAT`** · [detection rises past ~0.5 during this objective] · P2 · re-arm (30s)
> **DARYA:** They're getting curious. Ease off, let it cool. We are not caught until we're caught.

**`M0_EP_DONE`** · [on objective complete — card swipe] · P1 · once → fade to M1
> **DARYA:** Card's good. The doors are open.
>
> **YAS:** *(quiet)* Then I'm in the room with my father and the man trying to bury him.
>
> **DARYA:** One at a time, Yas. Listen first. Fight never.

### M0 checkpoint-restart re-entries [EXT] · [after COVER BLOWN → restart] · P1 · bank
> **DARYA:** Still with me? Good. The wall hasn't moved and neither has the plan. *(`M0_RETRY_01`)*
>
> **DARYA:** From the top, Yas. The roses will keep our secret. *(`M0_RETRY_02`)*

### M0 hint ladder — directional & explicit steps [EXT unless noted] · P3 · canon timings

Soft step (30s) is always a pull from the canon stall bank (`RX_STALL_*`, below).

| Objective | Directional (60s) | Explicit (90s) |
|-----------|-------------------|----------------|
| Obj_EnterGrounds | "Courtyard marker, past the hedges. Keep the fountain between you and the light." | "Hedge line to the fountain, wait out the sweep, then straight in to the courtyard." |
| Obj_Keycard | "The cypress on the east path. He's still smoking." | "Circle in behind Shirazi while he faces the trees, and ease the card off him." |
| Obj_EnterPalace | "Main steps, under the colonnade arches." | "Stay under the arches, out of the rooftop cone, and swipe the card at the double doors." |

---

## MISSION 1 — THE CUSTODIANS CONVENE

### Obj_Eavesdrop (hold the listening zone, 20s cumulative)

**`M1_EV_START`** · [on objective start] · P1 · once
> **DARYA:** The chamber's through the columns. You can't go in — but there's a service alcove where the ventilation carries every word. Plant your probe, stay in the zone, let it fill. Guards pass on a rotation; move between the two alcoves when they do.
>
> **YAS:** How long?
>
> **DARYA:** Long enough to hear something worth your neck. Stay in the sound.

**`M1_EV_ROT`** · [guard rotation approaches the occupied alcove] · P2 · re-arm (15s)
> **DARYA:** Rotation. Slide to the far alcove — the sound carries there too.

**`M1_EV_PROBE`** · [probe placed in the open] · P2 · re-arm (20s)
> **DARYA:** Your probe's showing. Tuck it, or a guard will find it before you find your answer.

**`M1_EV_LEAVE`** [EXT] · [player exits the listening zone mid-scene, Council audio ducking] · P2 · re-arm (15s)
> **DARYA:** You're out of the sound. Back in the alcove — every word we miss, we miss forever.

*(The Council scene itself — 20 lines, P0, heard through the vent — is not a
Darya cue; it is scripted in MISSION_SCRIPTS.md and expanded in
`Docs/drafts/eavesdrop_scene.md`, including the beat-gating proposal that
`Obj_Eavesdrop` completes only after the revelation beat has played.)*

**`M1_EV_DONE`** · [on objective complete — eavesdrop bar full] · P1 · once
> **YAS:** *(very quiet)* He's blackmailing the whole Council. And my father just dared him to do it.
>
> **DARYA:** Then the files are real, and they're here. That "inventory" is the succession dossier, Yas. If we take it, we take his whole plan out of the drawer with it.
>
> **YAS:** Where.
>
> **DARYA:** The chamber empties in seconds. He wouldn't carry it out through a hall full of Custodians. It's still in that room. Go — before the guards reset.

### Obj_StealDossier (steal the dossier from the chamber)

**`M1_SD_START`** · [on objective start] · P1 · once
> **DARYA:** They've adjourned. You've got a narrow window before the room's locked for the night. The dossier — a leather case, he wouldn't trust a screen with it.

**`M1_SD_SWEEP`** · [recess detail re-entering chamber] · P2 · once
> **DARYA:** Recess detail's coming back to sweep the room. In and out, Yas.

**`M1_SD_HINT`** · [stall timer in the chamber] · P2 · once
> **DARYA:** Check the head of the table. A man like Azadi keeps his knife where his hand rests.

**`M1_SD_DONE`** · [on objective complete — dossier picked up] · P1 · once
> **YAS:** I have it. Every name. Every debt he thinks he owns.
>
> **DARYA:** That's the ballgame. Now we walk it out quietly and none of this was ever —
>
> *(a distant door; footsteps)*
>
> **DARYA:** ...Yas. Someone's coming back for it.

### Obj_ReachThroneRoom (reach the throne room marker)

**`M1_TR_START`** · [on objective start] · P1 · once
> **DARYA:** Don't go back the way you came, the hall's filling. Through the far doors — the throne room, then out the garden side.
>
> **YAS:** The throne room's a dead end, Darya.
>
> **DARYA:** It's a door. All rooms are doors if you're brave enough. Move.

**`M1_TR_MID`** · [mid-corridor marker] · P2 · once
> **DARYA:** He'll know the case is gone the second he reaches for it. Speed now, quiet second.

**`M1_TR_COVER`** · [within the throne-room P0 exchange, after Azadi's "trying to be kind"] · P0 · once
> **DARYA (earpiece, low):** Yas — he won't let you walk. Cover, and make him work for it.

*(The rest of the throne-room exchange is Azadi/Yas — scripted P0, not earpiece.)*

### M1 checkpoint-restart re-entries [EXT] · [after COVER BLOWN → restart] · P1 · bank
> **DARYA:** Back in the hall. They're jumpy now — give them nothing to be jumpy about. *(`M1_RETRY_01`)*
>
> **DARYA:** Again, and quieter. The night's still ours to lose. *(`M1_RETRY_02`)*

### M1 hint ladder — directional & explicit steps · P3 · canon timings

| Objective | Directional (60s) | Explicit (90s) |
|-----------|-------------------|----------------|
| Obj_Eavesdrop | "Service alcove, left of the columns." *(canon)* | "Plant the probe *in* the alcove and stay put until the bar fills." *(canon)* |
| Obj_StealDossier [EXT] | "The chamber. Head of the table." | "The leather case is at Azadi's seat. Take it and walk — don't read it there." |
| Obj_ReachThroneRoom [EXT] | "Far doors, end of the corridor." | "Through the far doors into the throne room. Yes, I know it's a dead end. Go anyway." |

---

## MISSION 2 — THE PEACOCK THRONE

### Obj_DefeatAzadi (boss fight — Azadi's lines are P0 scripted, not earpiece)

**`M2_BOSS_P2`** · [phase 1→2 transition, ~66% health, after Azadi's summons] · P1 · once
> **DARYA (earpiece):** He's brought friends. Thin them out and keep off his line — he telegraphs before every volley.

**`M2_BOSS_HEART`** · [player low health / near-detection haptic beat] · P2 · bank w/ `M2_BOSS_HEART2`, re-arm (25s)
> **DARYA:** Heartbeat's up, Yas. Breathe. He's slower than his own pride.

**`M2_BOSS_HEART2`** [EXT] · [same trigger, alternate to avoid repeat] · P2 · bank
> **DARYA:** Cover. Let your heart slow before you trade with him again.

**`M2_BOSS_DONE`** · [on NotifyBossDefeated, after the Azadi-down P0 exchange] · P1 · once
> **DARYA (earpiece):** He'll be out ninety seconds, no more. Don't stay to argue with a sleeping man. Garden side, extraction at the gate. Go.

*(Hint ladder is disabled during the boss; the arena is the objective.)* [EXT]

### Obj_Escape (extraction through the gardens to the gate)

**`M2_ESC_START`** · [on objective start] · P1 · once
> **DARYA:** Back through the roses, the way you came. The garden's a different animal now — they'll have heard the noise.

**`M2_ESC_ALERT`** · [alerted-garden state active] · P2 · once
> **DARYA:** They know something happened in the palace, they just don't know it's you yet. Keep it that way.

**`M2_ESC_MID`** · [midway marker] · P2 · once
> **DARYA:** Case first, glory never. If it comes to it, you drop *me* before you drop that dossier.
>
> **YAS:** *(quiet)* I'd never drop you, Darya.
>
> **DARYA:** ...Just get to the gate, you sentimental idiot.

**`M2_ESC_SPOT`** · [spotted during escape] · P2 · re-arm (20s)
> **DARYA:** Spotlight — move, move, the fountains, like we practiced.

**`M2_ESC_DONE`** · [on objective complete — extraction marker → IsCampaignComplete] · P1 · once
> **DARYA:** Car's running. Case in the back. — You did it, Yas.
>
> **YAS:** *(getting in, looking back at the lit palace)* Did I? He was awake enough to smile.

**`M2_CLOSE`** · [within closing narration, after Yas's final VO beat] · P0 · once
> **DARYA (earpiece, soft):** Get some sleep, Yas. The succession is safe.

*(Yas's "For now." and the victory card follow — scripted, not earpiece.)*

### M2 checkpoint-restart re-entries [EXT] · [after COVER BLOWN → restart] · P1 · bank
> **DARYA:** On your feet. The case is ours until it isn't. *(`M2_RETRY_01`)*
>
> **DARYA:** The gate's still there, Yas. So are you. Move. *(`M2_RETRY_02`)*

### M2 hint ladder — Obj_Escape only [EXT] · P3 · canon timings

| Objective | Directional (60s) | Explicit (90s) |
|-----------|-------------------|----------------|
| Obj_Escape | "Garden side. Back through the roses." | "Down the garden, past the fountains — extraction is the gate. Don't stop for anything." |

---

## REACTIVE BANKS — Darya (canon, mission-agnostic, P4)

Hook the detection meter (`GetDetection`), guard state changes, and damage/down
events — not the objective chain (canon). Random pull, no immediate repeats.

**`RX_DET_UP_01–04`** · [detection crosses ~0.5]
> "They're curious, not certain. Sink back."
>
> "Someone thinks they saw a ghost. Prove them right about the ghost part — vanish."
>
> "Ease off, Yas. We cool this before it climbs."
>
> "Not caught. Curious. There's a whole world between those two words. Live in it."

**`RX_DET_RED_01–03`** · [meter red, >0.7, haptic heartbeat]
> "That's your heartbeat you're feeling. Slow it. Break their line and it fades."
>
> "One more set of eyes and it's over — and your father with it. Get gone."
>
> "Red, Yas. Move like it."

**`RX_DET_COOL_01–03`** · [detection cooling back down]
> "There. It's passing. Breathe out."
>
> "They've talked themselves out of it. Men always do."
>
> "Back to a ghost. Good."

**`RX_KILL_01–03`** · [player kills a guard — lethal, against tone]
> "...He's not getting up, Yas. That's a body now, and bodies get found."
>
> "Hide him. A sleeping man is a shift change; a dead one is an investigation."
>
> "I won't lecture you. But the morning will."

**`RX_DART_01–03`** · [player darts a guard — non-lethal, on-tone]
> "Down and dreaming. Tidy."
>
> "Nine minutes of nap. Use them."
>
> "Cleaner. Move him out of the light."

**`RX_STALL_01–03`** · [player lingers / stalls — doubles as the hint ladder's soft step]
> "The Council doesn't wait, and neither does the night."
>
> "Whatever you're admiring, admire it faster."
>
> "Clock, Yas."

**`RX_INTEL_01–02`** · [player finds optional intel/document]
> "Keep that. Every scrap is a vote we'll need later."
>
> "Photograph it, don't pocket it — a missing paper is a question, a copied one isn't."

*(Optional-objective exchanges — reception gossip, seating chart, the
half-burned letter, restraint bonus — remain in MISSION_SCRIPTS.md; they are
multi-speaker scenes, not solo Darya cues, and are already trigger-mapped there.)*

**Deliberately out of scope:** guard barks and Yas's idle one-liners (not
earpiece; see MISSION_SCRIPTS.md banks), and the Act I Bridge safe-apartment
scene (Darya in person, not on comms, and post-M2).

---

## ENGINEERING SUMMARY — cue → hook table (feeds Q7's data table)

| Cue ID(s) | Hook | Class | Repeat |
|-----------|------|-------|--------|
| M0_CO | level load / fade-in | P0 | once |
| M0_EG_START / M0_KC_START / M0_EP_START / M1_EV_START / M1_SD_START / M1_TR_START / M2_ESC_START | objective-start (`UMissionSubsystem`) | P1 | once |
| M0_EG_DONE / M0_KC_DONE / M0_EP_DONE / M1_EV_DONE / M1_SD_DONE / M2_BOSS_DONE / M2_ESC_DONE | `CompleteObjective` | P1 | once |
| M0_EG_SPOT / M0_EG_GUESTS / M0_EG_SPRINT / M0_KC_TURN / M0_KC_STALL / M0_EP_OVERWATCH / M0_EP_HEAT / M1_EV_ROT / M1_EV_PROBE / M1_EV_LEAVE / M1_SD_SWEEP / M1_SD_HINT / M1_TR_MID / M2_ESC_ALERT / M2_ESC_MID / M2_ESC_SPOT | condition volumes / timers / detection | P2 | per cue above |
| M1_TR_COVER / M2_CLOSE | inside P0 scripted scenes | P0 | once |
| M2_BOSS_P2 / M2_BOSS_HEART(2) | boss phase / low-HP | P1 / P2 | once / bank |
| Mx_RETRY_01–02 | checkpoint restart after fail | P1 | bank |
| Hint ladder rows | 30/60/90s stall per objective | P3 | canon cycle |
| RX_* banks | detection / guard state / damage events | P4 | bank |

## NEEDS-PC (nothing here is verified; wire + hear in an interactive session)

1. Confirm every hook exists or is cheap to add in `UMissionSubsystem` /
   guard AI: objective start/complete are canon-mapped; the P2 condition
   triggers (spotlight overlap, rotation proximity, probe-in-open, midway
   markers) need trigger volumes or events per the canon trigger map.
2. Playtest the priority model: does a P1 completion cue interrupting a P2
   warning feel right, or should P2 finish its sentence? (Suspected answer:
   let any cue under ~2.5s finish; hard-cut longer ones.)
3. Verify RETRY cues replace, not stack with, replayed `once` cues after
   checkpoint restart.
4. Tune re-arm cooldowns in play; the numbers above are authoring defaults.
5. Line count for VO planning: 21 canon Darya-led mission cues + 21 canon
   reactive-bank lines + 17 [EXT] lines (2×3 retries, 1 eavesdrop-leave,
   1 boss-heart alt, 9 hint-ladder steps) ≈ 38 canon / 17 [EXT] recordable
   units before the user's voice pass.
