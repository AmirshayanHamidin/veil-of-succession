# VEIL OF SUCCESSION — Mission Scripts (Prototype: M0 / M1 / M2)
*A work of political fiction. All persons, offices, and organizations are fictitious. These are the narrative scripts for the three playable prototype missions. Earpiece = "DARYA" (handler). VO = Yasaman "Yas" Rahimi unless noted.*

**Delivery notes.** Le Carré restraint: everything underplayed. Darya is dry, warm underneath, never shouts. Yas answers in half-sentences. Azadi is soft-spoken and reasonable — the calmer he is, the worse it is. Lines are keyed to the prototype's objective chain (per SPEC): M0 Obj_EnterGrounds / Obj_Keycard / Obj_EnterPalace; M1 Obj_Eavesdrop / Obj_StealDossier / Obj_ReachThroneRoom; M2 Obj_DefeatAzadi / Obj_Escape. Triggers: **[on objective start]**, **[on objective complete]**, **[ambient/on condition]**.

---

## MISSION 0 — THE WALL OF ROSES

*Palace rose-garden, night. Fountains, hedges, sandstone colonnade. A minor reception is winding down; a few officials linger with tea.*

### Cold open (pre-gameplay, black screen → fade to the wall)

> **DARYA (earpiece):** You're on the wall's blind side. Cameras sweep left to right, thirty-second cycle. There's a reception breaking up in the east garden — stay to its edge and you're just another guest who's had too much tea.
>
> **YAS:** And if someone asks whose guest I am?
>
> **DARYA:** You're Custodian Rahimi's daughter. Nobody asks the Rahimi girl anything. That's the whole plan.
>
> **YAS:** Comforting.
>
> **DARYA:** It's not meant to be. Get to the palace doors before the Council rises. Your father is in that room, Yas. Move like you belong and no one will look twice.

### Objective 1 — Obj_EnterGrounds (reach the courtyard marker)

**[on objective start]**
> **DARYA:** Over you go. Keep low along the hedge line — the spotlights don't reach the roses.

**[ambient, first spotlight sweep]**
> **DARYA:** Light incoming, right to left. Behind the fountain. Wait for it to pass.

**[ambient, if player lingers near reception guests]**
> **DARYA:** Those are guests, not guards. Don't dart them, don't spook them. A screaming undersecretary is worse than any alarm.

**[ambient, if player sprints near a sentry]**
> **DARYA:** Slow down. A guest strolls. Only the guilty run.

**[on objective complete]**
> **DARYA:** Good. You're inside the garden proper now. That's the last easy part of the night.

### Objective 2 — Obj_Keycard (lift Captain Shirazi's keycard)

**[on objective start]**
> **DARYA:** See the captain by the cypress? Omid Shirazi. Palace guard, off the clock, sneaking a cigarette he swore to his wife he'd quit. His card opens the inner doors.
>
> **YAS:** You want me to ask him nicely.
>
> **DARYA:** I want you to take it off him without him ever knowing it's gone. Come up behind while he's still. Ease — don't grab.

**[ambient, if Shirazi turns / player mistimes approach]**
> **DARYA:** He's turning — break off, break off. Be a woman admiring the roses.

**[ambient, hint if player stalls]**
> **DARYA:** He finishes that cigarette in ninety seconds and walks. Behind him, close, patient.

**[on objective complete — pickpocket success]**
> **YAS:** *(under breath)* Sorry, Captain. Give my regards to your wife.
>
> **DARYA:** Clean. He'll pat his pocket at the gate and blame the tailor. Card's live — get to the doors.

### Objective 3 — Obj_EnterPalace (reach the palace doors)

**[on objective start]**
> **DARYA:** Main doors, top of the steps. There's an overwatch on the colonnade roof — stay under the arches and he never sees you.

**[ambient, near the overwatch cone]**
> **DARYA:** Rooftop spotter, eleven o'clock. He calls, he doesn't chase. Don't give him a reason to call.

**[ambient, if detection rises]**
> **DARYA:** They're getting curious. Ease off, let it cool. We are not caught until we're caught.

**[on objective complete — reaches doors, card swipe]**
> **DARYA:** Card's good. The doors are open.
>
> **YAS:** *(quiet)* Then I'm in the room with my father and the man trying to bury him.
>
> **DARYA:** One at a time, Yas. Listen first. Fight never.

*Fade to Mission 1 as the doors close behind her.*

---

## MISSION 1 — THE CUSTODIANS CONVENE

*Palace interior. Grand hall, twin column rows, crimson carpet strip, side corridor to the meeting chamber and the throne room beyond. The Council is in secret recess-session.*

### Objective 1 — Obj_Eavesdrop (hold the listening zone, 20s cumulative)

**[on objective start]**
> **DARYA:** The chamber's through the columns. You can't go in — but there's a service alcove where the ventilation carries every word. Plant your probe, stay in the zone, let it fill. Guards pass on a rotation; move between the two alcoves when they do.
>
> **YAS:** How long?
>
> **DARYA:** Long enough to hear something worth your neck. Stay in the sound.

**[ambient, guard approaching listening zone]**
> **DARYA:** Rotation. Slide to the far alcove — the sound carries there too.

**[ambient, probe placed in the open]**
> **DARYA:** Your probe's showing. Tuck it, or a guard will find it before you find your answer.

---

### THE COUNCIL SCENE (overheard during Obj_Eavesdrop)

*Heard through the vent, muffled at the edges, clear at the center. Twelve men around a long table under a dying chandelier. AZADI stands; the Custodians sit. HOSSEIN RAHIMI is among them. Play these lines out across the 20-second eavesdrop, looping/continuing if the player re-enters the zone.*

> **AZADI:** *(soft, unhurried)* The Guide is dying. Tonight, tomorrow — the physicians won't say, because saying it makes it real. And when it is real, this table decides who holds the republic. Twelve tired men in a cold room.
>
> **CUSTODIAN QOMI:** The constitution is not tired, General. It is precise.
>
> **AZADI:** The constitution did not stop the sabotage at the southern refinery, Haj Agha. Foreign hands. I have the intercepts. In a week there will be more — the water, the grid. The people will want a strong arm, not a *committee*.
>
> **HOSSEIN RAHIMI:** *(dry, level)* Strange, General. Every crisis you warn us of, your directorate is the first to know, and the only one to profit. You bring us intercepts no one else may verify.
>
> **AZADI:** You'd have me apologize for being competent.
>
> **HOSSEIN:** I'd have you show the Council one thing it can *check*. Just once.
>
> **AZADI:** *(a beat; softer still)* Hossein. We were young together. I am asking this table to sign an emergency provision — temporary — that lets one office act while twelve deliberate. That is all.
>
> **CUSTODIAN QOMI:** "Temporary" powers have a way of forgetting to end.
>
> **AZADI:** Then let a man you trust hold them. Sheikh Borujerdi has agreed to serve as caretaker Guide.
>
> **HOSSEIN:** Borujerdi has agreed to whatever you last told him to agree to. He is a curtain, General, and you are the hand behind it.
>
> **AZADI:** *(no anger — this is the frightening part)* You've always mistaken suspicion for principle, old friend. I have files, Hossein. On every man at this table. Debts, sons, letters that should have been burned. I have not opened them. I am asking you to make sure I never have to.
>
> **CUSTODIAN QOMI:** *(quiet)* Is that a threat, in the Guide's own house?
>
> **AZADI:** It's an inventory. Sign the provision, and it stays in a drawer. Refuse, and the republic learns what its Custodians are, one by one, in the week it can least afford it.
>
> **HOSSEIN:** *(after a long pause)* Put your inventory on the table, then. Let us all read our sins together and vote anyway. I would rather hang honest than rule as your puppet.
>
> **AZADI:** *(almost gently)* You always did want to hang, Hossein. I never understood it. — We'll adjourn. Sleep on it, all of you. Grief makes men brave and morning makes them sensible.
>
> **CUSTODIAN QOMI:** *(as chairs scrape)* This session never happened.
>
> **AZADI:** *(fading)* Nothing important ever does.

**[on objective complete — eavesdrop bar full]**
> **YAS:** *(very quiet)* He's blackmailing the whole Council. And my father just dared him to do it.
>
> **DARYA:** Then the files are real, and they're here. That "inventory" is the succession dossier, Yas. If we take it, we take his whole plan out of the drawer with it.
>
> **YAS:** Where.
>
> **DARYA:** The chamber empties in seconds. He wouldn't carry it out through a hall full of Custodians. It's still in that room. Go — before the guards reset.

### Objective 2 — Obj_StealDossier (steal the dossier from the chamber)

**[on objective start]**
> **DARYA:** They've adjourned. You've got a narrow window before the room's locked for the night. The dossier — a leather case, he wouldn't trust a screen with it.

**[ambient, guards re-entering chamber]**
> **DARYA:** Recess detail's coming back to sweep the room. In and out, Yas.

**[ambient, hint]**
> **DARYA:** Check the head of the table. A man like Azadi keeps his knife where his hand rests.

**[on objective complete — dossier picked up]**
> **YAS:** I have it. Every name. Every debt he thinks he owns.
>
> **DARYA:** That's the ballgame. Now we walk it out quietly and none of this was ever —
>
> *(a distant door; footsteps)*
>
> **DARYA:** ...Yas. Someone's coming back for it.

### Objective 3 — Obj_ReachThroneRoom (reach the throne room marker)

**[on objective start]**
> **DARYA:** Don't go back the way you came, the hall's filling. Through the far doors — the throne room, then out the garden side.
>
> **YAS:** The throne room's a dead end, Darya.
>
> **DARYA:** It's a door. All rooms are doors if you're brave enough. Move.

**[ambient, mid-corridor]**
> **DARYA:** He'll know the case is gone the second he reaches for it. Speed now, quiet second.

**[on objective complete — enters throne room, doors seal, lights rise]**

*The throne room. The gold dais at the far end. The doors boom shut behind her. A single figure is already standing by the throne, unhurried. AZADI.*

> **AZADI:** *(not turning at first)* The Rahimi girl. Educated abroad, they said. I didn't believe the rest of what they said. Now I do.
>
> **YAS:** *(even)* General.
>
> **AZADI:** You have something of mine.
>
> **YAS:** You have something of everyone's. That seems to be the arrangement.
>
> **AZADI:** *(finally turning, drawing his pistol without haste)* Your father dared me to open those files an hour ago. I'd hoped to spare him the sight of his daughter's name in them. — Put the case down, Yasaman. Walk out. I am, believe it or not, trying to be kind.
>
> **YAS:** You'll forgive me if I've had enough of your kindness for one evening.
>
> **DARYA (earpiece, low):** Yas — he won't let you walk. Cover, and make him work for it.

*Combat begins. AZADI Activate() — boss fight, MaxHealth 600, three phases.*

---

## MISSION 2 — THE PEACOCK THRONE

*The throne room. Boss encounter with AZADI, then extraction through the gardens with the dossier.*

### Objective 1 — Obj_DefeatAzadi (boss fight)

**[on Activate / fight start]**
> **AZADI:** You know, your father and I fought in the same trenches. He came home wanting a republic. I came home wanting it to *survive*.

**[Phase 1 → 2 transition, ~66% health — he summons reinforcements]**
> **AZADI:** *(calling)* To me! — Don't kill her. She's worth more explaining herself.
>
> **DARYA (earpiece):** He's brought friends. Thin them out and keep off his line — he telegraphs before every volley.

**[Phase 2 → 3 transition, ~33% health — speed up, charge attack]**
> **AZADI:** *(first crack in the calm)* Enough theatre, girl. Stand *still.*
>
> **YAS:** *(dry, breathing hard)* You first, General.

**[ambient during fight, Yas near-detection / low health via haptic beat]**
> **DARYA:** Heartbeat's up, Yas. Breathe. He's slower than his own pride.

**[on objective complete — AZADI defeated / NotifyBossDefeated]**

*AZADI down on one knee by the dais, pistol spent, a dart in his shoulder. He is not dead — she didn't kill him. He almost smiles.*

> **AZADI:** *(quietly, almost approving)* A tranquilizer. You came into my house to steal my life's work and you wouldn't even take mine. — You're your father's daughter after all. That's not a compliment, Yasaman. It's a diagnosis.
>
> **YAS:** *(standing over him, case in hand)* Then we'll both live with it.
>
> **AZADI:** *(as his eyes go heavy)* Run, then. Run with your drawer full of secrets. You've bought your father a week. Maybe two. — Nobody wins the succession, girl. You only decide who holds it when it breaks.
>
> **DARYA (earpiece):** He'll be out ninety seconds, no more. Don't stay to argue with a sleeping man. Garden side, extraction at the gate. Go.

### Objective 2 — Obj_Escape (extraction through the gardens to the gate)

**[on objective start]**
> **DARYA:** Back through the roses, the way you came. The garden's a different animal now — they'll have heard the noise.

**[ambient, alerted garden]**
> **DARYA:** They know something happened in the palace, they just don't know it's you yet. Keep it that way.

**[ambient, midway]**
> **DARYA:** Case first, glory never. If it comes to it, you drop *me* before you drop that dossier.
>
> **YAS:** *(quiet)* I'd never drop you, Darya.
>
> **DARYA:** ...Just get to the gate, you sentimental idiot.

**[ambient, if spotted during escape]**
> **DARYA:** Spotlight — move, move, the fountains, like we practiced.

**[on objective complete — reaches extraction marker at the gate → IsCampaignComplete]**
> **DARYA:** Car's running. Case in the back. — You did it, Yas.
>
> **YAS:** *(getting in, looking back at the lit palace)* Did I? He was awake enough to smile.

### Closing narration (victory card → over the drive away)

*As the car pulls from the palace gate, the rose-garden shrinking in the mirror. Yas, quiet, the leather case on her knees. Fade toward the prototype's victory line.*

> **YAS (VO):** My father dared a patient man to open his files. I stole the files instead. That's the difference between us — he'd rather be right in the morning; I'd rather be gone by then.
>
> **YAS (VO):** Azadi was correct about one thing. Nobody wins a succession. You steal a week. You hand it to twelve tired men in a cold room and hope they spend it better than the last week they were given.
>
> **YAS (VO):** The dossier is out of his drawer. The Council will convene again, and this time they'll read their sins with the doors open — because I took away the man who was going to read them out one at a time.
>
> **YAS (VO):** *(a breath)* It fixes nothing. It buys everything a little more time.
>
> **DARYA (earpiece, soft):** Get some sleep, Yas. The succession is safe.
>
> **YAS (VO):** *(almost a smile)* For now.

**[VICTORY CARD]**
> **THE SUCCESSION IS SAFE — for now.**

*End of prototype. The full campaign continues into Act I, Mission 3: The Long Bazaar.*

---

## APPENDIX — VOICE & LOCALIZATION NOTES
- **Registers.** Custodians speak formally (honorifics: *Haj Agha, Sheikh, Ayatollah*); Darya and Yas speak plainly and modern; Azadi mixes soldier's bluntness with a cleric's cadence — deliberately hard to place, which is the point.
- **Farsi pass.** These lines are authored to localize cleanly into Farsi (see ROADMAP.md); honorifics and the father/daughter register carry the emotional weight and must be handled by native writers, not literal translation. The English is the pivot script, not the master.
- **Restraint rule (binding).** No line names a real person, state, or organization. Only the fictional Council of Custodians, Guardian Corps, and The Qanat. Keep it that way in every localization.

---

## REACTIVE DIALOGUE BANKS (prototype missions)

These are the systemic, non-scripted lines that play off gameplay state across M0–M2. Pull randomly (no immediate repeats). All keep the restraint tone — Darya dry, Yas clipped, guards ordinary men, not cartoons.

### Darya — detection rising (crosses ~0.5)
- "They're curious, not certain. Sink back."
- "Someone thinks they saw a ghost. Prove them right about the ghost part — vanish."
- "Ease off, Yas. We cool this before it climbs."
- "Not caught. Curious. There's a whole world between those two words. Live in it."

### Darya — near-caught (meter red, >0.7, haptic heartbeat)
- "That's your heartbeat you're feeling. Slow it. Break their line and it fades."
- "One more set of eyes and it's over — and your father with it. Get gone."
- "Red, Yas. Move like it."

### Darya — detection cooling back down
- "There. It's passing. Breathe out."
- "They've talked themselves out of it. Men always do."
- "Back to a ghost. Good."

### Darya — player kills a guard (lethal, against tone)
- "...He's not getting up, Yas. That's a body now, and bodies get found."
- "Hide him. A sleeping man is a shift change; a dead one is an investigation."
- "I won't lecture you. But the morning will."

### Darya — player uses a dart (non-lethal, on-tone)
- "Down and dreaming. Tidy."
- "Nine minutes of nap. Use them."
- "Cleaner. Move him out of the light."

### Darya — player lingers / stalls
- "The Council doesn't wait, and neither does the night."
- "Whatever you're admiring, admire it faster."
- "Clock, Yas."

### Darya — player finds an optional intel/document
- "Keep that. Every scrap is a vote we'll need later."
- "Photograph it, don't pocket it — a missing paper is a question, a copied one isn't."

### Guard barks — Suspicious (investigating)
- "...Who's there? Show yourself to the light."
- "Probably the wind off the fountains. Probably."
- "I heard something. Stay sharp."
- "If that's one of the reception guests, you're in the wrong garden, friend."

### Guard barks — Chasing
- "Intruder! Hold where you are!"
- "Do not let her reach the gate!"
- "Call it in — call it in!"

### Guard barks — losing the player (returning to Patrol)
- "...Gone. Or never there."
- "Report it as a shadow. I'm not writing up a shadow."
- "Eyes open. I don't like it."

### Guard barks — discovering a downed body
- "Man down! Someone dropped Reza!"
- "He's breathing — drugged. This wasn't a guest. Lock it down."

---

## FAILURE-SCREEN VARIANTS (flavour under the prototype "COVER BLOWN" card)

The card is fixed ("COVER BLOWN — Yasaman's mission ends here. Press R to restart"), but a small italic line beneath sets the cost by context:

- **Caught in the garden (M0):** *"A guard's whistle, a name checked against a list. Somewhere, a phone rings in Hossein Rahimi's house."*
- **Caught eavesdropping (M1):** *"They found the probe before she found her answer."*
- **Caught with the dossier (M1/M2):** *"Azadi gets his files back — and a daughter to trade against a father."*
- **Killed by Azadi (M2):** *"He said he was trying to be kind. He wasn't lying, exactly."*
- **Caught during extraction (M2):** *"So close to the gate. The succession decides itself now, without her in the room."*

---

## STORY-SCENE STAGING NOTES (for cinematics)

- **The Council scene** plays diegetically through the vent — the camera stays on *Yas listening* (close, controlled, her father's voice reaching her), cutting only briefly to the chamber. We feel it through her, not as a wide political set-piece. Le Carré framing.
- **The throne-room turn** is the one time the game lets Azadi and Yas share a frame at rest before violence — hold on their faces; his calm is the threat.
- **The drive-away closing** is handheld, quiet, santur-only; the palace lights recede in the mirror. No music swell. The last word ("For now.") lands almost thrown away.

---

## OPTIONAL-OBJECTIVE & COLLECTIBLE DIALOGUE (M0–M2)

The prototype's critical path is fixed, but these lines support optional evidence and no-detection play the full game builds on.

### M0 — optional: overhear the reception guests (world-building)
- **DARYA:** "Slow past the tea table — those undersecretaries gossip better than any wiretap."
- **GUEST (ambient):** "...they say the Guide hasn't spoken in two days. They say the General visits the physicians himself."
- **DARYA:** "'They say.' In this city, 'they say' is the only honest news service left."

### M0 — optional: no-detection bonus acknowledged
- **DARYA:** "Not a whisker out of place. If the whole week goes like this, your father sleeps easy."

### M1 — optional: photograph the seating chart (evidence)
- **DARYA:** "Photograph the chart — who sat where tells us who Azadi already owns."
- **YAS:** "Borujerdi's at his right hand."
- **DARYA:** "The curtain sits closest to the hand. Of course he does."

### M1 — optional: recover a Custodian's letter (evidence)
- **YAS:** "A letter. Half-burned. My father's handwriting."
- **DARYA:** "...Leave the original. Photograph it. If Azadi wants to own that letter, don't hand him proof you were the one who read it."

### M2 — optional: spare all reinforcements (restraint bonus)
- **DARYA:** "Two of his own men, asleep instead of dead. He'll hate that more than losing the fight."

---

## IDLE & TRADECRAFT ONE-LINERS (Yas, low-frequency, dry)

Play sparingly during long stealth stretches; keep the controlled, dry-humored register.
- "Beautiful house. Shame about the tenants."
- "My father calls this room 'the cold room.' Now I know why."
- "Somewhere out there Darya is drinking my share of the tea."
- "Move like you belong. Right. I belong in a room with a case full of other people's sins."
- "Every door in this palace opens for a name. Mine, tonight."
- "If I get out of this, I'm sleeping for a week. If I don't — well. So is he."

---

## HINT ESCALATION LADDER (if the player stalls, per objective)

Darya's guidance escalates in three steps, then goes quiet again — never nags.
- **Soft (30s):** a tone reminder ("Clock, Yas.").
- **Directional (60s):** names the place ("Service alcove, left of the columns.").
- **Explicit (90s):** names the action ("Plant the probe *in* the alcove and stay put until the bar fills."). Then silence for a full cycle before repeating. On Story difficulty the ladder is faster and adds a marker; on Veil it is slower and marker-free (ties to GDD §8/§12).

---

## TRIGGER-TO-OBJECTIVE IMPLEMENTATION MAP (engineering-facing)

Ties the scripted lines above to the prototype's actual objective chain (`UMissionSubsystem`, per SPEC), so audio/design can wire triggers without re-reading prose. Fire on `CompleteObjective` / objective-start hooks and on the listed conditions.

| Mission | Objective Id | Line trigger | Lines |
|---------|-------------|--------------|-------|
| M0 | (pre-gameplay) | level load / fade-in | Cold open (5 lines) |
| M0 | Obj_EnterGrounds | on start / spotlight overlap / sprint-near-guard | 3 blocks |
| M0 | Obj_EnterGrounds | on complete | 1 line |
| M0 | Obj_Keycard | on start / Shirazi turn / stall timer | 3 blocks |
| M0 | Obj_Keycard | on complete (pickpocket success) | 2 lines |
| M0 | Obj_EnterPalace | on start / overwatch cone / detection>0.5 | 3 blocks |
| M0 | Obj_EnterPalace | on complete (card swipe) | 3 lines → fade M1 |
| M1 | Obj_Eavesdrop | on start / guard rotation / probe-in-open | 3 blocks |
| M1 | Obj_Eavesdrop | in-zone, timed over 20s | **Council scene (20 lines)** |
| M1 | Obj_Eavesdrop | on complete (bar full) | 4 lines |
| M1 | Obj_StealDossier | on start / guards return / stall hint | 3 blocks |
| M1 | Obj_StealDossier | on complete (dossier pickup) | 3 lines + SFX cue |
| M1 | Obj_ReachThroneRoom | on start / mid-corridor | 2 blocks |
| M1 | Obj_ReachThroneRoom | on complete (throne room seal) | throne-room exchange (7 lines) |
| M2 | Obj_DefeatAzadi | on Activate / phase 66% / phase 33% / low-HP | 4 blocks |
| M2 | Obj_DefeatAzadi | on complete (NotifyBossDefeated) | Azadi-down exchange (5 lines) |
| M2 | Obj_Escape | on start / alerted / midway / spotted | 4 blocks |
| M2 | Obj_Escape | on complete (extraction → IsCampaignComplete) | 2 lines → closing narration → victory card |

Reactive banks (detection ladder, guard barks, kills/darts) hook the detection meter (`GetDetection`), guard state changes (Patrol/Suspicious/Chasing), and `ApplyDamage`/down events — not the objective chain — so they layer over any mission.

## LOCALIZATION LINE-COUNT ESTIMATE (prototype scripts)

- Scripted (critical-path) lines M0–M2: ~70, incl. the 20-line Council scene and both Azadi exchanges.
- Reactive banks: ~55 unique lines (Darya detection ×3 states, kill/dart, stall, guard barks ×4 states, idle ×6).
- Optional-objective lines: ~15.
- **Prototype total ≈ 140 lines × 2 languages (EN/FA) ≈ 280 recorded takes** (before alts/pickups). Scales roughly ×12 for the full campaign's scripted content plus far larger reactive/ambient banks — the basis for the VO budget in ROADMAP.md §17.

---

## ACT I BRIDGE — TEASER INTO MISSION 3 (post-prototype hook)

Plays after the victory card, as the first stinger of the full campaign — showing how the shipped prototype hands off to "The Long Bazaar." Same restraint; no music swell.

*A safe apartment, dawn. The leather case open on a table. Yas has not slept. Darya sets down two glasses of tea.*

> **DARYA:** Every name Azadi owns. Every debt. It's all here.
>
> **YAS:** And every page is coded. It's a locked box inside a locked box.
>
> **DARYA:** Then we find the key. There's a man in the old bazaar who reads Azadi's ciphers like a menu. Getting to him is the problem — Vaziri's people have had the market watched since midnight.
>
> **YAS:** *(dry)* So I walk into a covered market full of watchers, carrying the one thing they'd kill to get back, to meet a man I've never seen.
>
> **DARYA:** In daylight. In a crowd. Where you can't hide in a shadow because there aren't any.
>
> **YAS:** *(picking up the tea)* You know, when you recruited me, you promised glamour.
>
> **DARYA:** I promised you'd matter. That's the only glamour this work has. — Azadi's awake by now. He knows the case is gone, and he knows exactly whose daughter took it. This stops being a burglary today, Yas. Today it becomes a manhunt.
>
> **YAS:** *(setting down the glass, standing)* Then let's not keep the hunters waiting.

*Cut to black. Title card:*

> **VEIL OF SUCCESSION**
> **Act I — Mission 3: The Long Bazaar**

*(Full-campaign scripts for Missions 3–12 are authored in the production phase; the banks, triggers, and tone rules established here are the template.)*
