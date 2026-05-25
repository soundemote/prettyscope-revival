# Cross-Agent Handoff Template

Use this template when one agent needs to pass context, a request, or a boundary-sensitive update to another agent through Architect.

There is no live multi-agent chat.

Architect manually routes the handoff.

A good cross-agent handoff should be paste-ready, bounded, and clear about what the receiving agent should do.

## Purpose

A cross-agent handoff should answer:

* who is sending
* who is receiving
* why the handoff exists
* what context matters
* what action is requested
* what boundaries must be preserved
* whether the receiver should report back

## Basic Shape

# Cross-Agent Handoff: `<Sender>` → `<Receiver>`

## Reason

`<Why this handoff is being sent.>`

Examples:

* Console needs Tracer to know about a visual boundary.
* DIRECTOR needs Console to create public clips.
* Void created a browser sketch that may inform Prettyscope.
* Vision needs Codex to avoid a boundary collision.
* Architect wants two agents aligned before work continues.

## Context

Relevant context:

* context item 1
* context item 2
* context item 3

Keep this short.

Do not dump unrelated history.

## Request

Please do:

* requested action 1
* requested action 2
* requested action 3

Do not do:

* non-goal 1
* non-goal 2
* non-goal 3

## Boundaries

Preserve these boundaries:

* boundary 1
* boundary 2
* boundary 3

Examples:

* Do not touch another agent’s repo.
* Do not introduce a shared abstraction yet.
* Do not assume direct dependency between projects.
* Do not change runtime architecture.
* Do not rush public claims ahead of proof.
* Do not turn a sketch into core architecture prematurely.

## Expected Report

Report back with:

* what was inspected or changed
* conclusion or result
* blockers
* next recommendation
* commit hash if committed
* repo status if relevant

If no report is needed:

```
No report needed unless blocked or boundary changes.
```

## Stamp

End with:

```
[received: Sender→Receiver: previous]
[sent: Sender→Receiver: fresh]
```

# Copy/Paste Handoff Skeleton

# Cross-Agent Handoff: `<Sender>` → `<Receiver>`

## Reason

`<reason>`

## Context

* `<context>`
* `<context>`

## Request

Please:

* `<action>`
* `<action>`

Do not:

* `<non-goal>`
* `<non-goal>`

## Boundaries

* `<boundary>`
* `<boundary>`

## Expected Report

* `<report item>`
* `<report item>`

[received: `<Sender→Receiver: previous>`]
[sent: `<Sender→Receiver: fresh>`]
