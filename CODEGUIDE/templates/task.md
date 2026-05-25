# Task Template

Use this template for bounded agent tasks.

A task should be small enough to complete, build/test, commit, and report without creating architectural drift.

## Basic Shape

# Task: `<task title>`

## Goal

Describe the desired proof, behavior, or milestone.

Keep this focused.

Example:

```
Prove that a Circuit parameter can sync into externally owned DSP memory used by a contract-style DSP object.
```

## Scope

This task includes:

* item 1
* item 2
* item 3

This task does not include:

* non-goal 1
* non-goal 2
* non-goal 3

## Files

Create:

* `path/to/new_file.hpp`
* `path/to/new_demo.cpp`

Update:

* `path/to/existing_file.cpp`
* `docs/STATUS.md`
* `docs/SOME_PLAN.md`

Do not touch:

* unrelated repo
* unrelated subsystem
* submodules
* generated files unless requested

## Behavior Requirements

The implementation should:

* requirement 1
* requirement 2
* requirement 3

The implementation should not:

* non-behavior 1
* non-behavior 2
* non-behavior 3

## Demo / Test Requirements

The demo should prove:

* expected output or behavior 1
* expected output or behavior 2
* expected output or behavior 3

If generated files are written:

* verify expected markers
* remove generated files before commit unless instructed otherwise

## Architecture Boundaries

Preserve these boundaries:

* boundary 1
* boundary 2
* boundary 3

Do not introduce:

* forbidden abstraction 1
* forbidden abstraction 2
* forbidden cross-project dependency
* premature cleanup or redesign

## Build

Run:

```
<build command>
```

Example:

```
cmake --build C:\Users\argit\Desktop\soemdsp\build --config Debug
```

## Run

Run:

```
<demo/test command>
```

Example:

```
build/examples/Debug/runtime_demo.exe
```

## Commit

Commit message:

```
<commit message>
```

Example:

```
add dsp binding apply summary
```

Do not push unless Architect or the task explicitly allows it.

## Report

Report back with:

* files changed
* behavior added/changed
* demo output summary
* generated file verification/removal if applicable
* docs updated
* build result
* run/test result
* commit hash
* repo status
* whether push is recommended

## Stamp

End with:

```
[received: Sender→Receiver: previous]
[sent: Sender→Receiver: fresh]
```

# Copy/Paste Task Skeleton

# Task: `<title>`

## Goal

`<one focused goal>`

## Scope

Create:

* `<file>`

Update:

* `<file>`

Do not:

* `<non-goal>`
* `<non-goal>`
* `<non-goal>`

## Behavior

Implement:

* `<requirement>`
* `<requirement>`
* `<requirement>`

Preserve:

* `<boundary>`
* `<boundary>`

## Build

Run:

```
`<build command>`
```

## Run

Run:

```
`<run command>`
```

## Commit

Commit:

```
`<commit message>`
```

Do not push unless Architect asks.

## Report

Report:

* files changed
* behavior
* output summary
* docs updated
* build/run result
* commit hash
* repo status
* whether push is recommended

[received: `<Sender→Receiver: previous>`]
[sent: `<Sender→Receiver: fresh>`]
