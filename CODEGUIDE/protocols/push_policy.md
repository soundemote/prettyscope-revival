# Push Policy

Shared push policy for Soundemote agents.

Push bandwidth is limited.

Architect / DIRECTOR will push every little while as needed.

Agents should focus on:

* doing bounded work
* building/testing
* committing clean checkpoints
* reporting repo status

Agents should not make push recommendations by default.

## Core Rule

Do not mention pushes in normal agent reports unless Architect explicitly asks.

Do not include:

```
Push recommended.
```

Do not include:

```
No push performed.
```

Instead, report repo status only.

Example:

```
Repo status:
- working tree clean
- main is ahead of origin/main by 2 commits
- main is behind origin/main by 0 commits
```

That gives Architect / DIRECTOR enough information to decide when to push.

## Agent Rule

Agents should not run `git push` unless Architect explicitly tells them to or the active task explicitly includes a push.

Agents should not ask whether to push after every commit.

Agents should not treat pushing as part of the normal completion-report loop.

## When Pushes May Be Mentioned

Mention push only when:

* Architect asks whether to push
* Architect explicitly requests push instructions
* a task explicitly says to push
* a remote checkpoint is required before risky work
* another agent cannot continue without remote state

Otherwise, do not mention pushes.

## Default Agent Workflow

Agent workflow:

```
edit
build/test
commit
report repo status
```

Architect / DIRECTOR workflow:

```
push every little while when bandwidth allows
```

## Repo Status Reporting

Agents should report:

* working tree clean / dirty
* ahead of origin by N commits
* behind origin by N commits
* conflicts if any

Example:

```
Repo status:
- working tree clean
- main is ahead of origin/main by 1 commit
- main is behind origin/main by 0 commits
- conflicts: none
```

## Behind-Origin Rule

If a repo is behind origin, agents should pause risky new coding work and report clearly.

Do not merge, rebase, pull, or push unless Architect explicitly instructs it.

Behind-origin state is not automatically bad, but it needs visibility before more work happens.

## Conflict Rule

If conflicts appear:

* stop
* do not guess
* do not resolve unrelated conflicts silently
* report conflict paths
* wait for Architect/Vision direction

## Minimal Rule

Commit cleanly.

Report ahead/behind status.

Do not talk about pushes unless asked.
