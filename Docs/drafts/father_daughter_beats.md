# D6 DRAFT — The Father Thread: Yasaman & Hossein across M0 / M1 / M2

*A work of political fiction. All persons, offices, and organizations are fictitious.*

**Status:** DRAFT for the user's voice pass. Canon spine: `Docs/MISSION_SCRIPTS.md`
(all three prototype missions, reactive banks, failure cards, staging notes,
optional-objective lines, idle bank). Canon lines are reproduced **verbatim and
unmarked**; new material is tagged **[EXT]** so it can be stripped without
touching canon. Nothing in `MISSION_SCRIPTS.md` was modified.

**What this file adds:**

1. A **line-anchored inventory** of every father/daughter touchpoint that already
   exists in canon (29 matching lines, grep-verified, with file line numbers), so
   the thread stops being an impression and becomes an auditable list.
2. An **eight-movement arc** mapped one-to-one onto the prototype's eight existing
   checkpoints — the objective boundaries in `UMissionSubsystem` (auto-checkpoints
   at objective boundaries, GDD §"Save & checkpoint"). No new checkpoints, no new
   objectives, no new level geometry.
3. **[EXT] beats only where the thread goes silent at a checkpoint** — seven cues
   total, plus three idles and three failure-card alternates. One checkpoint
   (`Obj_EnterPalace`) is deliberately left alone as canon-sufficient.
4. **Placement rules** so the thread never collides with the scripted P0 scenes or
   the Q3 cue priority model.

**Design constraint this file is answering (GDD Pillar 3, binding):** *"A
daughter's stakes. If Yas is caught, Hossein Rahimi hangs with her. Detection is
never abstract — the fail screen names her father."* The audit below finds that
canon honors this at **two of five** failure cards; §7 closes that gap.

---

## 0. Binding constraints honored throughout

- **Tone (STORY_BIBLE):** "Le Carré restraint over Bond spectacle." Hossein is
  weary and principled; the father thread is never sentimental and never stated
  outright by Yas in company. She says it in half-sentences or not at all.
- **Hossein never appears on screen in the prototype.** He is a voice through a
  vent (M1) and a name in other men's mouths. Every [EXT] beat below respects
  that: no cutaway, no phone call, no flashback. He is a pressure, not a
  character you meet.
- **Yas answers in half-sentences** (MISSION_SCRIPTS delivery note). Her only
  whole thoughts are the closing VO — and per the D5 draft, that device is spent
  there. So the [EXT] beats give her fragments and give Darya the sentences.
- **Darya deflects; she does not comfort.** Warm underneath, dry on top. Where a
  beat risks becoming a hug, Darya cuts it with tradecraft.
- **Restraint rule (MISSION_SCRIPTS Appendix, binding):** no line names a real
  person, state, or organization. Held throughout.

---

## 1. CANON INVENTORY — every existing father/daughter touchpoint

Grep-verified against `Docs/MISSION_SCRIPTS.md` (search: `father|daughter|Hossein|Rahimi|girl`, case-insensitive; 29 matches). Line numbers are that file's.

| Line | Checkpoint / context | Speaker | Canon text (verbatim) |
|------|----------------------|---------|------------------------|
| 18 | M0 cold open | DARYA | You're Custodian Rahimi's daughter. Nobody asks the Rahimi girl anything. That's the whole plan. |
| 22 | M0 cold open | DARYA | It's not meant to be. Get to the palace doors before the Council rises. Your father is in that room, Yas. Move like you belong and no one will look twice. |
| 75 | `Obj_EnterPalace` complete | YAS | *(quiet)* Then I'm in the room with my father and the man trying to bury him. |
| 106 | `Obj_Eavesdrop` staging | — | *…AZADI stands; the Custodians sit. HOSSEIN RAHIMI is among them.* |
| 114 | `Obj_Eavesdrop` (Council) | HOSSEIN RAHIMI | *(dry, level)* Strange, General. Every crisis you warn us of, your directorate is the first to know, and the only one to profit. You bring us intercepts no one else may verify. |
| 118 | `Obj_Eavesdrop` (Council) | HOSSEIN | I'd have you show the Council one thing it can *check*. Just once. |
| 120 | `Obj_Eavesdrop` (Council) | AZADI | *(a beat; softer still)* Hossein. We were young together. … |
| 126 | `Obj_Eavesdrop` (Council) | HOSSEIN | Borujerdi has agreed to whatever you last told him to agree to. He is a curtain, General, and you are the hand behind it. |
| 128 | `Obj_Eavesdrop` (Council) | AZADI | …I have files, Hossein. On every man at this table. Debts, sons, letters that should have been burned. … |
| 134 | `Obj_Eavesdrop` (Council) | HOSSEIN | *(after a long pause)* Put your inventory on the table, then. Let us all read our sins together and vote anyway. I would rather hang honest than rule as your puppet. |
| 136 | `Obj_Eavesdrop` (Council) | AZADI | *(almost gently)* You always did want to hang, Hossein. I never understood it. … |
| 143 | `Obj_Eavesdrop` complete | YAS | *(very quiet)* He's blackmailing the whole Council. And my father just dared him to do it. |
| 187 | `Obj_ReachThroneRoom` complete | AZADI | *(not turning at first)* The Rahimi girl. Educated abroad, they said. … |
| 195 | `Obj_ReachThroneRoom` complete | AZADI | …Your father dared me to open those files an hour ago. I'd hoped to spare him the sight of his daughter's name in them. … |
| 212 | `Obj_DefeatAzadi` start | AZADI | You know, your father and I fought in the same trenches. He came home wanting a republic. I came home wanting it to *survive*. |
| 220 | `Obj_DefeatAzadi` phase 3 | AZADI | *(first crack in the calm)* Enough theatre, girl. Stand *still.* |
| 231 | `Obj_DefeatAzadi` complete | AZADI | …You're your father's daughter after all. That's not a compliment, Yasaman. It's a diagnosis. |
| 235 | `Obj_DefeatAzadi` complete | AZADI | …You've bought your father a week. Maybe two. … |
| 266 | Closing narration | YAS (VO) | My father dared a patient man to open his files. I stole the files instead. That's the difference between us — he'd rather be right in the morning; I'd rather be gone by then. |
| 304 | Reactive bank — near-caught | DARYA | "One more set of eyes and it's over — and your father with it. Get gone." |
| 357 | Failure card — M0 garden | — | *"A guard's whistle, a name checked against a list. Somewhere, a phone rings in Hossein Rahimi's house."* |
| 359 | Failure card — with dossier | — | *"Azadi gets his files back — and a daughter to trade against a father."* |
| 367 | Staging note — Council | — | *…the camera stays on* Yas listening *(close, controlled, her father's voice reaching her)…* |
| 383 | Optional — M0 no-detection | DARYA | "Not a whisker out of place. If the whole week goes like this, your father sleeps easy." |
| 391 | Optional — M1 letter | YAS | "A letter. Half-burned. My father's handwriting." |
| 403 | Idle bank | YAS | "My father calls this room 'the cold room.' Now I know why." |
| 474 | Act I Bridge | DARYA | …he knows exactly whose daughter took it. … |

*(Lines 2 and 287 also match the search but are front-matter and localization
notes, not dialogue; counted in the 29, excluded from the thread.)*

**Reading of the inventory.** Canon is dense at M1 and M2 and thin at M0. Of the
eight checkpoints, **four** (`Obj_EnterPalace`, `Obj_Eavesdrop`,
`Obj_ReachThroneRoom`, `Obj_DefeatAzadi`) already carry a father beat at the
boundary; **four** (`Obj_EnterGrounds`, `Obj_Keycard`, `Obj_StealDossier`,
`Obj_Escape`) carry none. Notably, the thread is silent during the theft of the
case that contains his file — the single largest gap in the prototype.

---

## 2. THE ARC — eight movements on eight existing checkpoints

The thread's job is to move Hossein through four states, in order:
**credential → location → person → cost.** He starts as a thing Yas *uses* and
ends as a thing she has *spent*.

| # | Movement | Checkpoint (existing) | Hossein is… | Carried by |
|---|----------|----------------------|-------------|-----------|
| 1 | **The Name** | M0 `Obj_EnterGrounds` complete | a credential | canon (cold open) + `FD_M0_EG_01` |
| 2 | **The Instrument** | M0 `Obj_Keycard` complete | a coat she wears | `FD_M0_KC_01` |
| 3 | **The Room** | M0 `Obj_EnterPalace` complete | a location | **canon only — no [EXT]** |
| 4 | **The Voice** | M1 `Obj_Eavesdrop` (in-zone) | a person, braver than she wanted | canon (Council) + `FD_M1_ED_01` |
| 5 | **The Theft** | M1 `Obj_StealDossier` start | a file in her hands | `FD_M1_SD_01` |
| 6 | **The Name Read Back** | M1 `Obj_ReachThroneRoom` mid + complete | a hostage Azadi names aloud | canon (187/195) + `FD_M1_TR_01` |
| 7 | **The Diagnosis** | M2 `Obj_DefeatAzadi` phase 2 | a resemblance used as a weapon | canon (212/231) + `FD_M2_DA_01` |
| 8 | **The Week** | M2 `Obj_Escape` start → closing | a debt measured in days | `FD_M2_ESC_01` + canon (266) |

**The turn is at movement 5.** Everything before it, Yas spends her father.
Everything after it, she is spending *for* him — and the price is named in days.

---

## 3. PLACEMENT RULES (feeds Q7's subtitle data table)

Follows the Q3 cue model (`Docs/drafts/darya_earpiece.md` §"Cue schema &
playback rules"). Data-table row convention: `Cue_<ID>`, e.g.
`Cue_FD_M0_EG_01`. No existing cue ID in Q3 or Q5 begins with `FD_`.

- **One father beat per checkpoint, maximum.** The thread is punctuation, not
  a subplot. If a beat cannot fit without displacing a canon line, it is cut,
  not shortened.
- **Class P2 by default**, so a father beat never interrupts a P0 scripted scene
  or a P1 objective cue — it queues behind them. The one exception is
  `FD_M1_ED_01`, which is P0 because it sits inside the Council scene.
- **Never during combat except `FD_M2_DA_01`**, which is deliberately placed in
  the phase-2 lull and drops entirely if the channel is busy.
- **`once` per mission run.** On checkpoint restart these do not replay — Q3's
  `Mx_RETRY_*` bank covers re-entry. A player who dies twice does not hear the
  father thread twice.
- **Offset from the canon cue it follows** (given per beat below) so the two do
  not read as one speech.
- **Strippable.** Deleting every `FD_*` row leaves canon intact and playable.

---

## 4. THE BEATS — M0, THE WALL OF ROSES

### Movement 1 — The Name · `Obj_EnterGrounds` complete

*Canon at this boundary:* **DARYA:** Good. You're inside the garden proper now. That's the last easy part of the night.

**`FD_M0_EG_01`** [EXT] · [on `Obj_EnterGrounds` complete, +5s] · P2 · once

> **YAS:** *(quiet)* He came in through the front, you know. Invited. A driver, a coat, his name on a list at the gate.
>
> **DARYA:** Different door, same house. — Hedge line, Yas.

*Why here:* the two entrances are the game's two identities (GDD Pillar 1 —
legitimacy vs. shadow) stated once, in ten seconds, without a lecture. Darya
refuses the sentiment and gives a direction instead, which is her whole method.

---

### Movement 2 — The Instrument · `Obj_Keycard` complete

*Canon at this boundary:* **YAS:** *(under breath)* Sorry, Captain. Give my regards to your wife. / **DARYA:** Clean. He'll pat his pocket at the gate and blame the tailor. Card's live — get to the doors.

**`FD_M0_KC_01`** [EXT] · [on `Obj_Keycard` complete, +6s, after the canon pair] · P2 · once

> **YAS:** If they question him, they'll ask who was standing close.
>
> **DARYA:** And somebody will say: only the Rahimi girl. Then they'll stop writing. — That name is the best coat you own, Yas. Wear it. Don't apologize to it.
>
> **YAS:** *(a beat)* He'd hate hearing it put that way.
>
> **DARYA:** He'd hate the whole evening. Doors.

*Why here:* it converts the cold open's "Nobody asks the Rahimi girl anything"
from a plan into a cost — a captain will absorb what her name deflects. This is
also the seed of the Shirazi-investigation thread in the D5 BITTER ending
(`M2_END_BIT_01`); if the user cuts that ending, this beat still stands alone.

---

### Movement 3 — The Room · `Obj_EnterPalace` complete

**No [EXT] beat. Canon is sufficient and should not be crowded.**

> **YAS:** *(quiet)* Then I'm in the room with my father and the man trying to bury him.
>
> **DARYA:** One at a time, Yas. Listen first. Fight never.

*Why nothing:* this is the strongest father exchange in M0 and it is already
doing movement 3's entire job — he stops being a credential and becomes a place.
Adding to it would be accumulation. Deliberately left alone.

---

## 5. THE BEATS — M1, THE CUSTODIANS CONVENE

### Movement 4 — The Voice · `Obj_Eavesdrop`, in-zone

The Council scene is P0 and canon-locked. The one thing it lacks is the instant
*before* she recognizes him — canon's staging note has the camera on her, but she
has no reaction line, so the recognition is carried entirely by performance.

**`FD_M1_ED_01`** [EXT] · [in-zone, 1.5s before HOSSEIN's first line (script line 114)] · **P0** · once

> **YAS:** *(barely)* That's —
>
> **DARYA:** *(low)* I know. Stay in the sound.

*Why here:* four words and a half-word. "Stay in the sound" is Darya's own canon
phrase (script line 94), reused deliberately — she answers an emotional moment
with the same instruction she gave for a technical one, which is the character.
If the user prefers pure performance, this cue strips without a seam.

---

### Movement 5 — The Theft · `Obj_StealDossier` start · **the turn**

*Canon at this boundary:* **DARYA:** They've adjourned. You've got a narrow window before the room's locked for the night. The dossier — a leather case, he wouldn't trust a screen with it.

**`FD_M1_SD_01`** [EXT] · [on `Obj_StealDossier` start, +7s] · P2 · once

> **YAS:** His file's in there. My father's.
>
> **DARYA:** Yes.
>
> **YAS:** *(a beat)* And mine?
>
> **DARYA:** …Don't ask me that in a corridor. — Head of the table, Yas. Go.

**ALT — `FD_M1_SD_01a`** *(stronger, but see Canon flag 2)*

> **YAS:** His file's in there. My father's. And mine.
>
> **DARYA:** *(a beat)* Take the case. Read it later, or never. Tonight you're a pair of hands.

*Why here:* this is the prototype's largest father-silence, and it sits on the
one action that fuses the political and the personal — the case she is stealing
to save the republic is also the case with her family in it. The primary version
leaves the question **open**, so Azadi's canon line at 195 ("the sight of his
daughter's name in them") lands as the *answer*. The ALT closes it early and
makes 195 a confirmation instead of a reveal — a real structural change, flagged
below rather than assumed.

---

### Movement 6 — The Name Read Back · `Obj_ReachThroneRoom`, mid-corridor

The complete-boundary here is the throne-room exchange (P0, seven canon lines,
untouchable). The beat goes mid-corridor instead.

*Canon mid-corridor:* **DARYA:** He'll know the case is gone the second he reaches for it. Speed now, quiet second.

**`FD_M1_TR_01`** [EXT] · [mid-corridor, after the canon mid cue, +6s] · P2 · once

> **YAS:** He's still in the building. My father.
>
> **DARYA:** He's in a car by now, arguing with his driver about the long way home.
>
> **YAS:** You don't know that.
>
> **DARYA:** No. — Corridor, Yas.

*Why here:* Darya tells a small kind lie, is caught inside four seconds, does not
defend it, and redirects. That single "No." is the warmest thing she does in the
prototype, and it costs one word. It also sets the audience up for Azadi naming
Hossein forty seconds later — the last person to mention her father before the
villain does is the person trying to protect her from thinking about him.

---

## 6. THE BEATS — M2, THE PEACOCK THRONE

### Movement 7 — The Diagnosis · `Obj_DefeatAzadi`, phase 2

Canon opens the fight with Azadi invoking the trenches (212) and closes it with
"You're your father's daughter after all… It's a diagnosis" (231). The gap is
that nothing in the game ever names what he is *doing* with that.

**`FD_M2_DA_01`** [EXT] · [phase 2, ~50% boss health, only if voice channel free ≥3s] · P2 · once · **drops if busy**

> **DARYA:** *(flat)* He wants you thinking about your father. That's a weapon too, Yas. Put it down and shoot.

*Why here:* one line, delivered flat, that reframes every Azadi father-reference
in the fight as tradecraft rather than sentiment — and gives the player
permission to stop absorbing it. Drops silently if anything else is playing;
never fires in phase 3, which belongs to Azadi's crack in the calm.

---

### Movement 8 — The Week · `Obj_Escape` start

*Canon just before:* **AZADI:** …You've bought your father a week. Maybe two. …
*Canon at this boundary:* **DARYA:** Back through the roses, the way you came. The garden's a different animal now — they'll have heard the noise.

**`FD_M2_ESC_01`** [EXT] · [on `Obj_Escape` start, +6s] · P2 · once

> **YAS:** *(moving)* A week, he said. Maybe two.
>
> **DARYA:** Then that's what we spend. — Roses, Yas.

*Why here:* it takes Azadi's parting insult and makes it the mission's unit of
measurement, six seconds after he says it, while she runs. Nothing is resolved;
the arithmetic just becomes hers. The canon midway beat
("I'd never drop you, Darya") and the closing VO (266) are untouched and
complete the movement.

---

## 7. SYSTEMIC LAYER — where the thread already lives outside the checkpoints

The checkpoint beats above are the spine. Three canon systems carry the father
thread *between* checkpoints, and two of them have gaps worth closing.

### 7a. Failure cards — GDD Pillar 3 is honored at 2 of 5

GDD Pillar 3 states plainly: *"the fail screen names her father."* Canon delivers
that at two of the five variants (M0 garden, caught-with-dossier). The other
three do not mention him. [EXT] alternates below keep each canon line intact and
extend it — a strip of everything after the canon sentence restores canon exactly.

| Variant | Canon line (verbatim, kept) | [EXT] extension |
|---------|------------------------------|-----------------|
| Caught eavesdropping (M1) | *"They found the probe before she found her answer."* | *"There are two Rahimis in the building tonight, and only one of them can still claim to be surprised."* |
| Killed by Azadi (M2) | *"He said he was trying to be kind. He wasn't lying, exactly."* | *"In the morning a Custodian will be told what his daughter was."* |
| Caught during extraction (M2) | *"So close to the gate. The succession decides itself now, without her in the room."* | *"Her father will be in the room. He will vote knowing precisely what it cost."* |

*(The two canon variants that already name him need nothing.)*

### 7b. Restraint scoring — the "father sleeps easy" hook exists only in M0

Canon's M0 no-detection bonus is scored in father terms: *"Not a whisker out of
place. If the whole week goes like this, your father sleeps easy."* M1 and M2
have no equivalent, so the game's one mechanical link between *how* you play and
*what it costs him* fires once and never returns. [EXT] parallels, same slot:

- **M1 — no detection through the chamber:** **DARYA:** "Twelve men adjourned and not one of them will ever know you were breathing on the other side of that wall. Including the one you'd have to explain it to."
- **M2 — spare all reinforcements** *(layers with the canon restraint line, does not replace it)*: **DARYA:** "Two of his men will wake up with headaches and a story nobody believes. That's the version of you your father would recognize."

These tie directly into the optional restraint-tally layer proposed in the D5
endings draft — a player who earns all three hears the ending's restraint
variant having been told three times what it was for.

### 7c. Idle bank — three additions

Canon has one father idle (403, "the cold room"). Three more, same register —
low-frequency, dry, P4, no immediate repeats:

- **`FD_IDLE_01`** [EXT] — "He'll hear about tonight from someone else. That's the kindest version of it."
- **`FD_IDLE_02`** [EXT] — "I could end this with one phone call and my own voice. That's the part nobody warns you about."
- **`FD_IDLE_03`** [EXT] — "Twenty-nine years of his name opening doors, and the first one I ever needed was a service alcove."

*(`FD_IDLE_03` reads best in M1; if the bank is not mission-gated, it can be cut
or the reference generalized.)*

---

## 8. CUE INDEX (for Q7's data table)

| Cue ID | Checkpoint / trigger | Class | Repeat | Lines |
|--------|---------------------|-------|--------|-------|
| `FD_M0_EG_01` | `Obj_EnterGrounds` complete +5s | P2 | once | 2 |
| `FD_M0_KC_01` | `Obj_Keycard` complete +6s | P2 | once | 4 |
| *(none)* | `Obj_EnterPalace` complete | — | — | canon-sufficient |
| `FD_M1_ED_01` | in-zone, −1.5s before script line 114 | P0 | once | 2 |
| `FD_M1_SD_01` | `Obj_StealDossier` start +7s | P2 | once | 4 |
| `FD_M1_SD_01a` | *(ALT for `FD_M1_SD_01`)* | P2 | once | 2 |
| `FD_M1_TR_01` | mid-corridor, after canon mid cue +6s | P2 | once | 4 |
| `FD_M2_DA_01` | `Obj_DefeatAzadi` phase 2, ~50% HP, channel free | P2 | once, drops if busy | 1 |
| `FD_M2_ESC_01` | `Obj_Escape` start +6s | P2 | once | 2 |
| `FD_FAIL_ED` / `FD_FAIL_KILLED` / `FD_FAIL_ESC` | failure card text, per variant | — | on fail | 3 |
| `FD_REST_M1` / `FD_REST_M2` | no-detection M1 / spare-all M2 | P2 | once | 2 |
| `FD_IDLE_01–03` | idle bank | P4 | bank | 3 |

**Totals:** 7 checkpoint cues (+1 ALT), 3 failure extensions, 2 restraint cues,
3 idles = **15 new addressable units, 27 new spoken lines.** Against the
prototype's ~140-line estimate (MISSION_SCRIPTS §localization) this is roughly a
**19% line-count increase**, ~54 additional recorded takes across EN/FA.

---

## 9. CANON FLAGS — approve or cut; automation does not decide these

1. **Hossein arrived openly and left by car** (`FD_M0_EG_01`, `FD_M1_TR_01`).
   Canon establishes he is inside the palace at a secret recess-session, and that
   the session "never happened." Whether a Custodian arrives at such a session
   *invited, with a driver, on a list* is an invention — a good one for the
   legitimacy/shadow contrast, but it makes the secret session semi-public. If
   the Council convened covertly, both beats need rewording.

2. **Does Yas know her own name is in the dossier before Azadi says it?**
   (`FD_M1_SD_01` vs. `FD_M1_SD_01a`.) Canon's line 195 plays as a reveal. The
   primary version preserves that; the ALT makes it a confirmation and gives Yas
   the knowledge earlier, which strengthens movement 5 and weakens the throne
   room. **This is the single largest structural choice in the file** and it
   interacts with the D4 confrontation draft, whose Version B already has Azadi
   working the name. Pick one and it should propagate.

3. **"That name is the best coat you own"** (`FD_M0_KC_01`). Darya has never been
   this explicit about instrumentalizing Hossein. Canon Darya says the same thing
   obliquely ("Nobody asks the Rahimi girl anything. That's the whole plan."). The
   [EXT] version is colder. If Darya's warmth should be nearer the surface in M0,
   cut the second sentence and keep "Then they'll stop writing."

4. **The failure-card extensions (§7a) lengthen a fixed UI card.** The card text
   is described as fixed with a small italic line beneath; three of the five
   italic lines would roughly double in length. This is a layout question as much
   as a writing one — it needs eyes on the actual widget in an engine session.

5. **`FD_IDLE_02` ("one phone call and my own voice")** implies Yas could warn or
   destroy her father at will, and chooses not to, all game. Canon never gives her
   that option explicitly. It is the most loaded line in the file and the easiest
   to cut.

6. **`FD_M1_ED_01` is P0 inside a P0 scene.** It is the only [EXT] cue in this
   file that inserts into a scripted sequence rather than around one. If the
   Council scene's 20-second budget is tight, this is the first thing to go.

---

## 10. NEEDS-PC — nothing here is verified; test plan for a live session

This file changes no code and no canon. It becomes real only when someone with
the engine wires and plays it. Required checks, in order:

1. **Timing headroom.** Every beat is specified as an offset from a canon cue.
   Verify each offset actually fits before the next canon cue fires — especially
   `FD_M0_KC_01` (+6s into the walk to the doors) and `FD_M1_SD_01` (+7s into a
   window canon describes as "narrow"). If a player is fast, these truncate.
   Measure against real traversal times, not the script.
2. **`FD_M1_ED_01` inside the 20-second eavesdrop.** Confirm the Council scene
   still completes within the bar's fill time with two extra lines in front of
   `HOSSEIN`'s entrance, including when the player leaves and re-enters the zone.
3. **`FD_M2_DA_01` channel arbitration.** Confirm it genuinely drops (not queues)
   when boss/reinforcement barks are active, and that it can never straddle the
   phase-3 transition at 33%.
4. **Checkpoint restart behavior.** Die once at each of the eight boundaries and
   confirm no `FD_*` cue replays on restart, and that Q3's `Mx_RETRY_*` bank
   covers the re-entry instead.
5. **Failure-card layout.** Screenshot all five cards with the §7a extensions at
   the shipping resolution and confirm no overflow or clipping.
6. **Subtitle duration.** Derive durations per Q3's rule (words ÷ 2.5 + 0.4s);
   do not hand-author. `FD_M0_KC_01` is the longest unit at four lines.

**No line in this file has been heard, timed, or seen on screen. Treat every
offset as a proposal, not a measurement.**

---

*Draft ends. Canon untouched: `Docs/MISSION_SCRIPTS.md`, `Docs/STORY_BIBLE.md`,
`Docs/GDD.md` unmodified by this run.*
