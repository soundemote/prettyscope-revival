# Read-Only Inspection Template

Use this template when an agent is asked to inspect a repo, codebase, file set, plugin shell, architecture, or project state without making changes.

Read-only means:

* no edits
* no commits
* no formatting changes
* no dependency updates
* no submodule updates
* no generated files unless explicitly requested
* no cleanup

## Purpose

A read-only inspection should answer:

* what exists
* where important code lives
* how the current architecture is shaped
* what seams are available
* what risks exist
* what the recommended first edit should be

The goal is orientation before action.

## Basic Shape

# Read-Only Inspection: `<topic>`

## Scope

Inspected:

* `repo/path`
* `file/or/folder`
* `file/or/folder`

Did not inspect:

* `optional item`
* `optional item`

No edits were made.

No commits were made.

## Repo State

Repo:

```
`<path>`
```

Branch:

```
`<branch>`
```

Remote:

```
`<remote>`
```

Status:

```
clean / dirty / ahead / behind / unknown
```

Submodule status, if relevant:

```
`<summary>`
```

## Files / Areas Inspected

* `path/to/file1`
* `path/to/file2`
* `path/to/folder`
* `build/config/file`

## Findings

* finding 1
* finding 2
* finding 3

## Architecture Summary

Current structure:

```
`<component A>`
    -> `<component B>`
    -> `<component C>`
```

Important classes / modules:

* `<name>` — `<role>`
* `<name>` — `<role>`
* `<name>` — `<role>`

## Relevant Seams

Potential places to attach future work:

* seam 1
* seam 2
* seam 3

Example:

* editor timer / repaint loop
* audio-to-UI queue
* parameter registry
* renderer component
* snapshot buffer
* plugin metadata
* build target

## Risks / Constraints

* risk 1
* risk 2
* risk 3

Examples:

* inherited architecture is still template-shaped
* audio thread cannot allocate
* submodules should not be updated
* UI path exists but not snapshot-safe yet
* current names are visible but class renames would cause broad churn
* descriptor identity must not be replaced by adapter-local IDs

## Recommendation

Recommended next task:

```
`<bounded first edit>`
```

Reason:

```
`<why this is the next smallest useful step>`
```

Do not do yet:

* non-goal 1
* non-goal 2
* non-goal 3

## Questions / Decisions Needed

Only include if needed.

* question 1
* question 2

If no questions are needed:

```
No architecture decision needed before the recommended next task.
```

## Stamp

End with:

```
[received: Sender→Receiver: previous]
[sent: Sender→Receiver: fresh]
```

# Copy/Paste Read-Only Inspection Skeleton

# Read-Only Inspection: `<topic>`

## Scope

Inspected:

* `<repo/path>`
* `<file/folder>`

No edits were made.

No commits were made.

## Repo State

Repo:

```
`<path>`
```

Branch:

```
`<branch>`
```

Remote:

```
`<remote>`
```

Status:

```
`<clean/dirty/ahead/behind>`
```

## Files / Areas Inspected

* `<file>`
* `<file>`
* `<folder>`

## Findings

* `<finding>`
* `<finding>`
* `<finding>`

## Architecture Summary

`<summary>`

## Relevant Seams

* `<seam>`
* `<seam>`

## Risks / Constraints

* `<risk>`
* `<risk>`

## Recommendation

Recommended next task:

```
`<task>`
```

Do not do yet:

* `<non-goal>`
* `<non-goal>`

[received: `<Sender→Receiver: previous>`]
[sent: `<Sender→Receiver: fresh>`]
