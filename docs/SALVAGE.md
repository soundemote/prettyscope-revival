# Salvage Notes

Prettyscope is being rebuilt from scratch. The existing root-level JUCE project,
installer files, presets, color maps, and `Source/` directory are treated as
PrettyScope Graveyard reference material.

## Initial Salvage

- `Shared/glad/3.2`: reused directly as the OpenGL 3.2 function loader. This is
  a small, well-scoped dependency that lets the first renderer run without
  reviving the old JUCE/plugin architecture.
- `LICENSE`: the repository is MIT licensed. Keep this license with any reused
  material.

No large old rendering or plugin code has been copied into the new `src/`
application path.
