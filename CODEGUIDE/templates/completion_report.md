# Completion Report Template

Use this template when an agent finishes a bounded task, milestone, inspection, or committed change.

A completion report should make it easy for Architect and Vision to understand:

* what changed
* what did not change
* whether the work built/ran
* whether it was committed
* whether it should be pushed
* what stamp thread it belongs to

## Basic Shape

# Completed: `<task title>`

## Files Changed

* `path/to/file1`
* `path/to/file2`
* `path/to/file3`

## What Changed

* change 1
* change 2
* change 3

## What Was Intentionally Not Changed

* non-change 1
* non-change 2
* non-change 3

Use this section when boundary preservation matters.

Examples:

* No scheduler added.
* No plugin code added.
* No shared abstraction introduced.
* No submodules changed.
* No inherited architecture removed.
* No cross-repo dependencies added.

## Behavior / Demo Summary

Describe what the task now proves or enables.

Include exact output when the task includes a demo.

Example:

```
gain memory before: 0
apply result: ok: true, parameters applied: 1
gain memory after: 2
processSample(0.25): 0.5
```

## Generated Files

If generated files were written:

* list generated files
* list verification markers
* say whether they were removed before commit

Example:

Generated:

* `runtime_demo.report.txt`

Verified:

* `[DSP BINDING REPORT]`
* `ok: true`

Removed before commit:

* yes

If no generated files were written:

```
No generated files were written.
```

## Docs Updated

* `docs/STATUS.md`
* `docs/PLAN.md`

Or:

```
No docs were updated.
```

## Build Result

Command:

```
`<build command>`
```

Result:

```
passed / failed
```

Include relevant failure details if failed.

## Run / Test Result

Command:

```
`<run or test command>`
```

Result:

```
passed / failed
```

Include output summary if useful.

## Commit

Commit:

```
`<hash> <message>`
```

Or:

```
Not committed.
```

## Repo Status

* working tree clean / dirty
* branch status
* ahead/behind origin
* push performed / not performed
* push recommended / not recommended

Example:

```
Working tree clean.
main is ahead of origin/main by 1 commit.
Push recommended as a clean checkpoint.
No push performed.
```

## Risks / Notes

Optional.

Use only if something needs attention.

Examples:

* This required a small validation change.
* This revealed a naming inconsistency.
* This confirms existing behavior was already all-or-nothing.
* This leaves the next likely task as `<task>`.

## Stamp

End with:

```
[received: Sender→Receiver: previous]
[sent: Sender→Receiver: fresh]
```

# Copy/Paste Completion Report Skeleton

# Completed: `<task title>`

## Files Changed

* `<file>`
* `<file>`

## What Changed

* `<change>`
* `<change>`

## What Was Intentionally Not Changed

* `<non-change>`
* `<non-change>`

## Behavior / Demo Summary

`<summary>`

Exact output summary:

```
`<line>`
`<line>`
`<line>`
```

## Generated Files

`<generated file status>`

## Docs Updated

* `<doc>`

## Build Result

Command:

```
`<build command>`
```

Result:

```
`<passed/failed>`
```

## Run / Test Result

Command:

```
`<run/test command>`
```

Result:

```
`<passed/failed>`
```

## Commit

`<hash> <message>`

## Repo Status

`<repo status>`

## Push Recommendation

`<push recommended or not>`

[received: `<Sender→Receiver: previous>`]
[sent: `<Sender→Receiver: fresh>`]
