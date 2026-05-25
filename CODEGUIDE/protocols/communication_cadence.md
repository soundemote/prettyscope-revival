# Communication Cadence

Shared communication cadence rules for Soundemote agents.

This file defines how often agents should report to Architect, Vision, or other routed agents.

The goal is to reduce routing overload while keeping important architecture decisions visible.

## Core Rule

Agents should not report every tiny action.

Agents should report when communication changes the outcome.

Good reasons to report:

* a bounded task is complete
* a milestone is complete
* a commit is made
* an architectural boundary is about to be crossed
* an agent is blocked
* another agent’s context is needed
* a task has drifted from its original scope
* a decision affects another project
* repo status has become unsafe or confusing, such as being behind origin or having unresolved conflicts

Bad reasons to report:

* every small edit
* every routine build attempt
* every tiny UI tweak
* every small refactor inside approved scope
* every thought during implementation
* ordinary casual discussion

## Architect’s Role

Architect is the manual router.

Architect may send or withhold any agent message.

Agents should write reports and handoffs so Architect can paste them directly.

Agents should not assume any other agent has seen a message unless Architect relays it.

Architect / DIRECTOR may push repositories every little while when bandwidth allows.

Agents should not treat pushing as part of normal reporting unless Architect explicitly asks.

## Vision’s Role

Vision receives reports when architecture, sequencing, boundaries, or cross-agent coordination matter.

Vision should not become a bottleneck for every local implementation detail.

Vision should be used when:

* the next step needs architectural judgment
* an agent might cross ownership boundaries
* two agents might collide
* public/business strategy needs technical alignment
* a clean next task is needed
* stamps or task threads become confusing
* repo state is unsafe or confusing

Vision should not be used for routine local progress inside an already approved task.

## Codex Cadence

Codex should report to Vision at every completed bounded task / commit.

Reason:

`soemdsp` and `soemdsp-sandbox` are the big-ticket runtime spine. Small architectural moves can affect the future graph runtime, DSP binding, serialization, plugin path, sandbox editor, and export model.

Codex rhythm:

```
one proof
one demo
one commit
one report
next instruction
```

Codex should report when:

* a bounded task is complete
* a commit is made
* a build/test/demo fails in a task-relevant way
* implementation requires architecture changes
* the task risks scheduler/plugin/UI/runtime leakage
* docs or demos reveal an architectural inconsistency
* repo status becomes unsafe, behind origin, conflicted, or confusing

Codex should not report for:

* every file edit
* every compile fix
* every routine build attempt
* ordinary local implementation choices inside the approved task

## Console Cadence

Console does not need to report to Vision after every small edit.

Console works in milestone chunks.

Console reports when:

* a milestone is complete
* a commit is made
* Console is blocked
* Console is about to introduce a shared abstraction
* Console is about to connect to `soemdsp` / `soemdsp-sandbox`
* Console is about to change audio/plugin architecture
* Console is about to remove or rename inherited startingpoint structure
* Console needs another agent’s context
* repo status becomes unsafe, behind origin, conflicted, or confusing

Console should not report for:

* every local implementation step
* routine debugging
* routine build attempts
* small visual adjustments
* small JUCE layout changes
* internal cleanup already covered by the approved task

Console rhythm:

```
Vision gives a bounded milestone.
Console works independently inside that scope.
Console reports when complete, blocked, committed, or crossing a boundary.
```

## Tracer Cadence

Tracer does not need to report to Vision after every small edit.

Tracer works in milestone chunks.

Tracer reports when:

* a milestone is complete
* a commit is made
* Tracer is blocked
* Tracer is about to port the golden renderer
* Tracer is about to introduce renderer abstraction
* Tracer is about to remove or quarantine inherited synth/MIDI/voice architecture
* Tracer is about to expand descriptor coverage beyond approved scope
* Tracer is about to touch Console/Asciiscope territory
* Tracer needs another agent’s context
* repo status becomes unsafe, behind origin, conflicted, or confusing

Tracer should not report for:

* every local implementation step
* routine debugging
* routine build attempts
* small OpenGL tweaks
* small JUCE layout changes
* internal cleanup already covered by the approved task

Tracer rhythm:

```
Vision gives a bounded milestone.
Tracer works independently inside that scope.
Tracer reports when complete, blocked, committed, or crossing a boundary.
```

## Void Cadence

Void mostly works directly with Architect.

Void is documented for context, but Void is not part of the normal routed agent workflow.

Void does not need to report to Vision after every website edit or creative sketch.

Void reports to Vision only when Architect decides a Void discovery should be routed into the formal system.

Reasons to route Void through Vision may include:

* a website milestone affects broader Soundemote positioning
* a public demo becomes relevant to another agent
* a browser sketch becomes useful to Codex, Console, Tracer, or DIRECTOR
* a technical choice risks implying runtime architecture
* Void needs another agent’s context

Void should not report to Vision for:

* every visual tweak
* ordinary website layout changes
* routine browser prototype iteration
* casual creative exploration with Architect

Void rhythm:

```
Architect and Void sketch freely.
Void stays separate from normal agent routing.
Vision routes useful discoveries only when Architect chooses.
```

## DIRECTOR Cadence

DIRECTOR mostly speaks directly with Architect.

DIRECTOR should not route every business or content idea through Vision.

DIRECTOR reports to Vision when:

* business strategy affects technical sequencing
* a campaign needs Console, Tracer, Void, or Codex work
* a technical claim needs architecture accuracy
* public messaging risks misrepresenting a project
* a launch plan needs cross-agent sequencing
* DIRECTOR needs a handoff written for another agent
* boundaries might be crossed

DIRECTOR should not report to Vision for:

* every social caption
* every YouTube idea
* every ordinary business conversation
* every offer/pricing thought
* every small positioning decision

DIRECTOR rhythm:

```
DIRECTOR talks to Architect.
DIRECTOR contacts Vision for cross-agent alignment.
```

## Milestone vs Local Work

A milestone is a meaningful completed unit.

Examples:

* identity rename complete
* visual component added
* audio snapshot path added
* descriptor parameter adapter proven
* OpenGL view embedded
* DSP binding proof committed
* website landing section complete
* commission menu drafted

Local work is implementation detail inside a milestone.

Examples:

* adjusted padding
* fixed include path
* renamed local variable
* moved helper function
* tried a build
* fixed compiler warning
* changed one label

Agents should report milestones, not every local step.

## Boundary Crossings

Always report before crossing boundaries.

Boundary crossings include:

* Console touching Tracer work
* Tracer touching Console work
* Void defining core runtime architecture
* DIRECTOR assigning engineering work directly
* Codex introducing plugin or UI abstractions
* any agent updating submodules
* any agent removing inherited architecture
* any agent introducing a shared abstraction
* any agent changing another agent’s repo

## Push Policy Summary

Do not mention pushes in normal agent reports unless Architect explicitly asks.

Push bandwidth is limited.

Architect / DIRECTOR will push every little while as needed.

Agents should focus on:

* doing bounded work
* building/testing
* committing clean checkpoints
* reporting repo status

Instead of saying:

```
Push recommended.
```

Say only:

```
Repo status:
- working tree clean
- branch is ahead of origin/main by N commits
```

Mention push only when:

* Architect asks whether to push
* Architect explicitly requests push instructions
* a task explicitly says to push
* a remote checkpoint is required before risky work
* another agent cannot continue without remote state

Otherwise, do not mention pushes.

## Architect High-Load Mode

When Architect is carrying heavy routing load:

* reduce chatter
* avoid unnecessary questions
* give paste-ready text
* summarize current state
* preserve the next obvious action
* do not create extra routing work
* keep stamps clean

High routing load is acceptable when packets have identity, ownership, and thread continuity.

## Minimal Rule

If unsure whether to report, ask:

```
Does this message protect architecture, sequence, boundaries, money, repo safety, or momentum?
```

If yes, report.

If no, keep working inside the approved scope.
