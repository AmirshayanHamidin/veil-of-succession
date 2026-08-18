# AUTOMATION AGENDA — Veil of Succession
Single source of truth for AUTONOMOUS (scheduled, headless) runs.
Interactive 10-minute sessions use WORK_FRAMEWORK.md instead; this file covers
only work a cloud session can do WITHOUT the engine, the PC, or the user.

The copy of this file on the REMOTE (GitHub) is ground truth. Every run reads
it from the remote FIRST (cache-busted), works the top unchecked queue item,
and pushes the updated agenda in the same commit as its results.

===============================================================================
## HARD GUARDRAILS (never violate; encode failures as failures)
===============================================================================
- NEVER handle credentials, tokens, or paid services. If push access is
  missing, STOP after producing outputs and say so in the summary.
- Touch NOTHING outside this repository.
- NO asset downloads (MetaHuman, Fab, freesound) — those need the user.
- A cloud run CANNOT build, package, or see the game. Therefore it NEVER
  claims "works", "fixed", or "verified" about gameplay. Code output is always
  a DRAFT PATCH + test plan, queued for an interactive session to build and
  screenshot-verify (Rules 1–3 in WORK_FRAMEWORK.md).
- One queue item per run. If an item is too big, split it in the queue and do
  the first part. Never start a second item in the same run.
- If the same item fails twice across runs, mark it BLOCKED with the reason
  and move on. Do not retry forever.
- Report honestly: a run that produced nothing publishable logs exactly that.

===============================================================================
## RUN PROTOCOL (every scheduled run, in order)
===============================================================================
1. FETCH remote README + this agenda (cache-busted: append ?nocache=<epoch>).
   If the previous run's entry in RUN_LOG.md is < 45 min old and its item is
   still marked in-progress, STOP (avoid overlapping runs).
2. CLEAN-STATE CHECK (a crashed run must not deadlock every later run):
   a. Stale locks: if ANY *.lock file under .git/ (index.lock, HEAD.lock,
      objects/maintenance.lock, ref locks) is > 15 min old, the git process
      that made it is dead — delete it and continue. SANDBOX NOTE: `rm` in
      the mounted repo fails with "Operation not permitted" until deletion
      is enabled; call the `mcp__cowork__allow_cowork_file_delete` tool
      (load via ToolSearch) with the lock's VM path, then retry `rm`. If
      deletion still fails, STOP and report the exact error.
   b. Interrupted pre-registration: if the working tree is dirty and the
      ONLY change is a single queue item flipped [ ]->[~] in this file,
      AND that item's output paths do not exist, AND RUN_LOG.md has no
      completed entry for it — recover: `git checkout -- AUTOMATION_AGENDA.md`,
      note the recovery in this run's RUN_LOG line, and continue.
   c. Interrupted completion (a crashed run's FINISHED work — publish it,
      don't stop): if the dirty/staged changes consist ONLY of (i) new or
      modified files at the output paths named by the current [~] (or just-
      flipped [x]) queue item, (ii) that item's mark in this file, and
      (iii) a RUN_LOG.md entry for that item — this is automation output,
      NOT the user's work in progress. Spot-verify the outputs against
      canon, correct the RUN_LOG entry's publish claim if it is premature,
      then commit and push as a recovery. That recovery consumes this
      run's one item: STOP after the verified push and summarize.
   d. ANY other dirty state, or a failed pull: STOP and report. Never
      stash, reset, or overwrite changes this run did not make.
   e. Abandoned claim: if the newest COMMITTED [~] is >= 45 min old with no
      outputs at its named paths and no completed RUN_LOG entry, treat it
      as abandoned — it becomes this run's item (leave the [~] as is).
3. TAKE the top unchecked [ ] item below (or the re-claimed item from 2d).
   Pre-registration is ATOMIC: flip the mark to [~], commit, and push
   back-to-back BEFORE any other work. If that push fails, revert the mark
   and STOP with the reason. Never begin work on an unpushed [~].
4. WORK using the planner/executor split:
   - Judgment (this model): interpreting canon, tone choices, specs, review.
   - Plumbing (delegate to cheap subagents via the Agent tool, model=haiku or
     sonnet): formatting, tables, boilerplate from a tight spec, file assembly.
   - Verify all delegated output before committing (spot-read + schema check).
5. WRITE outputs to the paths named in the item. Drafts go under Docs/drafts/,
   code patches under Patches/ as unified diffs + a NEEDS-PC test plan.
6. UPDATE this agenda: [~] → [x] (or BLOCKED), append one line to RUN_LOG.md.
7. COMMIT + PUSH results and agenda together. VERIFY the push by re-fetching
   one changed file and comparing byte-for-byte. Unverified = not published.
8. SUMMARIZE in 3 sentences: what shipped, what's queued next, what needs the
   user. This becomes the push/email notification.

===============================================================================
## QUEUE — cloud-safe items, ordered. Top unchecked item first.
===============================================================================

### Story (Phase D — no engine needed; drafts for the user's voice pass)
[x] Q1  D1 prep: three tonal variants of the story bible's opening + core canon
        (literary-thriller / le Carré-cold / TLOU-intimate), each ~600 words,
        plus 10 voice questions for the user. → Docs/drafts/story_bible_v2/
        NOTE: the user owns D1. These are raw material, not the final.
[x] Q2  D2: the Council eavesdrop scene — full script, 2 alternates for the
        key revelation. → Docs/drafts/eavesdrop_scene.md
[x] Q3  D3: Darya's earpiece dialogue, all 3 missions, cue-by-cue with trigger
        points from MISSION_SCRIPTS.md. → Docs/drafts/darya_earpiece.md
[x] Q4  D4: Yasaman/Azadi confrontation before the boss — 2 drafts (one where
        she speaks first, one where he does). → Docs/drafts/confrontation.md
[x] Q5  D5: ending — "the succession is safe, for now" and what it costs her.
        3 endings: bitter / ambiguous / defiant. → Docs/drafts/endings.md
[ ] Q6  D6: father/daughter beats woven across missions 1-3, mapped to
        existing checkpoints. → Docs/drafts/father_daughter_beats.md

### Code drafts (NEEDS-PC: build + screenshot verification in a live session)
[ ] Q7  D7 spec + patch: subtitle/dialogue-line system in MissionSubsystem
        (data table of cues, on-screen text w/ timing). → Patches/subtitles.diff
[ ] Q8  E1 patch: USaveGame save/load (mission, weapons, health) + test plan.
        → Patches/savegame.diff
[ ] Q9  E2 patch: distraction throwable (coin — noise event at impact point,
        reuses guard hearing). → Patches/throwable.diff
[ ] Q10 F2 patch: cache HUD actor lookups (6 world scans/frame → cached refs).
        → Patches/hud_cache.diff
[ ] Q11 F1 plan (plan only, no patch): decompose the 923-line SpyCharacter
        into components; migration order that keeps every build green.
        → Docs/drafts/spycharacter_refactor_plan.md

### Audits (repeatable; safe filler when blocked)
[ ] Q12 Docs-vs-code audit: verify every claim in PROJECT_STATE.md §1 against
        Source/; log mismatches to Docs/AUDITS.md with file:line evidence.
[ ] Q13 GDD missions 4-6: expand each to beat-sheet + guard-layout spec so an
        interactive session can build them. → Docs/drafts/missions_4-6/

### When the queue is empty → AUDIT MODE
Re-audit drafts against STORY_BIBLE canon, tighten specs, consolidate. Do NOT
invent new out-of-scope items; consolidation beats accumulation.

===============================================================================
## NEEDS-USER (automation never does these)
===============================================================================
- D1 final: the story bible in YOUR voice (use Q1's variants as raw material)
- All downloads: MetaHuman, Fab/Quixel, sounds (A2, B1-B5, C1-C6)
- Taste calls: pick among drafted alternates; art direction
- Build + screenshot verification of every patch in Patches/

===============================================================================
## RUN_LOG (one line per run, newest first — see RUN_LOG.md once it exists)
===============================================================================
