# Prettyscope

Prettyscope is a modern OpenGL oscilloscope and signal visualizer. The goal is a
beautiful, musical scope that can grow from a standalone visual demo into a
reusable visualization layer for generated signals, audio buffers, and eventual
`soemdsp-sandbox` runtime output.

This repository still contains PrettyScope Graveyard material at the root. New
work lives in `src/` and should stay small, direct, and runnable.

## Current Demo

- Native Win32 window
- OpenGL rendering through the vendored GLAD 3.2 loader
- Animated shader-drawn waveform trace
- Small `SignalBuffer`, `TestSignalGenerator`, and `VisualParams` abstraction

## Build

```powershell
cmake -S . -B build-ninja -G Ninja
cmake --build build-ninja
.\build-ninja\prettyscope.exe
```

Run those commands from a Visual Studio Developer PowerShell or Developer
Command Prompt so MSVC is on `PATH`.

Press `Esc` or close the window to quit.

## Salvage Policy

The old JUCE plugin project, presets, color maps, and installer files are
reference material only unless a piece is intentionally adapted. Any salvaged
code or assets should be documented in `docs/SALVAGE.md` with source and reason.
