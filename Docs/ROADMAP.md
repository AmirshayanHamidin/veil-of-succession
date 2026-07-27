# VEIL OF SUCCESSION — Production Roadmap (Prototype → Full Game)
*A work of political fiction. All persons, offices, and organizations are fictitious. This is the practical plan for taking the delivered UE5 C++ blockout prototype to the full ~10-hour campaign described in GDD.md.*

---

## 0. WHERE WE ARE (the delivered prototype)

**What exists.** A UE 5.3 C++ project (`VeilOfSuccession`), zero content assets, everything spawned from code:
- `ASpyCharacter` + `ASpyPlayerController` — player, tranq pistol, Enhanced Input (built dynamically), spring-arm camera, detection/noise, restart.
- `UStealthComponent`, `AGuardCharacter`, `AGuardAIController`, `ABossCharacter` (Azadi) — C++ perception + Patrol/Suspicious/Chasing state machine, no navmesh (straight-leg `AddMovementInput`), 3-phase boss.
- `UMissionSubsystem`, `AVeilGameMode`, `APalaceLevelBuilder`, `AVeilHUD`, `AInteractableActor` — objective chain (M0/M1/M2), code-built palace (walls, garden, hall, chamber, throne room), canvas HUD.
- Blockout visuals: engine BasicShapes (Cube/Cylinder/Sphere/Plane) + runtime-colored `BasicShapeMaterial`; capsule + shape "characters."

**What this proves.** The full gameplay loop — infiltrate, eavesdrop, steal, boss, extract — plus detection tuning, non-lethal balance, objective flow, and the DualSense-first control map. It is a *vertical slice of systems*, not of fidelity.

**The gap to the full game.** Everything that makes it look, animate, and stream like a shipped title, plus 9 more missions of content and a second language. The rest of this document is that gap, sequenced.

---

## 1. GUIDING PRINCIPLES FOR THE PORT

1. **Keep the C++ contracts, swap the presentation.** The cross-class signatures in SPEC (`ASpyCharacter`, `UMissionSubsystem`, `AInteractableActor`, etc.) are good architecture. Real assets should be *driven by* these systems, not replace them. Blueprints subclass the C++ classes for designer-facing tuning; C++ stays authoritative.
2. **Data-drive the missions.** The prototype hard-codes one objective chain and one code-built level. Refactor `UMissionSubsystem` to load objective chains from a **DataTable/DataAsset per mission** and replace `APalaceLevelBuilder` with real streamed `.umap` levels. This is the single most important structural change.
3. **Blockout-first level design survives.** The straight-leg patrol discipline and readable geometry from `APalaceLevelBuilder` are the level-design *intent*; art dresses over the greybox without moving the sightlines that make stealth fair.
4. **Ship-quality on PS5 target from day one.** Profile 1080p/60 continuously; don't let fidelity work outrun the frame budget.

---

## 2. WORKSTREAM A — ART: REPLACING BLOCKOUT WITH REAL ASSETS

### 2.1 Environment art
- **Method.** Keep `APalaceLevelBuilder`'s output as an in-editor greybox reference, then build the 12 mission `.umap`s by hand in-editor, matching sightlines and patrol legs. Migrate from code-spawned BasicShapes to Static/Nanite meshes.
- **Nanite + Lumen.** Use Nanite for architectural detail (colonnades, muqarnas, carved screens) and Lumen for the "beautiful palace, quiet rot" lighting mood. Budget Lumen carefully against 60fps — consider baked lighting fallback on the perf mode.
- **Asset sourcing.**
  - **Quixel Megascans** (free in-engine): stone, plaster, tilework surfaces, rubble, foliage — the backbone of the sandstone/desert/mountain palettes.
  - **UE Marketplace / Fab**: Middle-Eastern & Persian architecture kits, bazaar/market packs, palace interior kits, prison/industrial kits, snow/mountain kits, TV-studio and office kits — as *kitbash bases*, always re-dressed so the world reads as one fictional country, not a pack.
  - **Custom hero assets**: the Peacock-Throne dais, the succession-dossier case, key set-piece props, signature santur-motif set dressing — bespoke, not Marketplace.
- **Level palette continuity.** Preserve the prototype's colour language (sandstone, hedge green, crimson carpet, gold dais) as the art bible's anchor so Mission 1 and Mission 12 (shared palace) stay legible.
- **Vegetation/weather.** Foliage tool for rose gardens/seminary; Niagara for desert wind, snow, searchlight volumetrics, transmitter haze.

### 2.2 Character art
- Replace capsule + shape blockout characters with skeletal meshes.
- **Player & named cast (Yas, Azadi, Hossein, Darya cameo, Vaziri, Bijan, Banu):** **MetaHuman** for faces/performance capture fidelity; custom body/outfit meshes over MetaHuman rigs. This gives the Le Carré close-up acting the story needs.
- **Enemy archetypes (9, per GDD §5):** a smaller set of modular MetaHuman/mannequin-based bodies with uniform/kit variation (Guardian Corps trooper, sentry, technician, minder plainclothes, shocktrooper, K-9 handler + a custom dog rig, investigator). Shared skeleton for shared animation.
- **LODs & Nanite-for-characters** evaluated per platform budget.

### 2.3 Deliverable
Each mission moves through: **greybox (exists/derived) → art blockout → first-pass dress → lighting → optimization → final.** Art never changes a sightline without design sign-off.

---

## 3. WORKSTREAM B — ANIMATION

- **Retarget to UE5 Mannequin / MetaHuman skeleton.** Adopt the UE5 Manny/Quinn (or MetaHuman) skeleton as the standard rig; use **IK Retargeter** to bring in animation sets. The prototype's non-skeletal blockout is replaced wholesale here.
- **Locomotion.** A proper **stealth locomotion set**: walk/crouch-walk/sprint, lean, cover, mantle/grapple, prone-optional; blendspaces + **Motion Matching** (UE5.4+) for grounded, weighty movement that suits restraint tone. Distance-matched starts/stops so guards and Yas don't foot-slide (the prototype's `AddMovementInput` locomotion becomes animation-driven).
- **Combat/takedowns.** Curated **non-lethal takedown** set (chokehold, dart-hit reactions, subdue-from-cover) plus lethal variants; clinical, un-lingering per tone. Boss-specific sets for Azadi (pistol volley telegraph, charge) and Vaziri (searching, stand-down beat).
- **Facial & dialogue.** MetaHuman Animator / performance capture for the story scenes (the Council eavesdrop, the throne-room exchange, the Hossein reunion). Runtime lip-sync for both Farsi and English tracks.
- **AI anim integration.** Guard states (Patrol/Suspicious/Chasing) drive anim state machines; investigate/search/callout/discover-body animations added for the expanded roster and Alert-Tier behaviors.
- **Sourcing.** Marketplace animation packs + Mixamo as *temp/second-tier*; hero and stealth-signature animation bespoke or mocap. A modest mocap shoot (or vendor) for the ~2 hours of story scenes and the signature takedown set.

---

## 4. WORKSTREAM C — AI: BEHAVIOR-TREE MIGRATION

- **From hand-rolled state machine to BT + navmesh.** The prototype's `AGuardAIController` C++ Patrol/Suspicious/Chasing machine with straight-line movement was a deliberate no-navmesh constraint. For the full game:
  - Add **NavMesh** to every level; replace `AddMovementInput` locomotion with `MoveTo`/`AIMoveTo` so guards path around real geometry.
  - Re-express the state machine as a **Behavior Tree + Blackboard** (Patrol / Investigate / Search / Chase / Combat / Callout / DiscoverBody / Stand-down), keeping the *tuning values* proven in the prototype (perception radius 2200 / FOV 70° / lose 2600 / hearing 1500; suspicion accumulation scaled by `StealthComponent` visibility + distance).
  - Keep `UAIPerceptionComponent` (Sight + Hearing configured in C++); the perception model already works — reuse it.
- **New behaviors for the roster (GDD §5).** EQS (Environment Query System) for cover selection, flanking, and *methodical search* (Vaziri's Investigators re-checking hiding spots); group coordination for Corridor Pairs; scent/noise tracking for K-9; the social **suspicion read** for Minders (a separate BT service, not a vision cone).
- **Facility Alert Tier.** A game-mode/subsystem-level director (extend `UMissionSubsystem` or a new `UAlertDirectorSubsystem`) that scales patrol density, spawns sweeps, and shortens detection-decay grace — the systemic backbone of "the world reacts and remembers."
- **Boss AI.** Azadi and Vaziri as BT-driven bosses subclassing `ABossCharacter`; preserve the phase thresholds (100/66/33%) and `NotifyBossDefeated()` contract.
- **Designer authoring.** Patrol routes, EQS, and BT tuning exposed to designers via Blueprint/DataAssets over the C++ base.

---

## 5. WORKSTREAM D — LEVELS & STREAMING (12 MISSIONS)

- **World Partition + Level Instances / streaming.** Retire `APalaceLevelBuilder` (single code-built map) in favor of authored levels. Use **World Partition** for the larger missions (the bazaar, the night-of-Custodians city, the retreat) and **Level Streaming / Data Layers** for interior/exterior transitions and mission phases.
- **One level per mission (12), sub-streamed by phase** so a 45–55 min mission loads only what's in play; act-shared assets (palace in M1 & M12) share a source level with data-layer variants (garden reception vs. curfew checkpoints).
- **Mission data.** Objective chains move from the hard-coded array in `UMissionSubsystem` into **per-mission DataAssets** (objective id, title, text, type, links) so design iterates without code. `AInteractableActor` types (Keycard/Intel/EavesdropZone/Door/Marker) extend with new types (Photograph, DeadDrop, Disguise, Persuade, Rig, Escort-waypoint).
- **Save system.** Add checkpointing + mid-mission saves (difficulty-gated per GDD §8) — absent in the prototype, required for 10 hours.
- **Performance.** Continuous profiling to hold 1080p/60 on PS5 lead; streaming budgets, Nanite/Lumen cost passes, and a 4K/30 quality mode.
- **Set-pieces.** Author the systemic squeezes (searchlight sweeps, live-broadcast window, rope-line crossing, salt gauntlet, quorum race) as designer-scripted encounters over the shared systems — no bespoke one-off code where a system will do.

---

## 6. WORKSTREAM E — UI/UX

- **Replace `AVeilHUD` canvas drawing with UMG.** The prototype's `DrawText`/`DrawRect` HUD proves the layout (mission/objective top-left, health/darts bottom-left, detection top-center, boss bar, eavesdrop bar, crosshair, fail/victory cards). Rebuild in **UMG** with real art, animation, and full localization/accessibility (scalable text, colourblind meters, caption system).
- **Menus & meta.** Main menu, mission select, skill-tree UI (3 branches, GDD §6.2), evidence-dossier inventory, difficulty & accessibility settings (the large slider set from GDD §8/§12), pause/save.
- **DualSense feedback UI.** Surface adaptive-trigger/haptic states where useful (tool status, detection heartbeat) with accessible visual/audio redundancy.

---

## 7. WORKSTREAM F — AUDIO

- **Original score.** Commission the santur + restrained electronic score (GDD §10) with **adaptive stems keyed to the Alert Tier**. Implement via **MetaSounds / Wwise** (recommend Wwise for the adaptive complexity and localization mixing).
- **3D audio / Tempest** for PS5; spatialized guard callouts and "direction of attention" as a core stealth sense, with captioned/visual redundancy.
- **VO.** Full cast recording in **English and Farsi** (see §8). Diegetic masking beds (fountains, call to prayer, bazaar, wind, mast hum) per level identity.
- **SFX.** Tranq/EMP/grapple/probe tool audio; footstep-by-surface (carpet/marble/snow/salt) tied to haptics; UI and stinger design.

---

## 8. WORKSTREAM G — LOCALIZATION (FARSI / ENGLISH)

- **Dual-language from day one, not bolted on.** English is the pivot script (see MISSION_SCRIPTS.md); **Farsi is authored by native writers**, not literal-translated — the father/daughter register and clerical honorifics carry the drama and must feel native.
- **Full VO in both languages** for all story scenes and earpiece/ambient lines; lip-sync authored for both.
- **Engine setup.** UE **String Tables + localization dashboard**; **RTL text support** and a Farsi-capable font pipeline for all UMG; culture-correct dates/numbers; text-expansion-safe layouts.
- **Cultural authenticity & sensitivity pass.** A consultant/writer review to keep the fictional republic respectful and grounded, and to hold the binding restraint rule: **no real persons, states, or organizations — only the Council of Custodians, Guardian Corps, and The Qanat.** This review gates every script and asset.
- **Beyond launch (optional).** Text-only additional languages (Arabic, Turkish, French, etc.) architected-for but out of core scope.

---

## 9. WORKSTREAM H — QA, PERFORMANCE, CERTIFICATION

- Continuous automation for the objective/mission-flow systems (inheriting the prototype's clean subsystem seams).
- Stealth-balance passes per difficulty (GDD §8) with telemetry on detection/restraint.
- PS5 **TRC certification**, DualSense feature compliance, accessibility validation (GDD §12).
- Perf hold: 1080p/60 lead, 4K/30 quality mode; streaming/memory budgets.

---

## 10. TEAM & TIMELINE (honest estimate)

**Team size: 15–30 people.** A focused mid-size production. Indicative composition at full ramp (~25):
- Design: 1 lead + 3–4 level/system/mission designers.
- Programming: 1 lead + 3–4 (gameplay/AI, tools/data, UI, systems).
- Art: 1 art director + 4–6 (environment, character, technical art/lighting).
- Animation: 2–3 (+ mocap vendor).
- Audio: 1 in-house + composer/VO vendors.
- Narrative: 1 lead writer + Farsi co-writer + cultural consultant.
- Production/QA/Loc-management: 3–4.
Scale toward 30 during peak content production (Acts II–III), down at start/end.

**Timeline: 2–3 years** from the current prototype to ship. Realistic phasing:

| Phase | Duration | Milestone |
|-------|----------|-----------|
| **Pre-production / systems hardening** | ~4–6 mo | Data-drive missions; navmesh + BT migration; UMG shell; art bible & vertical-slice target; pipeline & loc setup. |
| **First Playable (Act I, 4 missions)** | ~6–8 mo | Missions 1–4 at first-pass art/anim/audio; the prototype content re-shipped in the new pipeline; core loop proven at fidelity. |
| **Production (Acts II–III, 8 missions)** | ~10–14 mo | All 12 missions content-complete; full roster/AI; both-language VO recorded; set-pieces in. |
| **Alpha → Beta → Cert** | ~6–8 mo | Feature/content lock; balance, perf, localization polish; TRC cert; accessibility validation; day-one. |

**Honest risk notes.**
- The **AI migration** (state machine → BT + navmesh + EQS search behaviors) and **12-mission content volume** are the two biggest cost/schedule risks; the roster's methodical-search and social-stealth enemies are non-trivial R&D.
- **Full dual-language VO** roughly doubles audio/loc cost — budget it explicitly, don't discover it in beta.
- **Scope discipline** is the whole game: 12 handcrafted 45–55 min missions is ambitious for 15–30 people. If the team lands nearer 15, expect the upper end (3 years) or a scope trim (e.g., merge two Act II missions). Protect the pillars, cut breadth before depth.
- **Cultural authenticity + restraint rule** is a standing gate, not a phase — resourced across the whole timeline.

**One-line summary.** The prototype proves the game *works*; a 15–30 person team over 2–3 years is what turns proof into a shippable, dual-language, PS5-first stealth thriller — provided the mission systems are data-driven early and scope stays honest.

---

## 11. ASSET BREAKDOWN (indicative counts)

Rough content volume implied by GDD — for scheduling, not a bill of materials.

| Category | Count | Notes |
|----------|-------|-------|
| Mission levels (.umap, art-final) | 12 | 45–55 min each; palace shared M1/M12 via data layers |
| Environment kits (art-directed) | ~8 | palace, bazaar, ministry, seminary, TV station, safehouse district, salt prison, mountain retreat |
| Hero props (bespoke) | ~30 | Peacock throne dais, dossier case, santur set-dressing, set-piece props |
| Named-character models (MetaHuman) | ~7 | Yas, Azadi, Hossein, Darya, Vaziri, Bijan, Banu |
| Enemy archetype models | 9 | modular kits + variants (incl. dog rig) |
| Gadgets (models + FX) | 5 | tranq, EMP, probe, grapple, disguise set |
| Animation sets | ~6 major | stealth locomotion, cover, takedowns (lethal/non), boss ×4, facial/dialogue |
| Story cinematics | ~24 | ~2 in/out per mission + act beats; mocap for hero scenes |
| VO lines (per language) | thousands | scripted + reactive banks (see MISSION_SCRIPTS.md), ×2 languages |
| Music stems | adaptive sets | per Alert-Tier, per mission mood; santur + electronic |
| UMG screens | ~15 | HUD, menus, skill tree, dossier, settings/accessibility |

**Reuse strategy.** Act-shared spaces (palace), modular architecture kits, shared enemy skeleton/animation, and data-driven objectives keep 12 missions tractable for a 15–30 person team. Bespoke effort concentrates on hero props, named-cast faces, story scenes, and set-pieces.

---

## 12. TOOLING & PIPELINE

- **Source control:** Perforce (binary assets) + typed asset naming; C++ via the existing module.
- **Data-driven missions:** DataAsset/DataTable authoring for objective chains, interactable types, patrol routes, dialogue triggers — designers iterate without programmer round-trips. This is the highest-leverage tooling investment (retires `APalaceLevelBuilder` and the hard-coded chain).
- **AI authoring:** Behavior Tree / Blackboard / EQS assets subclassing the C++ controllers; in-editor patrol-route and perception-debug visualizers (extend prototype debug draws).
- **Localization dashboard + String Tables**, RTL/Farsi font pipeline, VO batch-import tooling.
- **Audio:** Wwise integration (recommended) for adaptive stems + dual-language mixing; MetaSound fallback for simpler cases.
- **Automated tests** on the mission/objective subsystems (clean seams inherited from the prototype's subsystem design).
- **Perf HUD & telemetry:** continuous 1080p/60 profiling; stealth/restraint telemetry to tune detection and difficulty.

---

## 13. RISK REGISTER

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| AI migration (state machine → BT + navmesh + EQS search/social behaviors) overruns | High | High | Prototype early in pre-pro; reuse proven perception tuning; phase behaviors (basic → search → social) |
| 12-mission content volume vs. 15–30 team | High | High | Data-driven missions, kit reuse, shared palace; ready-to-trim (merge two Act II missions) |
| Full dual-language VO cost/schedule | High | Med | Budget from day one; lock scripts early; native Farsi writer, not translation |
| Cultural authenticity / restraint-rule slip (naming a real entity) | Med | High | Standing consultant gate on every script/asset; the rule is binding, reviewed continuously |
| 1080p/60 with Nanite/Lumen on PS5 | Med | Med | Continuous profiling; baked-lighting perf mode; 4K/30 quality mode as pressure valve |
| Social-stealth / legitimacy systems are novel R&D | Med | Med | Vertical-slice the ministry mission (M6) early to de-risk the meter and Minder AI |
| Scope creep on set-pieces (bespoke one-offs) | Med | Med | Build set-pieces from shared systems; no bespoke code where a system serves |
| Save/checkpoint system (absent in prototype) underestimated | Low | Med | Address in pre-pro systems-hardening phase, not late |

---

## 14. PHASE EXIT CRITERIA (definition of done per phase)

- **Pre-production done when:** missions are data-driven; navmesh + BT guard works in a test level at parity with the prototype's behavior; UMG HUD replaces canvas HUD; the vertical-slice target (one Act-II social-stealth mission, e.g. M6) is greenlit; loc + audio pipelines proven end-to-end in both languages.
- **First Playable done when:** Missions 1–4 play start-to-finish at first-pass fidelity, prototype content re-shipped in the new pipeline, core loop and DualSense features validated on PS5 dev kit.
- **Production done when:** all 12 missions content-complete, full roster/AI, all set-pieces in, both-language VO recorded, skill tree + evidence dossier + accessibility settings functional.
- **Ship-ready when:** feature/content locked, balanced across difficulties, TRC cert passed, accessibility validated, both languages fully mixed and lip-synced, 1080p/60 held.

---

## 15. SUMMARY

The delivered C++ prototype is an honest vertical slice of *systems* — infiltrate, eavesdrop, steal, boss, extract — with the detection model, non-lethal balance, objective flow, and DualSense-first controls already proven. The path to the full game is: **data-drive the missions and migrate the AI early, dress the proven greybox with real assets and animation, author 12 streamed missions and a second language, and hold scope with a 15–30 person team over 2–3 years.** Protect the four pillars (the name opens doors; restraint over spectacle; a daughter's stakes; a victory that fixes nothing); cut breadth before depth; keep the republic — and everyone in it — fictional.

---

## 16. STAFFING RAMP (indicative, by phase)

Headcount flexes with the phase; peak lands in Production (Acts II–III content).

| Discipline | Pre-pro | First Playable | Production (peak) | Alpha→Cert |
|-----------|---------|----------------|-------------------|------------|
| Design | 3 | 4 | 5 | 3 |
| Programming | 4 | 5 | 5 | 3 |
| Environment art | 2 | 3 | 5 | 2 |
| Character/tech art | 1 | 2 | 3 | 1 |
| Animation | 1 | 2 | 3 | 1 |
| Audio (in-house) | 1 | 1 | 1 | 1 |
| Narrative + Farsi + consultant | 2 | 2 | 2 | 1 |
| Production/QA/Loc-mgmt | 2 | 3 | 4 | 4 |
| **Approx. total** | **~16** | **~22** | **~28** | **~16** |

Vendors (mocap, VO studios ×2 languages, composer, additional QA/loc) sit outside core headcount and spike during Production and Alpha.

## 17. BUDGET CONSIDERATIONS (directional, not a quote)

- Cost is dominated by **people-time over 2–3 years** (15–30 staff) — the single biggest line by far.
- **Dual-language VO + mocap** are the largest external spends; budget both from day one (dual-language roughly doubles the VO/loc line).
- **Middleware/licensing:** UE5 royalty terms, Wwise, Perforce hosting, PS5 dev kits — modest against headcount but real.
- **Marketplace/Quixel** asset spend is small and front-loaded; the savings are in *time*, not licenses — Megascans are free in-engine, kits are cheap relative to bespoke.
- **Contingency:** hold 15–20% schedule/budget contingency against the AI-migration and content-volume risks (§13). A 15-person team should assume the upper (3-year) timeline or a planned scope trim.

## 18. POST-LAUNCH (out of core scope, architected-for)

- **Text-only additional languages** (Arabic, Turkish, French) — the loc pipeline supports them; VO stays EN/FA.
- **Accessibility patches** driven by community feedback (the slider architecture makes these cheap).
- **New Game+ / Veil mode / mission-select scoring** ship at launch (GDD §19) — no live-service tail planned; this is a complete single-player narrative game, not a platform.
- **The restraint rule outlives launch:** any post-launch content, marketing, or community material stays within the fiction — Council of Custodians, Guardian Corps, The Qanat — and never references a real person, state, or organization.

---

## 19. CRITICAL-PATH DEPENDENCIES

Order matters — some work gates everything downstream. The dependency spine:

1. **Data-driven mission system** (retire hard-coded chain + `APalaceLevelBuilder`) → gates all level authoring. **Do first.**
2. **Navmesh + Behavior-Tree guard at prototype parity** → gates the whole enemy roster and every mission's encounter design. **Do first, in parallel.**
3. **UE5-mannequin/MetaHuman skeleton + retarget pipeline** → gates all animation and character art. **Pre-pro.**
4. **UMG HUD + localization/RTL pipeline** → gates all UI and both-language text. **Pre-pro.**
5. **Vertical-slice mission (recommend M6 Ministry Archives)** → de-risks the novel legitimacy/social-stealth systems before they're spread across Act II. **First Playable.**
6. **Wwise + adaptive-audio + dual-language VO pipeline** → gates final audio; script-lock feeds it. **Pre-pro setup, Production execution.**

Everything else (art dressing, additional missions, set-pieces, polish) is parallelizable once 1–4 are stable. The two hard gates are **data-driven missions** and **AI migration**; slip either and the whole content phase slips.

## 20. FIRST 90 DAYS (concrete kickoff)

A pragmatic pre-production sprint plan from the delivered prototype:

- **Weeks 1–2.** Stand up Perforce, asset-naming, CI, PS5 dev kits. Import prototype; confirm it builds and plays on kit at 1080p/60. Author art bible from the prototype palette and STORY_BIBLE canon.
- **Weeks 3–6.** Refactor `UMissionSubsystem` to load per-mission DataAssets; prove it by re-driving the existing M0–M2 chain from data with no behavior change. Add navmesh to a test level; port one guard from the C++ state machine to BT + `MoveTo` at perception parity.
- **Weeks 7–10.** Retarget UE5 mannequin; get Yas + one guard animating with a first-pass stealth locomotion set replacing the blockout capsules. Stand up UMG HUD replacing the canvas HUD. Set up String Tables + Farsi RTL font in a test screen.
- **Weeks 11–13.** Grey-box the M6 vertical slice; prototype the Legitimacy meter + Minder suspicion-read AI. Record a scratch dual-language VO pass of the M0 cold open through the loc pipeline end-to-end. Review: greenlight the vertical-slice target and lock the First-Playable plan (Missions 1–4).

Exit this 90 days with the two hard gates (data-driven missions, AI migration) *proven in a test level*, the presentation pipeline (skeleton, UMG, loc) *stood up*, and the riskiest new system (social stealth) *prototyped* — i.e. the schedule's biggest unknowns retired before Production ramps.

---

## 21. PER-MISSION PRODUCTION TRACKER (template)

A living status grid to run during Production; stages: **G**reybox / **B**lockout-art / **D**ress / **L**ight / **A**udio / **V**O(EN+FA) / **P**olish / **C**ert. All start at "derived from prototype" only where the palace is shared.

| # | Mission | Phase target | Notes |
|---|---------|--------------|-------|
| 1 | Wall of Roses | First Playable | greybox derives from prototype garden |
| 2 | Custodians Convene | First Playable | greybox derives from prototype interior; boss re-ship |
| 3 | Long Bazaar | First Playable | crowd/social systems debut — schedule buffer |
| 4 | Ledger of Ashes | First Playable | EMP + Vaziri Duel 1; Act I climax |
| 5 | House of Safe Rooms | Production | reactive raid escalation R&D |
| 6 | Ministry Archives | Vertical Slice | **build first** to de-risk legitimacy system |
| 7 | Seminary | Production | reverence/no-weapons systems + cultural review |
| 8 | Broadcast Standard | Production | live-window + Vaziri Duel 2; Act II climax |
| 9 | Salt Prison | Production | escort-in-two-bodies system |
| 10 | Cold Retreat | Production | snow tracks + the moral-hinge branch |
| 11 | Night of Custodians | Production | Quorum ledger + city streaming (World Partition) |
| 12 | Veil of Succession | Production→Cert | branch-aware finale; shares palace source with M1 |

Rule: a mission is not "content-complete" until its **VO exists in both languages** and its **restraint/detection telemetry** reads sane on Standard difficulty.
