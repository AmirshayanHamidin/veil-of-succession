# D2 DRAFT — The Council Eavesdrop Scene (full script + 2 revelation alternates)

*A work of political fiction. All persons, offices, and organizations are fictitious.*

**Status:** DRAFT for the user's voice pass. Canon spine: the 20-line Council scene
in `Docs/MISSION_SCRIPTS.md` (M1, Obj_Eavesdrop). Canon lines are reproduced
verbatim and unmarked; new material is tagged **[EXT]** so it can be stripped
without touching canon. Nothing in `MISSION_SCRIPTS.md` was modified.

**What this file adds:** (1) the scene as a complete, recordable script with
staging and beat timing; (2) two alternate versions of the key revelation —
the moment the Council learns Azadi holds files on all twelve of them.

---

## Design frame (how a 2½-minute scene fits a 20-second objective)

The eavesdrop bar fills over 20 cumulative seconds in the listening zone, but the
scene runs ~2½ minutes of audio. Canon already says the lines "loop/continue if
the player re-enters the zone." Proposal, to guarantee every player hears the
revelation:

- Split the scene into **Beats 1–6** below. The bar may reach 100% visually, but
  `Obj_Eavesdrop` does not complete until **Beat 4 (the revelation)** has played.
  Fast players hold a full bar for a few extra seconds of Beat 4; slow players
  never notice. NEEDS-PC: wire beat-gating in `UMissionSubsystem` (see the
  trigger map in MISSION_SCRIPTS.md).
- Beats 5–6 keep playing after completion for players who stay in the sound.
  Leaving early is allowed — the post-scene Yas/Darya exchange covers the plot
  minimum either way.

**Staging (binding, per canon staging notes):** the camera stays on *Yas
listening* — close, controlled, her father's voice reaching her through the
grate. Cut to the chamber only twice: once on the revelation, once on Hossein's
answer. We feel the scene through her. Le Carré framing, no score until the
adjournment shuffle.

---

## THE FULL SCRIPT (baseline — canon revelation)

*Service alcove off the grand hall. YAS at the ventilation grate. Through it,
muffled at the edges, clear at the center: twelve men around a long table under
a dying chandelier. AZADI stands; the Custodians sit. HOSSEIN RAHIMI among them.*

### Beat 1 — The dying Guide (~25s)

**[EXT — staging]** *Chairs settle. A glass poured somewhere. Yas's hand flat
against the stone, steadying.*

> **AZADI:** *(soft, unhurried)* The Guide is dying. Tonight, tomorrow — the physicians won't say, because saying it makes it real. And when it is real, this table decides who holds the republic. Twelve tired men in a cold room.
>
> **CUSTODIAN QOMI:** The constitution is not tired, General. It is precise.

**[EXT]**
> **CUSTODIAN SABERI:** *(elderly, careful)* Precise, Haj Agha, and slow. The General is not wrong about the hour.

### Beat 2 — The manufactured crisis (~30s)

> **AZADI:** The constitution did not stop the sabotage at the southern refinery, Haj Agha. Foreign hands. I have the intercepts. In a week there will be more — the water, the grid. The people will want a strong arm, not a *committee*.
>
> **HOSSEIN RAHIMI:** *(dry, level)* Strange, General. Every crisis you warn us of, your directorate is the first to know, and the only one to profit. You bring us intercepts no one else may verify.
>
> **AZADI:** You'd have me apologize for being competent.
>
> **HOSSEIN:** I'd have you show the Council one thing it can *check*. Just once.

**[EXT — staging]** *On Yas: the smallest reaction to her father's voice — not
pride yet. Recognition. He talks at the Council table the way he talks at dinner.*

### Beat 3 — The ask, and the curtain (~30s)

> **AZADI:** *(a beat; softer still)* Hossein. We were young together. I am asking this table to sign an emergency provision — temporary — that lets one office act while twelve deliberate. That is all.
>
> **CUSTODIAN QOMI:** "Temporary" powers have a way of forgetting to end.
>
> **AZADI:** Then let a man you trust hold them. Sheikh Borujerdi has agreed to serve as caretaker Guide.
>
> **HOSSEIN:** Borujerdi has agreed to whatever you last told him to agree to. He is a curtain, General, and you are the hand behind it.

**[EXT]**
> **CUSTODIAN SABERI:** *(low, to no one)* God preserve us from men who volunteer.

### Beat 4 — THE REVELATION (canon baseline) (~35s) — *objective may not complete before this beat plays*

*First cut to the chamber: Azadi, standing, hands at rest on a chairback. Calm.*

> **AZADI:** *(no anger — this is the frightening part)* You've always mistaken suspicion for principle, old friend. I have files, Hossein. On every man at this table. Debts, sons, letters that should have been burned. I have not opened them. I am asking you to make sure I never have to.
>
> **CUSTODIAN QOMI:** *(quiet)* Is that a threat, in the Guide's own house?
>
> **AZADI:** It's an inventory. Sign the provision, and it stays in a drawer. Refuse, and the republic learns what its Custodians are, one by one, in the week it can least afford it.

### Beat 5 — Hossein's answer (~20s)

*Second cut to the chamber: Hossein, seated, not moving. Then back to Yas —
this is the line she will carry into the throne room.*

> **HOSSEIN:** *(after a long pause)* Put your inventory on the table, then. Let us all read our sins together and vote anyway. I would rather hang honest than rule as your puppet.
>
> **AZADI:** *(almost gently)* You always did want to hang, Hossein. I never understood it. — We'll adjourn. Sleep on it, all of you. Grief makes men brave and morning makes them sensible.

### Beat 6 — Adjournment (~15s, plays even after objective completes)

> **CUSTODIAN QOMI:** *(as chairs scrape)* This session never happened.
>
> **AZADI:** *(fading)* Nothing important ever does.

**[EXT — staging]** *Footsteps disperse beyond the wall. Yas stays one breath
too long at the grate — then moves. The post-scene exchange (canon, on
objective complete) picks up from here: "He's blackmailing the whole Council…"*

---

## ALTERNATE A — "The Demonstration"
*Replaces Beat 4 only. Beats 1–3, 5–6 unchanged. Azadi never announces the
files — he demonstrates one. The threat is procedural, not declared. Colder;
trusts the audience.*

*First cut to the chamber: an aide places a slim leather case before Azadi. He
opens it without hurry, puts on reading glasses — a bureaucrat's gesture, worse
than a drawn gun.*

> **AZADI:** *(reading, flat, unhurried)* "Custodian Saberi. Fourth of Mehr. Transfer of eleven billion rials from the Bonyad accounts to a shipping concern registered to his son-in-law. Repaid — *(turning a page)* — in part."
>
> *(silence at the table)*
>
> **CUSTODIAN SABERI:** *(barely audible)* General —
>
> **AZADI:** *(closing the file, removing the glasses)* I am not reading the rest. There is a file like it for every chair at this table. I keep them in a drawer, and I am asking for your signatures so that the drawer stays shut. That is the whole of my request.
>
> **CUSTODIAN QOMI:** *(quiet)* Is that a threat, in the Guide's own house?
>
> **AZADI:** It is a kindness, Haj Agha. The threat is what the newspapers would do with it.

*Hossein's Beat 5 answer plays unchanged — "Put your inventory on the table,
then…" — and lands harder, because now the table has heard what an opened file
sounds like.*

**Why pick A:** the scene's horror becomes specific and administrative. One
named victim (Saberi, established in Beats 1/3 as the frightened moderate)
makes the other eleven files real without a speech. Azadi never says a
threatening sentence — fully on-tone for "the calmer he is, the worse it is."

**Knock-ons:** none to the throne room. Yas's post-scene line could sharpen to:
"He read a man's sins to his face like a grocery list. And my father just dared
him to keep reading."

---

## ALTERNATE B — "The Personal Ledger"
*Replaces Beat 4 and adjusts Beat 5. Hossein preempts the reveal — canon says
he suspects and cannot prove; here he forces it into the open, and Azadi
escalates to the one file that matters: family. Yas hears her own name.*

> **HOSSEIN:** *(cutting in, level)* Ask what you came to ask, General. Or shall I ask it for you? — He keeps files, brothers. On this table. I've spent a year finding the edges of them: debts, sons, letters that should have been burned. He has never denied it to me privately. Let him deny it here.
>
> *(a pause; when Azadi speaks, it is softer than before)*
>
> **AZADI:** I won't insult you with a denial. Yes. Twelve files. Sign the provision and they stay in their drawer — I have said this to Hossein in private and I say it to all of you now, so no man votes blind.
>
> **HOSSEIN:** Then put them on the table. Let us all read our sins together and vote anyway. I would rather hang honest than rule as your puppet.
>
> **AZADI:** *(a beat; the room goes very still)* You know what is in your file, old friend. You have made your peace with hanging; I believe you. But it is a thick file. — Does your daughter know she is in it?

*Cut to Yas at the grate. She has stopped breathing.*

> **HOSSEIN:** *(the first time his voice is not level)* …My daughter has nothing to do with this table.
>
> **AZADI:** *(almost gently)* Neither did anyone else's son. That is what a file *is*, Hossein. — We'll adjourn. Sleep on it, all of you. Grief makes men brave and morning makes them sensible.

*Beat 6 unchanged.*

**Adjusted post-scene exchange (replaces canon on-complete lines if B is chosen):**

> **YAS:** *(very quiet)* He has a file on me. My father knew there were files — he didn't know I was in one.
>
> **DARYA:** Then you heard the only thing in that room that matters. The files are real, they're *here*, and one of them has your name in it. Take the drawer, Yas. Take the whole plan out of it.

**Why pick B:** the revelation stops being political and becomes the game's
actual subject — a father and daughter inside a machine that eats families. It
gives Yas a personal reason to steal the dossier beyond the mission, and makes
the player complicit in her wanting to *read her own file*.

**Knock-ons (must be handled if B is chosen):** the throne-room line "I'd hoped
to spare him the sight of his daughter's name in them" changes from first
reveal to callback — suggest: "You've heard by now what's in your file.
I'd still have spared your father the sight of it." Hossein's Beat 5 dare also
gains a cost he *knows* about, which slightly recolors "I would rather hang
honest" — flag for the user's taste pass.

---

## Notes for the user (taste calls — NEEDS-USER)

1. **Pick one revelation:** canon baseline (declared inventory), A (demonstrated
   on Saberi), or B (personal — Yas's name). A is coldest, B is most intimate;
   the baseline is the most economical (~5 fewer lines than A, ~8 fewer than B).
2. **[EXT] lines** (Saberi in Beats 1/3, staging inserts) stand on their own and
   are worth keeping even with the baseline — they make "twelve men" audible.
   Saberi is required for Alternate A.
3. **VO impact:** baseline scene 20 lines; +3 [EXT]; A replaces 3 lines with 6
   (+1 Saberi); B replaces 3 with 7 and swaps the 2-line post-scene exchange.
   One new speaking role either way (Saberi — elderly, careful).
4. **Localization:** Saberi's ledger entry in A uses the Persian calendar
   ("Fourth of Mehr") and rials by design; per the Farsi-pass rule, honorifics
   and the father/daughter register need native handling, not literal
   translation. Restraint rule holds: no real persons, states, or organizations.
