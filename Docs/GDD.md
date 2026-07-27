# VEIL OF SUCCESSION — Game Design Document (Full Campaign)
*A work of political fiction. All persons, offices, and organizations are fictitious. This document extends the 3-mission C++ prototype into the complete ~10-hour campaign.*

---

## 1. VISION & PILLARS

**Logline.** Yasaman "Yas" Rahimi, a constitutional intelligence operative, has one week to stop General Farrokh Azadi from stealing the succession of a dying republic — while her own father, a Custodian, sits inside the machine Azadi is trying to capture.

**Genre.** Third-person stealth-action espionage thriller. Single-player, narrative-driven, linear-with-branches.

**Target spec.** PlayStation 5 lead platform, 1080p / 60fps performance target (quality mode 4K/30 optional). DualSense-first design: adaptive triggers and haptics are not garnish, they carry mechanics (see §11). PC and Xbox Series follow.

**Length.** ~10 hours critical path, 12 missions across 3 acts, 45–55 minutes each.

### Design pillars

1. **The name opens doors the tradecraft cannot.** Yas is a Custodian's daughter. Half the game's problem-solving is social access — being *permitted* to be somewhere — and the other half is not being *seen* once the permission runs out. Every mission pairs a "legitimacy" layer (papers, escorts, family standing) with a "shadow" layer (vents, rooftops, blind spots). Losing the first forces you into the second.

2. **Restraint over spectacle (Le Carré, not Bond).** The palace is beautiful and the rot is quiet. Set-pieces are tense, not explosive. Violence is *optional wherever the level allows it* — tranquilizer darts, chokeholds, and clean exits score higher than bodies. Lethal play is available but the world reacts to it: corpses trigger lockdowns, and the epilogue tallies your restraint.

3. **A daughter's stakes.** If Yas is caught, Hossein Rahimi hangs with her. Detection is never abstract — the fail screen names her father. This raises the felt cost of a single mistake above the usual stealth-game "reload and shrug."

4. **A victory that fixes nothing permanently.** The campaign ends on "the succession is safe — for now." No triumphant regime change; a fragile constitutional breathing space bought at cost. The whole game is tuned to earn that ambivalence.

### Anti-pillars (what we will not do)
- No real living officials, real named states, or real organizations — only the fictional **Council of Custodians**, **Guardian Corps**, and **The Qanat**.
- No wanton civilian violence rewarded by systems.
- No gadget power-fantasy that trivializes tension (gadgets are scarce, loud, and consequential).
- No open world. Handcrafted, readable levels.

---

## 2. STORY & CAST (canon summary — binding)

- **Yasaman "Yas" Rahimi**, 29 — protagonist. Daughter of Custodian Hossein Rahimi; educated abroad; recruited by **The Qanat**, a back-channel network of constitution-loyal officers. Elegant, controlled, dry-humored.
- **General Farrokh Azadi**, 58 — antagonist. Commander of Guardian Corps intelligence. Engineering a "managed succession" via staged foreign-sabotage crisis to seize emergency powers. Keeps the succession dossier (blackmail on all twelve Custodians) in the palace.
- **Hossein Rahimi** — Yas's father, moderate Custodian. Suspects the plot, cannot prove it, will not flee.
- **"Darya"** — Yas's Qanat handler, voice on the earpiece.
- **Captain Omid Shirazi** — palace guard captain whose keycard opens the inner doors (prototype); recurs as a reluctant asset.
- **The Council of Custodians** — twelve senior clerics and officials who constitutionally choose the successor to **Ayatollah Mahdavi**, the dying Supreme Guide.

New supporting cast introduced for the full campaign (all fictional):
- **Colonel Nasrin Vaziri** — Azadi's counter-intelligence chief, methodical, the recurring human obstacle; a "boss" of investigation rather than firepower.
- **Bijan "the Archivist" Motahari** — ministry records clerk, terrified, turnable; source in Act II.
- **Sardar Yas-adjacent field asset "Kaveh"** — Qanat legwork man who occasionally provides exfil.
- **Mother of the House, Banu Sohrabi** — matriarch of the seminary hostel, gatekeeper of the holy-city mission.

---

## 3. CAMPAIGN STRUCTURE — 12 MISSIONS, 3 ACTS

Acts map to the classic infiltration arc: **gather proof → turn the deciders → survive the night the throne changes hands.** Missions 1–2 are the shipped prototype content (M0/M1/M2 collapse into the campaign's Missions 1 and 2 with expanded objectives; the prototype's throne-room boss becomes the Act I mid-boss rather than the finale).

| # | Act | Title | Location | Core |
|---|-----|-------|----------|------|
| 1 | I | The Wall of Roses | Palace gardens (Niavaran-style) | Infiltration, keycard lift |
| 2 | I | The Custodians Convene | Palace interior | Eavesdrop, dossier theft, Azadi mid-boss |
| 3 | I | The Long Bazaar | Old-city covered market | Tail & be-tailed, dead drop |
| 4 | I | Ledger of Ashes | Guardian Corps records annex | Steal financial proof, first Vaziri duel |
| 5 | II | House of Safe Rooms | Qanat safehouse network under siege | Defense/escape, protect asset |
| 6 | II | The Ministry Archives | State ministry, daytime, badge-legit | Social stealth, forge the counter-file |
| 7 | II | The Seminary of Quiet Water | Holy-city seminary & hostel | No-weapons sacred-space stealth |
| 8 | II | Broadcast Standard | State TV station | Splice the truth into the feed, Vaziri rematch |
| 9 | III | The Salt Prison | Desert detention facility | Prison break of Hossein Rahimi |
| 10 | III | The Cold Retreat | Azadi's mountain retreat | Assassinate-or-expose branch |
| 11 | III | The Night of Custodians | City under curfew, counter-coup | Escort/rally the Council |
| 12 | III | Veil of Succession | The palace, again | Finale, Azadi Phase-2 boss |

Pacing rhythm: each act = one "loud" mission (defense or chase) between "quiet" infiltrations, so tension breathes. Acts end on escalating personal cost: I ends with proof but Azadi now hunting Yas by name; II ends with her father arrested to draw her out; III ends with the fragile win.

---

## 4. MISSION-BY-MISSION DESIGN

Format per mission: **Location · Story beat · Objectives · Stealth/combat setup · Unique mechanic · Failure states · Boss/set-piece.**

### MISSION 1 — THE WALL OF ROSES  *(Act I · prototype M0)*
- **Location.** The palace rose-garden and outer courtyard at night; ornamental hedges, fountains, sandstone colonnade.
- **Story beat.** Mahdavi is failing. The Council convenes in secret tonight. Darya gets Yas over the wall: her job is to reach the palace doors before the session ends. Establishes tone, controls, and the "legitimacy vs shadow" duality — a garden party of minor officials gives partial cover if Yas moves like she belongs.
- **Objectives.** 1) Enter the grounds (reach the courtyard marker). 2) Lift Captain Shirazi's keycard (pickpocket during his cigarette break). 3) Reach the palace doors (Door interactable, needs keycard).
- **Stealth/combat setup.** ~6 courtyard patrols on straight legs, roaming spotlights, one static overwatch on the colonnade roof. No lethal expectation; darts available but noise brings a second patrol.
- **Unique mechanic.** **The Pickpocket window** — approach Shirazi from behind while he is stationary; a DualSense adaptive-trigger "resistance-then-give" pull represents easing the card from his coat. Blow it and he turns.
- **Failure states.** Full detection (COVER BLOWN). Killing a garden guest (hard fail — civilians). Losing the keycard by triggering a body search.
- **Set-piece.** Spotlight sweep across the reflecting pool — a timed dash between fountain plinths.

### MISSION 2 — THE CUSTODIANS CONVENE  *(Act I · prototype M1+M2)*
- **Location.** Palace interior — grand hall with twin column rows, side corridor, the meeting chamber, and the throne room with its raised gold dais.
- **Story beat.** Inside, Yas eavesdrops on the secret session: Azadi pressuring the Custodians, her father resisting (full scene in MISSION_SCRIPTS.md). She steals the succession dossier — and Azadi finds her in the throne room. Mid-boss.
- **Objectives.** 1) Eavesdrop on the session (hold the listening zone, 20s cumulative, moving between two safe alcoves as guards pass). 2) Steal the dossier from the chamber. 3) Reach the throne room. 4) Survive Azadi. 5) Escape through the gardens.
- **Stealth/combat setup.** Interior patrols (2 hall, 2 corridor), chamber guarded during recess. Tight sightlines reward the listening probe and vent routes.
- **Unique mechanic.** **The Listening Probe** — plant/hold a directional mic to fill the eavesdrop bar; guards can spot the probe if left in the open, so placement is a bet.
- **Failure states.** Detection; being caught with the dossier before extraction (chase, not instant fail); dying to Azadi.
- **Boss — GENERAL AZADI (Encounter 1 of 2).** Per engineering: MaxHealth 600, three phases at 100/66/33%. P1 telegraphed pistol volleys (dive between columns). P2 summons 2 reinforcements. P3 charge attack (bait and sidestep). Narratively he *lets her go* wounded — he wants to follow the dossier to The Qanat. This is the prototype's shipped fight, recontextualized as "he chose to lose."

### MISSION 3 — THE LONG BAZAAR  *(Act I)*
- **Location.** The old city's covered bazaar — a labyrinth of vaulted brick galleries, carpet stalls, tea houses, a caravanserai courtyard. Daytime, dense crowd.
- **Story beat.** The dossier is coded; Yas must reach a Qanat cryptographer via a chain of dead drops while Vaziri's watchers sweep the bazaar for her. First taste of **being the hunted** rather than the hunter.
- **Objectives.** 1) Make the first dead drop (tea-house samovar). 2) Identify and lose your tail (three watchers, only two are real). 3) Reach the cryptographer's stall without leading anyone to it. 4) Exfil through the caravanserai.
- **Stealth/combat setup.** Social stealth in crowd; "blend" meter instead of shadows. Weapons drawing here empties the gallery and hard-fails the covert objective.
- **Unique mechanic.** **Crowd-blend & the tail-read** — hold a slow walk to sink into foot traffic; a haptic "tap" on the controller pulses toward a watcher who has eyes on you, teaching players to read surveillance geometry. Ducking a stall's curtain breaks line of sight.
- **Failure states.** Leading a watcher to the cryptographer (mission-critical fail, stall "burns"). Full exposure. Crowd panic from drawn weapon.
- **Set-piece.** The caravanserai squeeze — gates closing on a timer as watchers converge; a rooftop grapple exit.

### MISSION 4 — LEDGER OF ASHES  *(Act I)*
- **Location.** Guardian Corps financial records annex — a brutalist mid-rise, filing floors, a basement incinerator room where evidence is burned nightly.
- **Story beat.** The dossier proves blackmail; now Yas needs the *money* — proof Azadi funded the "foreign sabotage" himself. The paper is scheduled to burn at dawn. Act I climax; Colonel Vaziri runs the building.
- **Objectives.** 1) Reach the ledger floor before the incinerator cycle. 2) Photograph three funding transfers (each in a different guarded office). 3) Plant a listening probe in Vaziri's office. 4) Escape before lockdown. Optional: pull the physical ledger (raises detection, richer proof in epilogue).
- **Stealth/combat setup.** Camera networks (EMP-scramble to blind), roving two-guard pairs, a keycard-tiered elevator. Introduces the **EMP scrambler** gadget properly.
- **Unique mechanic.** **The Burn Timer** — a global countdown; every alarm you trigger advances the incinerator cycle, so loud play literally destroys your objective. Systemic pressure to stay quiet.
- **Failure states.** Ledger burns (partial fail — you lose the strongest evidence tier but can continue). Full detection. Dying.
- **Boss — COLONEL VAZIRI (Duel 1 of 2, non-lethal cat-and-mouse).** Not a health-bar brawl: a *pursuit puzzle*. Vaziri methodically clears floors room-by-room; Yas must evade or non-lethally neutralize her without killing (killing her makes Act II vastly harder and darkens the ending). Defeating = escaping with proof while she's still searching the wrong floor.

### MISSION 5 — HOUSE OF SAFE ROOMS  *(Act II)*
- **Location.** A Qanat safehouse — an unremarkable townhouse rigged with hidden rooms, connected via rooftops and a shared cellar to two neighbors.
- **Story beat.** Azadi's people have traced the dossier's decryption to this house. The "loud" mission of Act II: defend the cryptographer and the decrypted files long enough to move them, then escape a raid. Personal — Kaveh may die here based on player performance.
- **Objectives.** 1) Rig the escape route (barricades, a rope line between roofs). 2) Hold the cellar while decryption finishes (waves, but stealth-takedowns thin them silently to avoid escalation). 3) Extract the cryptographer across the rooftops. 4) Collapse the route behind you.
- **Stealth/combat setup.** Hybrid: a siege you *can* fight quietly. Every gunshot summons more Guardian Corps; silent takedowns keep the wave count low. Rewards discipline under pressure.
- **Unique mechanic.** **Reactive raid escalation** — an on-screen "alert tier" rises with noise and falls with silence and time; the raid's size scales to it live. The mission teaches the whole game's thesis in one room.
- **Failure states.** Cryptographer dies. Decryption interrupted. Yas dies.
- **Set-piece.** The rope-line rooftop crossing under a searchlight helicopter (fictional Guardian Corps aviation), grapple-timed.

### MISSION 6 — THE MINISTRY ARCHIVES  *(Act II)*
- **Location.** A state ministry, mid-day, fluorescent corridors and reading rooms. Yas enters on legitimate business using her father's standing and a forged researcher's pass.
- **Story beat.** To turn wavering Custodians, Yas needs a *counter-file*: an official-looking dossier that reframes Azadi's crisis as his own fabrication. She must forge it *inside* the system using real stamps and real registry numbers — the archivist Bijan is her frightened inside man.
- **Objectives.** 1) Pass reception on legitimacy (dialogue/pass check). 2) Meet Bijan; calm him (a small trust mini-scene). 3) Pull three genuine documents to Frankenstein the counter-file. 4) Get it stamped and registered (the "official" step). 5) Walk out clean before a real auditor cross-checks.
- **Stealth/combat setup.** **Social stealth dominant** — no weapons drawn in a working ministry without hard fail. Restricted zones need escalating credentials; wander wrong and a clerk challenges you.
- **Unique mechanic.** **The Legitimacy meter** — a second, inverse detection bar. Acting like you belong (right pace, right rooms, answering challenges) keeps it high; it decays in restricted areas. At zero, you're an intruder and the shadow game begins.
- **Failure states.** Legitimacy collapse into a locked-down building. Bijan panics and reports you (avoidable via the trust scene). Forgery flagged at registration (redo step).
- **Set-piece.** The stamp-and-seal sequence — a DualSense adaptive-trigger press to work an old mechanical date-stamp without smudging, timed against a passing supervisor.

### MISSION 7 — THE SEMINARY OF QUIET WATER  *(Act II)*
- **Location.** A holy-city seminary and its pilgrim hostel: courtyards, a reflecting pool, cloisters, a library of manuscripts. Reverent, hushed, no-firearms environment.
- **Story beat.** Custodian Ayatollah Feyzi — the swing vote — has retreated here to pray on the succession. Yas must reach him and deliver proof *without* profaning a sacred space or being seen by his Guardian Corps minders. Banu Sohrabi, the hostel matriarch, controls access.
- **Objectives.** 1) Gain the hostel (win Banu's confidence — a conversation, and respecting house customs). 2) Move through the seminary during prayer cycles (movement only permitted at certain hours/zones). 3) Reach Feyzi's cell unseen by minders. 4) Deliver the proof and secure his vote (dialogue). 5) Slip out before the dawn call.
- **Stealth/combat setup.** **Weapons holstered by design** (drawing one is an instant reverence-fail and turns the whole seminary against you). Pure movement, timing, and disguise. Minders patrol; pilgrims are cover.
- **Unique mechanic.** **Prayer-cycle timing** — the space empties and fills on a schedule shown as a subtle clock; whole courtyards become safe or forbidden by the hour. Stealth becomes rhythm, not just geometry.
- **Failure states.** Reverence-fail (weapon, running in the sanctuary, entering forbidden zones). Minders spot you. Feyzi refuses if you arrived disrespectfully.
- **Set-piece.** Crossing the manuscript library while a scholar reads aloud — footfall-quiet traversal, haptics carry your own footstep loudness back to you.

### MISSION 8 — BROADCAST STANDARD  *(Act II)*
- **Location.** State television and radio complex — studios, a master-control room, transmitter mast, underground cabling.
- **Story beat.** Act II climax. The counter-file means nothing unheard. Azadi is about to announce the "foreign sabotage" live; Yas must splice the truth into the national feed at the moment of his broadcast — turning his own address into his exposure. Vaziri, promoted and furious after Mission 4, defends the building.
- **Objectives.** 1) Reach master control via the cable tunnels. 2) Loop the security-camera feeds (EMP + probe). 3) Cue the counter-file into the broadcast chain. 4) Hold master control during Azadi's live address for the splice window. 5) Escape the mast.
- **Stealth/combat setup.** Verticality (studios stacked over tunnels), camera-dense, a live-broadcast "no-noise-on-air" constraint. Loud play risks cutting the feed and losing the objective.
- **Unique mechanic.** **The Live Window** — a fixed real-time countdown to Azadi's address you cannot pause or rush; you must be *in position and unseen* at the exact second. Forces planning a whole route around one immovable moment.
- **Failure states.** Missing the window (mission fail). Feed cut by alarm. Death.
- **Boss — COLONEL VAZIRI (Duel 2 of 2).** The rematch resolves the Mission-4 thread. If Yas spared her before, Vaziri hesitates at the key moment — a scripted beat where restraint pays off narratively and mechanically (she can be talked down / stood down). If Yas killed *her people* before, no mercy: a hard non-lethal chase through the transmitter gantries. The set-piece is a catwalk stand-off over the humming mast.

### MISSION 9 — THE SALT PRISON  *(Act III)*
- **Location.** A remote desert detention facility built into salt flats — bleached walls, a wind that carries sound, black-site cellblock where Hossein Rahimi is now held (arrested at the end of Act II to bait Yas).
- **Story beat.** Act III opens on the personal debt of the whole game: Yas breaks her father out. He will not thank her cleanly — he'd rather have stayed to fight from inside. The reunion is tense, not warm.
- **Objectives.** 1) Cross the exposed salt approach (no cover — use night, wind, and timing). 2) Breach the cellblock (EMP the door grid). 3) Reach and free Hossein. 4) Escort him out (he is slow, unarmed, and stubborn — protect and pace him). 5) Reach the vehicle before dawn.
- **Stealth/combat setup.** Sparse cover, long sightlines, watchtowers with searchlights. The escort half flips the game into protective stealth — Hossein can be seen independently of Yas.
- **Unique mechanic.** **Escort-in-two-bodies** — you can scout ahead and signal Hossein to move or freeze; mismanaging his exposure is its own fail lane. Wind gusts (audio-haptic) mask both your and his noise on a cycle.
- **Failure states.** Hossein spotted/recaptured (fail). Yas dies. Dawn deadline.
- **Set-piece.** The searchlight gauntlet across the salt flat, father in tow, lights sweeping on the wind's rhythm.

### MISSION 10 — THE COLD RETREAT  *(Act III)*
- **Location.** Azadi's private mountain retreat — snow, cedar lodges, a frozen reservoir, a hardened comms bunker. Isolated, personal, quiet.
- **Story beat.** The moral hinge. Yas learns where Azadi will shelter on succession night. The Qanat is split: kill him and become what they fight, or take the last piece of proof and beat him constitutionally. The mission is built to be completed *either* way and the ending remembers.
- **Objectives (branch).** 1) Infiltrate the retreat (shared). 2) Reach Azadi's study (shared). Then EITHER **[Expose]** copy his private crisis-orders and leave him unaware, OR **[Strike]** confront him — a fight that *can* end lethally. Optional shared: recover a hostage aide who can testify.
- **Stealth/combat setup.** Snow shows tracks (a stealth tell), thermal-camera bunker, small elite guard detail. The most "traditional stealth-assassination" sandbox in the game — then it asks you not to use it that way.
- **Unique mechanic.** **Tracks in snow** — your footprints persist and are read by patrols; you manage a physical trail, using swept paths and hard ground. Reinforces "leave nothing behind."
- **Failure states.** Detection. In [Strike], dying to Azadi's personal guard. In [Expose], being seen (blows the constitutional play).
- **Boss — AZADI (Encounter 2 of 2, optional-lethal).** Only if the player chooses [Strike]. A colder, quieter fight than Mission 2 — no monologue, close quarters in the study, he fights like the old soldier he is. The player may still subdue non-lethally; killing him is possible and shades the finale and epilogue toward tragedy. Choosing [Expose] skips the fight entirely and is the "restraint" path.

### MISSION 11 — THE NIGHT OF CUSTODIANS  *(Act III)*
- **Location.** The capital under curfew — dark boulevards, checkpoints, the Council's members scattered across safehouses and residences. A city-scale, multi-stop mission.
- **Story beat.** Azadi springs his counter-move: with Mahdavi dead, he declares the emergency and orders the loyal Custodians detained before dawn's vote. Yas and the Qanat must reach, warn, and rally enough of the twelve to reach the palace and vote *before* the Guardian Corps locks the city.
- **Objectives.** 1) Reach three named Custodians before Azadi's squads (each a mini-infiltration/extraction, order is player-chosen and affects who survives). 2) Run a checkpoint or two (legitimacy or shadow). 3) Convince the wavering (short dialogue gates using proof gathered in Acts I–II — the game *counts* your evidence). 4) Assemble the quorum at the rally point.
- **Stealth/combat setup.** Chase-and-race structure; time pressure per Custodian; roadblocks. The counter-coup's "loud" mission but rewarding the quiet, fast player with more Custodians saved.
- **Unique mechanic.** **The Quorum ledger** — a live tally of how many Custodians you've secured; the finale's difficulty and the ending's strength scale to it. Evidence you skipped earlier now visibly costs you votes here.
- **Failure states.** Falling below quorum (fail — Azadi wins the vote). A Custodian killed on your watch. Yas dies.
- **Set-piece.** A three-way convoy race to the last Custodian's house as Guardian Corps closes streets — grapple shortcuts across rooftops beat the roadblocks.

### MISSION 12 — VEIL OF SUCCESSION  *(Act III · Finale)*
- **Location.** The palace — where it began. The rose garden is now checkpointed; the grand hall is the vote chamber; the throne room is Azadi's last redoubt.
- **Story beat.** The quorum must physically reach the chamber and vote while Azadi, cornered, tries to seize the moment by force. Yas escorts the Council through the very grounds she once infiltrated alone. Payoff of every system, faction, and choice.
- **Objectives.** 1) Get the Custodians through the checkpointed garden (protect the quorum — inverse of Mission 1). 2) Secure the vote chamber (clear or evade the Guardian detail). 3) Hold the chamber during the vote (defense scaled to your Quorum ledger). 4) Confront Azadi in the throne room. 5) Resolve.
- **Stealth/combat setup.** Full toolkit unlocked; the game's widest sandbox. Defense phase respects non-lethal builds (subdue, don't slaughter — the epilogue tallies it).
- **Failure states.** Vote disrupted (fail). Quorum lost. Yas dies. Azadi escapes the chamber to rally the city (bad-ending fail-forward if certain thresholds unmet).
- **Boss — AZADI (Finale confrontation).** Form depends on Mission 10:
  - If **[Expose]** (spared): Azadi is politically finished mid-fight — the confrontation becomes a tense stand-down; he can be arrested rather than killed. The strongest "restraint" ending.
  - If **[Strike]-nonlethal**: a full third phase-escalated boss (P3 charge + reinforcements) ending in his capture.
  - If **[Strike]-lethal earlier**: the throne room fight is grimmer; the republic survives but Yas carries what she did.
- **Ending.** The Council votes. A moderate successor is confirmed. Hossein and Yas share a spare, unresolved scene. Final card: *"The succession is safe — for now."* Epilogue tallies restraint, evidence, Custodians saved, and Vaziri/Azadi fates into one of several "shades" of the same fragile win (never a clean triumph).

---

## 5. ENEMY ROSTER

Stats reference the prototype baseline (AGuardCharacter MaxHealth 100; speeds patrol 220 / suspicious 420 / chase 600; melee 20 dmg @180uu / 1.2s; perception sight radius 2200, FOV 70°, lose 2600; hearing 1500). Full-game archetypes extend that AI.

1. **Garden Sentry (baseline patrol).** HP 100. The prototype guard. Fixed straight-leg patrol, sight/hearing per spec, non-lethal-friendly (goes down to one dart). Behavior: Patrol→Suspicious(investigate 3s)→Chase. The teaching enemy.

2. **Hall Overwatch (static sniper/spotter).** HP 80. Stationary elevated position, long sight radius (2800), slow turn. Doesn't chase; instead *calls* — raises facility alert and cues nearer guards. Countered by staying out of its cone or darting it (line-of-sight to elevation required).

3. **Corridor Pair (coordinated 2-man).** HP 100 each. Move and clear as a bonded pair; if one is downed and the *other sees it*, instant Suspicious→search. Punishes sloppy takedowns. Teaches isolating targets.

4. **Guardian Trooper (combat-grade).** HP 140, armored (dart takes 2 to drop; body shot to unarmored gap = 1). Carries a rifle: ranged 12 dmg bursts at distance in Chase. Appears Act II+. The "you actually have to think before going loud" enemy.

5. **Technician / Camera-Operator (soft, networked).** HP 60, unarmed, flees on sight but reactivates cameras and re-powers grids you EMP'd. Killing/darting one keeps a camera zone dark longer. Introduces the value of the EMP scrambler and the cost of leaving support alive.

6. **K-9 Handler + Tracker Dog (scent/sound pair).** Handler HP 100; Dog HP 40, fast (speed 640), tracks noise and (Mission 10) snow tracks; ignores standard visual cover, homes on your last-known noise. Countered by distance, water crossings, and staying quiet. The anti-camper.

7. **Minder (plainclothes, social-stealth).** HP 90, no visible weapon. Operates in crowd/ministry/seminary missions; instead of a vision cone he runs a **suspicion read** — challenges you, checks papers, watches pace. Beaten socially (legitimacy) or by breaking his line of sight in crowd. The Act II signature enemy.

8. **Vaziri's Investigator (methodical searcher / elite).** HP 160. Doesn't patrol a fixed leg — *searches*: clears rooms systematically, re-checks places you hid, opens closets, follows disturbed objects. Slow but relentless; models Vaziri's doctrine. Cannot be cheesed by a single hiding spot.

9. **Guardian Shocktrooper (heavy, Act III).** HP 220, riot-armored, immune to a single tranq (needs a called weak-point or EMP-stun then subdue). Slow (patrol 200) but a moving wall in the finale defense. Encourages tools over darts.

**Behavior notes shared across archetypes:** all use the prototype state machine (Patrol/Suspicious/Chasing) extended with a facility-wide **Alert Tier** (calm/heightened/lockdown) that changes patrol density, adds sweeps, and shortens the detection-decay grace. Bodies (lethal or unconscious) left in the open are discovered and raise the tier — carry/hide them.

---

## 6. PLAYER PROGRESSION

### 6.1 Gadgets (the toolkit — scarce, loud, consequential)
- **Tranq Pistol (starting).** The prototype weapon: line-trace ~3000uu while aiming, 50 dmg = one-shot on baseline guards, 12-dart reserve. Quiet but not silent up close; armored enemies need 2. The default answer, never a spray weapon (slow re-aim, limited ammo).
- **EMP Scrambler (M4).** Thrown/placed pulse that kills cameras, door grids, and lights in a radius for a timed window; briefly stuns electronics-heavy enemies (Shocktrooper). Cooldown-gated; Technicians undo it. Core to social-and-tech missions.
- **Listening Probe (M2).** Directional mic to fill eavesdrop bars remotely and to *pre-hear* a room's guard chatter (reveals patrol callouts). Can be spotted if placed in sight. Upgradable to relay through walls.
- **Grapple (M3).** Vertical traversal to marked anchors — rooftops, gantries, mast rungs. Not a combat tool; a route-opener that rewards reading levels three-dimensionally. Silent. Set-piece exits depend on it.
- **Disguise System (M6+).** Context uniforms/robes (Guardian tech coveralls, ministry researcher, pilgrim, staff). A disguise grants a **legitimacy** buffer in matching zones but degrades under scrutiny (Minders, restricted areas) — it's a timer, not a costume you keep forever. Wrong disguise in the wrong zone is worse than none. Ties directly to the Legitimacy meter (§4, M6).

*Deliberately absent:* explosives-as-toys, lethal-first arsenals, gadget-spam. Scarcity keeps every use a decision.

### 6.2 Skill tree — three branches
Points from mission completion, restraint bonuses, and optional objectives. Roughly 8 nodes per branch; you cannot max all three in one playthrough — build identity matters.

**Branch A — SHADOW (movement & evasion).**
Quieter footsteps · faster crouch-move · silent takedown from cover · carry/hide bodies faster · shorter time-to-hidden after breaking line of sight · grapple recovery speed · "ghost read" (see guard vision cones briefly) · capstone **"Vanish"**: one full detection-reset per mission when fully hidden.

**Branch B — CRAFT (tradecraft & tech).**
+2 dart reserve · faster aim/steadier trace · EMP radius & duration · probe wall-relay · pickpocket/forgery success window widened · disguise degrades slower · camera-loop lasts longer · capstone **"Clean Hands"**: non-lethal takedowns are faster and never make a discovery-noise (rewards the pacifist build hardest).

**Branch C — LEGITIMACY (social & influence).**
Higher starting legitimacy · calmer challenge outcomes · read a Minder's suspicion at range · father's-name gates open more doors · persuade wavering NPCs with less evidence · faster trust mini-scenes · checkpoint bluffs · capstone **"The Name"**: once per mission, walk through one legitimacy challenge unquestioned. This branch is the game's mechanical expression of Pillar 1.

### 6.3 Meta-progression & scoring
- **Restraint tally** (non-lethal %, bodies hidden, civilians unharmed) persists across missions and feeds the epilogue "shade."
- **Evidence dossier** (documents, photos, probe intel) is a *literal inventory* that gates dialogue in Missions 7 and 11 — skipped optionals cost votes later.
- No XP grind; progression is authored around act breaks so power scales with the story.

---

## 7. LEVEL DESIGN PRINCIPLES

1. **Two layers, always.** Every space offers a *legitimacy path* (front doors, escorts, papers) and a *shadow path* (vents, roofs, blind spots). Great runs braid them. This is the level-design form of Pillar 1.
2. **Readable before clever.** Landmarks, light, and colour (the prototype's sandstone/hedge/crimson/gold palette) orient the player at a glance; stealth difficulty comes from timing and geometry, never from a confusing map.
3. **Sightline authoring over navmesh reliance.** Inheriting the prototype's straight-leg patrol discipline: patrol routes are legible and honest; the player can *learn* a room and beat it with knowledge, not luck.
4. **Silence is a resource the level spends.** Ambient masking (fountains, prayer calls, crowds, wind, mast hum) is placed deliberately so each level teaches a different relationship to noise.
5. **Verticality earns the grapple.** From Mission 3 on, every level has a meaningful roof/gantry layer so traversal upgrades change how a space is solved.
6. **No dead ends in a chase.** Any space you can be spotted in has at least two exits; detection escalates tension, it doesn't instantly trap.
7. **Set-piece = pressure, not scripting-on-rails.** Each mission's set-piece (searchlights, live window, rope-line, salt gauntlet) is a systemic squeeze the player solves with the toolkit, not a QTE corridor.
8. **The world reacts and remembers.** Alert tiers, discovered bodies, snow tracks, burned evidence — levels visibly respond to how you play, reinforcing consequence.
9. **Return with new eyes.** Mission 1 and Mission 12 share the palace; the finale re-reads familiar geometry from the protector's side. Reuse is a narrative device, not a budget crutch.

---

## 8. DIFFICULTY MODES

- **Custodian (Story).** Forgiving detection decay, generous darts, watchers give more warning, no hard deadline fails (soft timers). For players here for the thriller. Restraint still tallied.
- **Operative (Standard).** The intended balance — prototype detection tuning, real deadlines, meaningful scarcity.
- **Qanat (Hard).** Faster detection, tighter ammo, smarter searches (Investigators re-check more spots), shorter grace before lockdown, deadlines strict.
- **Veil (Ironman/Expert, unlock).** Qanat rules + no manual mid-mission saves + one-life act-runs; a single detection can end an act. For the "if she's caught, he hangs" fantasy taken literally.
- **Custom sliders.** Independent toggles: detection speed, enemy density, deadline strictness, aim assist, forgiveness of set-pieces — so the difficulty and accessibility systems overlap rather than fight.

Non-lethal play is viable and *rewarded* on every mode; no difficulty forces killing.

---

## 9. NARRATIVE DELIVERY & TONE SYSTEMS

- **Earpiece (Darya).** Primary storytelling channel — reactive, dry, never hand-holding; comments on restraint and route choices. Full lines for the prototype missions in MISSION_SCRIPTS.md.
- **Overheard world.** Guards, Custodians, minders, and crowds carry plot in ambient dialogue; the Listening Probe is a narrative tool as much as a mechanical one.
- **Documents.** The evidence dossier doubles as environmental storytelling — every photographed transfer and stolen file reads as prose.
- **Restraint messaging.** Fail and success screens name Hossein and count bodies; the UI itself enforces the tone. (Prototype fail card: *"COVER BLOWN — Yasaman's mission ends here."*)
- **No gore-forward violence.** Takedowns are controlled and clinical; the camera does not linger on death. Le Carré, not the morgue.

---

## 10. AUDIO DIRECTION

- **Score.** A **santur** (Persian hammered dulcimer) as the emotional lead, woven with restrained modular-**electronic** underscore — analog pulses and low drones for tension, the santur for the human/family register. The two never resolve fully; the music, like the story, stays unresolved.
- **Adaptive layers.** Music tracks the Alert Tier: calm = solo santur and room tone; heightened = electronic pulse enters, tempo tightens; lockdown = full stems, dissonant santur clusters. Detection near-miss briefly ducks music for a held breath.
- **Diegetic masking.** Fountains, the call to prayer, bazaar crowd, desert wind, the transmitter's 50-cycle hum — each level's signature masking sound is also its sonic identity.
- **Voice.** Grounded, quiet performances; Yas dry and controlled, Azadi soft-spoken and reasonable (more menacing for it), Hossein weary and principled. No shouting-as-tension; the rot is quiet.
- **Silence as a cue.** The score drops out at the highest-stakes beats (the eavesdrop, the live window, the salt gauntlet) so the player hears only footsteps and their own restraint.
- **3D audio / Tempest.** Full PS5 3D audio: guard footsteps, callouts, and the direction of a watcher's attention are spatialized — listening is a core sense, not flavour.

---

## 11. DUALSENSE FEATURES (PS5-FIRST)

- **Adaptive-trigger resistance on aim.** Pulling L2 to raise the tranq pistol meets a rising resistance and a "wall" at full aim — the player *feels* the steadied shot. R2 has a crisp break past the wall for the dart release. Overcoming the wall too fast = a rushed, less accurate shot.
- **Haptic heartbeat when nearly detected.** As detection crosses the near-caught threshold (prototype: meter >0.7, turns red), the controller carries a low, quickening heartbeat pulse — the player feels exposure before the fail. It slows as detection decays. This is the signature feel of the game.
- **Tradecraft micro-haptics.** Pickpocket (M1) — resistance-then-give as the keycard eases free. Stamp-and-seal (M6) — the mechanical thunk of an old date-stamp. Grapple — a taut-line snap on anchor. Probe placement — a soft tick when the mic locks a target.
- **Surveillance taps.** In social-stealth missions, a directional haptic tap pulses toward a watcher currently reading you (the tail-read, M3), teaching surveillance geometry through touch.
- **Environmental texture.** Footfall haptics differ on gravel, carpet (the crimson strip), marble, snow (M10 tracks), and salt (M9) — texture underfoot is also a stealth tell.
- **Adaptive-trigger states as feedback.** A jammed/empty tranq gives a dead, resistance-less trigger; an EMP on cooldown gives a stiff, unyielding one — tool status read through the hands.
- **Mic (optional).** Player noise-making (blowing into the mic / real-world sound) is *not* used to detect the player — accessibility first; all noise is in-fiction and controller-driven.

---

## 12. ACCESSIBILITY

- **Difficulty & custom sliders** (see §8) decouple challenge from access: detection speed, aim assist, deadline strictness, set-piece forgiveness, enemy density — all independent.
- **Non-lethal is fully viable** on every mode; players uncomfortable with violence can complete the whole campaign without a kill and be rewarded for it.
- **Full remapping**, toggle/hold options for every hold action (aim, sprint, crouch, grapple), and one-button-takedown options.
- **Haptic and audio redundancy.** Every haptic cue (heartbeat detection, surveillance tap) has a visual and audio equivalent (the detection meter, directional markers) — no mechanic is haptics-only. Conversely, audio stealth cues have captioned/visualized equivalents (a directional "noise" indicator) for deaf/HoH players.
- **Full subtitles & captions** — dialogue, earpiece, overheard lines, and important ambient sound, with speaker names, size/background/colour options. Lip-sync and captioning authored in **Farsi and English** from day one (see roadmap).
- **Colourblind modes** for the detection/legitimacy meters and objective markers (never colour-only state).
- **Reduced-motion / camera-shake / flash toggles** (relevant to searchlights, EMP flashes, mast strobes).
- **Screen-narration & high-contrast HUD** options; scalable HUD/subtitle text.
- **Guided objectives / ping** toggle (an optional stronger objective marker and route hint for players who want less navigational load) separate from difficulty.
- **Reverence/violence content notes** surfaced in settings so players know the seminary and prison missions' tone in advance.

---

## 13. SCOPE SUMMARY (for the roadmap)
- 12 handcrafted missions, 3 acts, ~10 hours critical path.
- 9 enemy archetypes; 4 named bosses/duels (Azadi ×2, Vaziri ×2).
- 5 gadgets + disguise system; 3-branch skill tree.
- Dual-language (Farsi/English) full VO and text.
- PS5-first DualSense feature set, 1080p/60 target.

*Continues in ROADMAP.md for production plan and MISSION_SCRIPTS.md for prototype-mission scripts.*

---

## 14. CORE GAMEPLAY LOOP (MOMENT-TO-MOMENT)

The 30-second loop, extended from the prototype's detection/noise model:

1. **Read the room.** Enter a space; use the camera, listening probe pre-hear, and (if skilled) the "ghost read" vision-cone glimpse to map patrols, cameras, cover, masking sound, and the two path layers (legitimacy / shadow).
2. **Choose an approach.** Front-door legitimacy (papers, escort, disguise), shadow route (vents, roofs, blind spots), or a braid of both. Objective type (steal, eavesdrop, photograph, persuade, escort) shapes the choice.
3. **Manage exposure.** Move to keep detection low; crouch-still is 0 noise, sprint is loud. Break line of sight to cool down. Watch the detection meter (top-center) and feel the haptic heartbeat past the near-caught threshold.
4. **Neutralize only when you must.** Dart, subdue, or slip past. Every downed body is a liability that must be hidden before discovery raises the Alert Tier.
5. **Resolve the objective, leave no trace.** Complete the beat, cover it (re-hide the probe, sweep tracks, restore power a Technician might re-flag), and exit clean. The epilogue tallies restraint.

### 14.1 Detection model (extends prototype)
- **Global exposure** `0..1` (as prototype `GetDetection`): guards add to it; it decays −0.15/s with no contributor. Crosses **0.7** = red meter + haptic heartbeat = near-caught. Reaching 1.0 with an active hunter = COVER BLOWN.
- **Per-guard suspicion** accumulates from a sight stimulus, scaled by distance and `StealthComponent` visibility multiplier (crouched .45, standing 1.0, sprinting 1.6). Full suspicion → Chase.
- **Alert Tier** (facility-wide): *Calm → Heightened → Lockdown.* Rises from noise, discovered bodies, alarms; each tier increases patrol density, adds sweeps, and shortens decay grace. Falls with sustained silence over time. This is the systemic layer above the prototype's per-guard machine.
- **Legitimacy meter** (inverse detection, social missions): high when you act like you belong; decays in restricted zones; at zero you're an intruder and the shadow game begins.

### 14.2 Noise model
Discrete surfaces and actions produce a noise radius guards hear (prototype hearing range 1500): crouch-still silent, walk small, sprint large; hard surfaces (marble, salt) louder than soft (carpet, snow); tools (tranq up close, EMP pop, grapple snap) each have a signature radius. Masking sound (fountains, prayer call, crowd, wind, mast hum) subtracts from effective noise — the level's masking bed is a resource.

### 14.3 Takedown flow
Approach unseen → prompt → non-lethal subdue (choke) or lethal (contextual, avoidable) → the body is now physics/liability → carry & hide (skill-tree speeds this) before a patrol's search finds it. Corridor Pairs and coordinated enemies punish taking one down in the other's sight.

---

## 15. BOSS & DUEL DESIGN (DEEP DIVE)

At least four named encounters across the campaign; two are Azadi, two are Vaziri. They are built as *distinct verbs*, not four health bars.

### 15.1 General Azadi — Encounter 1 (Mission 2, prototype boss)
- **Verb: survive the reasonable man.** MaxHealth 600, phases at 100/66/33% (prototype contract). P1 telegraphed pistol volleys (0.8s tell, dive between columns). P2 summons 2 reinforcements. P3 speed ×1.5 + charge (bait & sidestep).
- **Design intent.** He is *not trying to kill her* — he wants the dossier to lead him to The Qanat, so he "chooses to lose." Non-lethal resolution (dart) is canon and rewarded. Teaches the player that beating Azadi with restraint is possible and correct.

### 15.2 Colonel Vaziri — Duel 1 (Mission 4)
- **Verb: evade the methodical searcher.** Not a brawl — a **pursuit puzzle**. Vaziri clears the records annex room-by-room, re-checking hiding spots (Investigator behavior). "Winning" = escaping with proof while she searches the wrong floor. Killing her is possible but darkens Act II and the ending; the design steers hard toward evasion.

### 15.3 Colonel Vaziri — Duel 2 (Mission 8)
- **Verb: consequence of restraint.** Rematch on the transmitter gantries. If Yas spared her people in Duel 1, Vaziri hesitates at the key beat and can be *stood down* — a scripted payoff where restraint is mechanically and narratively decisive. If Yas was brutal before, no mercy: a hard non-lethal chase over the humming mast. The same encounter, two shapes, decided by earlier play.

### 15.4 General Azadi — Encounter 2 & Finale (Missions 10 & 12)
- **Verb: the moral hinge, then the reckoning.** Mission 10 [Strike] is a colder, close-quarters study fight (optional-lethal); [Expose] skips the fight for the constitutional path. Mission 12's finale form is *determined by* Mission 10: a stand-down/arrest (Expose), a full phase-escalated capture (Strike-nonlethal), or a grim throne-room fight (Strike-lethal). The boss's final shape is the player's ethics reflected back.

**Shared boss principles.** Every boss can be resolved non-lethally; the world and epilogue remember which you chose; telegraphs are readable (Le Carré fights are legible, not chaotic); no boss is a bullet-sponge gate — each is a *test of a specific skill* the mission taught.

---

## 16. ECONOMY & PACING (per act)

Scarcity is tuned so tension never fully relaxes. Indicative Standard ("Operative") values:

| Resource | Act I | Act II | Act III | Notes |
|----------|-------|--------|---------|-------|
| Tranq darts (reserve) | 12 (prototype) | 12–16 | 16–20 | +2/branch node; refills scarce, found not bought |
| EMP charges | — → 1 | 1–2 | 2–3 | Introduced M4; Technicians undo effects |
| Grapple | — → yes | yes | yes | Introduced M3; unlimited use, route-gated by anchors |
| Disguises | — | context | context | Timed legitimacy buffer, not permanent |
| Skill points | ~6 | ~8 | ~8 | Cannot max all three branches in one run |

**Pacing rhythm per act:** two quiet infiltrations bracketing one "loud" mission (Act I: M3 chase between M1–2 and M4; Act II: M5 defense; Act III: M9 break / M11 counter-coup). Act climaxes escalate personal cost: I ends with Azadi hunting Yas by name; II ends with Hossein arrested to bait her; III ends with the fragile win. Story scenes are front-and-back-loaded per mission so the middles stay playable.

---

## 17. ENEMY STAT REFERENCE (TABLE)

Baseline from prototype `AGuardCharacter`. Speeds: patrol / suspicious / chase.

| # | Archetype | HP | Speeds | Darts to drop | Signature behavior | Counter |
|---|-----------|----|--------|--------------|--------------------|---------|
| 1 | Garden Sentry | 100 | 220/420/600 | 1 | Fixed straight-leg patrol, Patrol→Susp→Chase | The teaching enemy; time the legs |
| 2 | Hall Overwatch | 80 | static | 1 (LOS to height) | Calls/raises alert, doesn't chase | Stay out of cone; dart from elevation |
| 3 | Corridor Pair | 100 ea | 220/420/600 | 1 ea | Bonded; sees partner downed → search | Isolate before takedown |
| 4 | Guardian Trooper | 140 | 220/420/600 | 2 (1 to gap) | Ranged rifle bursts in chase (12 dmg) | Think before going loud |
| 5 | Technician | 60 | flees | 1 | Re-powers grids, reactivates cameras | Down them to keep zones dark |
| 6 | K-9 Handler + Dog | 100 / 40 | dog 640 | 1 / 1 | Tracks noise & snow, ignores visual cover | Distance, water, silence |
| 7 | Minder (plainclothes) | 90 | social | n/a (no draw) | Suspicion read, challenges papers | Legitimacy or break LOS in crowd |
| 8 | Vaziri Investigator | 160 | slow/relentless | 2 | Systematic room-clear, re-checks hides | Keep moving; no single hidey-hole |
| 9 | Guardian Shocktrooper | 220 | 200/—/slow | EMP-stun then subdue | Riot-armored moving wall (Act III) | Tools over darts; weak-point |

All share the Patrol/Suspicious/Chasing core (prototype), extended by the Alert Tier and, per archetype, search / callout / discover-body / social-read behaviors.

---

## 18. HUD & CAMERA REFERENCE (extends prototype)

- **HUD (from prototype `AVeilHUD`, rebuilt in UMG):** mission title + current objective (top-left); health bar + dart count (bottom-left); detection meter (top-center, red >0.7); eavesdrop/legitimacy progress bars contextually; boss health bar (top-center when active); dot crosshair when aiming; full-screen fail ("COVER BLOWN — Yasaman's mission ends here") and victory ("THE SUCCESSION IS SAFE — for now.") cards; controls hint. All states get colourblind-safe and captioned equivalents (§12).
- **Camera (from prototype):** third-person spring-arm length 380, socket offset (0,60,70), pawn-control rotation; aiming zooms to 180 (interp 10). Character orients to movement except while aiming. Full-game adds cover-cam framing, cinematic story-scene cameras, and photo-mode.

---

## 19. OPTIONAL OBJECTIVES & REPLAY

- Each mission carries **1–3 optional objectives** (extra evidence, an asset saved, a no-detection clear, a no-KO clear) that feed the evidence dossier and restraint tally — and cash out later (Missions 7 & 11 dialogue gates, finale strength).
- **New Game+** carries skill build and restraint record; raises Alert-Tier baselines and enables Veil mode.
- **Mission-select replay** post-campaign with per-mission restraint/speed/detection scoring, for the stealth-mastery audience — never required, always rewarded.

---

## 20. WORLD & FACTIONS (design-facing)

The setting is a near-future, Tehran-inspired capital of an unnamed-in-dialogue Islamic republic. All institutions are fictional; the design never references a real state, official, or organization. Three forces define the board:

### The Council of Custodians
Twelve senior clerics and officials who constitutionally choose the Supreme Guide's successor. Not a monolith — a spectrum from Azadi's clients to Hossein's moderates to the frightened middle. **Mechanically**, the Council is the game's *stakes made countable*: the evidence dossier and the Quorum ledger track how many you can move, and the finale scales to it. Individual Custodians (Qomi, Feyzi, Borujerdi as a "curtain," Hossein) are named touchpoints across missions.

### The Guardian Corps (Azadi's instrument)
The intelligence directorate Azadi commands — the game's antagonist faction and the source of most enemy archetypes (sentries, troopers, technicians, minders, shocktroopers, K-9, Vaziri's investigators). **Design role:** the visible, competent, *reasonable-seeming* apparatus of the managed succession. They are ordinary professionals, not monsters — the barks and behavior keep them human, which is what makes the rot quiet rather than cartoonish. Their escalation is the Alert Tier.

### The Qanat (Yas's network)
A back-channel network of constitution-loyal officers — Darya (handler), Kaveh (field/exfil), the cryptographer, the safehouse keepers. **Design role:** the player's support layer — earpiece guidance, gadgets, exfil, and the moral center that argues *against* becoming what they fight (the Mission-10 split). Named after the underground water channels that keep a desert city alive unseen — the game's central metaphor for constitutional loyalty running quiet beneath the surface.

**Faction reactivity.** Player restraint shifts how each faction reads Yas by the epilogue: a bloodless campaign leaves the Guardian Corps able to dismiss her as a "rumor," the Council able to act openly, and the Qanat intact; a violent one hardens all three and darkens the ending shade (§22).

---

## 21. CHOICE & CONSEQUENCE

The game is linear in mission order but *branches in how the world remembers you*. Four persistent systems, all surfaced honestly to the player:

1. **Restraint tally (continuous).** Non-lethal %, bodies hidden, civilians unharmed, sacred-space respect. Persists all game; feeds the ending shade and faction reactivity. Never blocks progress — always colors it.
2. **Evidence dossier (accumulative inventory).** Documents, photos, probe intel — a *literal* inventory. Skipped optionals are visibly missing when Missions 7 and 11 gate persuasion on evidence, and when the finale's Quorum strength is tallied. The game *counts* your diligence.
3. **The Vaziri thread (relational).** Sparing her people in Duel 1 (M4) unlocks the stand-down in Duel 2 (M8); brutality forecloses it. One recurring human whose arc you author by conduct.
4. **The Azadi hinge (M10 Expose vs. Strike).** The single largest branch: expose him constitutionally (restraint path, skips the fight) or strike (optional-lethal). Determines the finale boss's *form* (M12) and the ending shade. Neither is "the good path" mechanically punished — Strike-nonlethal and Expose both lead to a survivable republic; only Strike-lethal shades toward tragedy.

**Quorum ledger (M11→M12).** How many Custodians you reach and convince (using accumulated evidence) sets the finale's defense difficulty and the vote's margin. Evidence skipped in Acts I–II literally costs votes here — the clearest expression of "diligence compounds."

Design rule: **consequences are legible, not gotchas.** The player is told what restraint tracks, what evidence is for, and that Azadi's fate is theirs to choose. The ambivalence is authored, not accidental.

---

## 22. ENDING SHADES

There is one *outcome* — the succession is stabilized, constitutionally, for now — and several *shades* of it, blended from the persistent systems above. This honors the canon ending: a victory that fixes nothing permanently.

- **"The Quiet Win" (high restraint, full evidence, Expose, Vaziri spared, strong quorum).** Azadi is arrested and politically finished; no one Yas didn't have to hurt was hurt; a moderate successor confirmed with a clear margin. Hossein and Yas share a spare, almost-warm scene. The strongest, cleanest breathing space — still only "for now."
- **"The Costed Win" (mixed restraint / some evidence gaps / Strike-nonlethal).** The republic holds, but by a thin margin; some Custodians were lost on the Night; Yas carries the weight of what the week required. Hossein is alive but changed. The default, most-earned shade.
- **"The Pyrrhic Win" (low restraint, Strike-lethal, bodies, weak quorum).** Azadi is dead; the vote passes but the new order is born already fearful, and Yas has become a little of what she fought. Hossein survives but the reunion is cold. The tragedy shade — still a win, and it still fixes nothing.
- **Fail-forward bad outcome (below quorum / thresholds unmet in M12).** Azadi escapes the chamber to rally the city — a defeat state, not a shade, restart-gated.

Every shade closes on the same card — **"THE SUCCESSION IS SAFE — for now."** — but the epilogue text, the Hossein scene, and the faction summaries differ. The line means something different in each mouth.

---

## 23. INPUT & CONTROLS REFERENCE (full, PS5-first)

From the prototype input map; PS5 DualSense = standard UE gamepad keys. All remappable; all holds have toggle options (§12).

| Action | Keyboard/Mouse | DualSense | Notes |
|--------|----------------|-----------|-------|
| Move | WASD | Left Stick | Axis2D |
| Look | Mouse | Right Stick | pawn-control rotation |
| Sprint (hold) | Left Shift | Cross (Face Bottom) | raises noise/visibility 1.6× |
| Crouch (toggle) | C | Circle (Face Right) | visibility .45× |
| Aim (hold) | Right Mouse | L2 | adaptive-trigger resistance; zoom to 180 |
| Fire | Left Mouse | R2 | tranq dart; crisp break past aim wall |
| Interact | E | Square (Face Left) | 250uu sphere; pickpocket/probe/etc. |
| Gadget (full game) | Q / mouse-wheel | L1 / R1 | EMP, grapple, disguise select |
| Restart (fail) | R | Options / Special-Right | reloads current level |

**Camera:** spring-arm 380, socket offset (0,60,70); aim zoom 180 (interp 10); orient-to-movement except while aiming. **DualSense signature feel:** adaptive-trigger aim wall (L2) + haptic heartbeat past detection 0.7 (§11).

---

## 24. PRODUCTION-FACING FEATURE LIST (traceability to prototype)

| Feature | Prototype state | Full-game state |
|---------|-----------------|-----------------|
| Player controller, tranq, camera | Shipped (C++) | Blueprint-tuned subclass; gadgets added |
| Detection/noise model | Shipped (global + per-guard) | + Alert Tier + Legitimacy meter |
| Guard AI | Shipped (C++ state machine, no navmesh) | BT + navmesh + EQS + 9 archetypes |
| Boss (Azadi) | Shipped (3-phase, 600 HP) | + Vaziri ×2, Azadi ×2, branch-aware |
| Objective/mission subsystem | Shipped (hard-coded M0–M2 chain) | Data-driven, 12 missions, save system |
| Level | Shipped (code-built palace) | 12 authored streamed levels, real art |
| HUD | Shipped (canvas draw) | UMG, localized, accessible |
| DualSense | Control map proven | Full adaptive-trigger/haptic feature set |
| Dialogue | (this doc, prototype scripts) | Full dual-language VO, reactive banks |

This mapping is the contract between what was built and what ships: **every prototype system has a named growth path; nothing is thrown away.**

---

## 25. MECHANIC TEACHING CADENCE

Every mission introduces or deepens exactly one idea, so the 10 hours build a vocabulary rather than dump it. Nothing is taught twice; nothing arrives untaught.

| # | Mission | New mechanic introduced | Deepens | Player fantasy taught |
|---|---------|------------------------|---------|-----------------------|
| 1 | Wall of Roses | Core stealth, pickpocket, detection | — | "I can move like I belong." |
| 2 | Custodians Convene | Listening probe, eavesdrop, first boss | Stealth | "Restraint beats the reasonable man." |
| 3 | Long Bazaar | Crowd-blend, tail-read, grapple | Detection→social | "I can be hunted and slip it." |
| 4 | Ledger of Ashes | EMP scrambler, burn timer, methodical-searcher duel | Cameras/tech | "Loud play destroys my own goal." |
| 5 | House of Safe Rooms | Reactive raid escalation, defense-quietly | Noise economy | "Silence controls the siege." |
| 6 | Ministry Archives | Legitimacy meter, disguise, forgery | Social stealth | "Belonging is a tool I manage." |
| 7 | Seminary | Prayer-cycle timing, weapons-holstered stealth | Timing/legitimacy | "Rhythm, not just shadow." |
| 8 | Broadcast Standard | The live window, Vaziri stand-down | EMP/timing/relations | "One immovable moment to plan around." |
| 9 | Salt Prison | Escort-in-two-bodies, exposure management | Sparse-cover stealth | "I protect, not just infiltrate." |
| 10 | Cold Retreat | Snow tracks, the moral hinge | Trace management | "My choices are the mechanic." |
| 11 | Night of Custodians | Quorum ledger, race/checkpoints | Evidence payoff | "My diligence compounds into votes." |
| 12 | Veil of Succession | Full-toolkit sandbox, branch-aware finale | Everything | "I earned this fragile win." |

### 25.1 Onboarding philosophy
- **Teach in the fiction.** Darya's earpiece is the tutorial voice; no menu-page tutorials. The prototype already models this (M0 lines walk pickpocket and spotlights diegetically).
- **Show, gate, release.** Each new tool is introduced in a low-stakes beat, required once in a controlled beat, then left to the player's discretion. Difficulty comes from combination, not from withholding.
- **Optional depth.** Advanced expression (ghost read, no-detection clears, disguise chains) is available early to skilled players but never required, so mastery and access coexist (ties to §8 sliders and §12).

### 25.2 Difficulty curve
Tension rises across acts via *systemic* pressure, not stat inflation: Act I teaches the core and the reasonable-man boss; Act II layers social stealth, tech, and immovable timing while the world starts remembering (Alert Tier, Vaziri thread); Act III adds escort/protection stakes, the moral hinge, and the count-everything finale. Each act's "loud" mission is the local peak; each act's ending is the emotional peak. New Game+ and Veil mode raise baselines for mastery players without rebalancing the authored curve.

---

## 26. AUDIO CUE LIST (design-facing, ties to §10)

Signature cues the score/SFX systems must deliver, so audio and design stay in lockstep:
- **Detection stinger ladder:** a single santur note (curious) → a tightening electronic pulse (heightened) → dissonant cluster + full stems (lockdown); mirrors the Alert Tier and the meter/haptic.
- **Near-caught held breath:** music ducks to near-silence + heartbeat haptic at detection >0.7, resolving as it decays.
- **Objective-complete motif:** a small, unresolved santur phrase — satisfaction without triumph (tone rule).
- **Set-piece silence:** score drops out entirely at the eavesdrop, the live window, and the salt gauntlet — footsteps and breath only.
- **Faction motifs:** Guardian Corps = cold electronic; The Qanat = warm santur; the Council = a slow, formal drone. They never harmonize fully.
- **Ending:** santur-only over the final card, thrown away, no swell — the same restraint the whole game asks of the player.

---

## 27. POSITIONING & COMPARABLES

*Design reference points, to align the team on feel — not a promise to copy.*

- **Stealth core:** the readable-patrol, learn-the-room, non-lethal-viable school (immersive-sim-lite). We inherit the prototype's honest sightlines rather than randomized guard soup.
- **Third-person espionage staging:** grounded over acrobatic; the camera serves character acting and tension, not parkour spectacle.
- **Tone target:** John le Carré and quiet political thrillers — bureaucratic dread, moral compromise, the beautiful surface over quiet rot. Explicitly *not* the Bond/blockbuster register, even though Yas has a "007 sensibility." The gap between her competence and the smallness of her stakes (one father, one week) is the whole point.
- **Narrative-choice model:** consequences that shade an ending rather than fork the plot — the player authors the *color* of an inevitable, fragile win, not a branching tree of triumphs.
- **What makes it its own thing:** the **legitimacy layer** (the name opens doors) as a first-class mechanic sitting beside classical shadow-stealth, and a stakes framing where getting caught is not "reload" but "your father hangs with you." No comparable centers a daughter-and-father political stake inside a stealth game.

**Elevator pitch (one line):** *A le Carré stealth thriller where a spymaster's daughter must save a dying republic's succession without becoming the strong hand she's fighting — and where being caught doesn't just kill her, it kills her father.*

---

## 28. PROTAGONIST EXPRESSION (Yas in systems)

Yasaman's character must be *felt in play*, not only in cutscenes:

- **Control under pressure → animation & camera.** Yas never panics; even sprinting from a chase she moves economically. Locomotion and the near-caught state read as *composure fraying*, not flailing. The haptic heartbeat is her heartbeat — the one thing she can't fully control.
- **Dry humor → earpiece cadence.** Her lines land flat and quick (see idle/tradecraft bank in MISSION_SCRIPTS). The writing rule: she deflects fear with dryness, never with quips-for-quips'-sake.
- **The name as power → the Legitimacy branch.** Her father's standing is literally a skill branch and a meter. Playing "legitimate" is playing *as a Rahimi*; playing "shadow" is playing *as a Qanat operative*. The two identities are the two halves of her.
- **A daughter's stakes → the fail screen.** The UI names Hossein on failure. Restraint is characterized as *her* choice, not just an optimal strategy — the epilogue frames non-lethality as who she decides to be.
- **Educated-abroad outsider → world reactivity.** NPCs occasionally read her as not-quite-belonging (Minders, the seminary); the game uses her outsider status as friction, not a superpower.

---

## 29. GLOSSARY (shared canon — binding across all docs)

*All fictional. The restraint rule forbids substituting any of these with a real person, state, or organization in dialogue, UI, marketing, or localization.*

- **Ayatollah Mahdavi** — the dying Supreme Guide whose succession the game concerns. Never seen in the prototype; his death triggers Act III.
- **Council of Custodians** — the twelve senior clerics/officials who constitutionally choose the successor. The game's stakes made countable.
- **General Farrokh Azadi** — antagonist; commander of Guardian Corps intelligence; architect of the "managed succession." Boss ×2.
- **Guardian Corps** — Azadi's intelligence directorate; the antagonist faction and source of the enemy roster.
- **Hossein Rahimi** — Yas's father, a moderate Custodian; suspects the plot, will not flee. The emotional stake.
- **Colonel Nasrin Vaziri** — Azadi's counter-intelligence chief; the recurring "investigation" boss (duel ×2).
- **The Qanat** — the constitution-loyal back-channel network Yas serves; the player's support faction. Named for the underground water channels that keep a desert city alive unseen.
- **"Darya"** — Yas's Qanat handler; the earpiece voice; the tutorial and moral-center voice.
- **Captain Omid Shirazi** — palace guard captain whose keycard opens the inner doors (M0); recurs as a reluctant asset.
- **The succession dossier** — Azadi's blackmail files on all twelve Custodians ("the inventory"); the M1–M2 objective and the plot's central object.
- **Managed succession** — Azadi's plan: stage a foreign-sabotage crisis, declare emergency powers, install a pliable caretaker (Sheikh Borujerdi, "the curtain"), and rule from behind the veil.
- **Yasaman "Yas" Rahimi** — the protagonist. 29, educated abroad, Qanat-trained, Custodian's daughter. Elegant, controlled, dry.

**The binding line (repeat across every deliverable):** *The republic, its offices, and everyone in them are fictional. Keep them that way.*

---

## 30. SYSTEMS NOT IN THE PROTOTYPE (required for ship)

Named here so they are scheduled, not discovered late (cross-referenced in ROADMAP.md):

- **Save & checkpoint.** Auto-checkpoints at objective boundaries + difficulty-gated manual saves (none on Veil). The prototype's restart-on-fail becomes a full save system for a 10-hour game. Restart still reloads the current mission cleanly.
- **Performance modes.** Performance (1080p/60, lead target) and Quality (4K/30) toggles; Lumen/Nanite budgets tuned per mode.
- **Photo mode.** Fits the "beautiful palace, quiet rot" art direction; free camera, filters, character posing — a low-cost engagement/marketing feature, out of critical path.
- **Skill-tree & dossier UI.** The three-branch tree (§6.2) and the evidence inventory (§21) need full UMG screens; absent in the prototype.
- **Disguise & legitimacy systems.** New systems layered on the prototype's detection model (§6.1, §14.1) — the largest net-new gameplay R&D after the AI migration.
- **Cinematics & facial performance.** Story scenes (MISSION_SCRIPTS.md) need a cinematic camera + MetaHuman facial pipeline; the prototype has none.
- **Two-language VO integration.** Runtime language switching, dual lip-sync, and the reactive-bank system driving barks off gameplay state.

Each maps to a workstream and phase in the roadmap; none conflicts with the prototype's C++ contracts — they extend them.

---

*End of Game Design Document. Companion documents: MISSION_SCRIPTS.md (prototype narrative scripts) and ROADMAP.md (production plan). All three keep the STORY_BIBLE canon binding: the republic and everyone in it are fictional.*
