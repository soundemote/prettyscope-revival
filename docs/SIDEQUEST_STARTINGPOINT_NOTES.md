# Sidequest Starting Point Notes

These notes come from a read-only inspection of:

- `C:\Users\argit\Desktop\baconpaulstartingpointTEMPLATE`

The inspected repository currently has local submodule/source changes and points
at `https://github.com/soundemote/asciiscope-clap.git`. Do not treat it as the
authoritative Prettyscope plugin workspace until Architect confirms which local
copy should be used.

## High-Level Shape

- The project is CLAP-first and uses `sst::clap_juce_shim::ClapJuceShim` to host
  a JUCE component editor.
- Parameters are owned by an `engine::Patch` model built from
  `sst::plugininfra::patch_support::PatchBase`.
- CLAP parameter callbacks are implemented directly in `src/clap/plugin-clap.cpp`.
- The editor talks to the audio/engine side with ring-buffer queues:
  `mainToAudio` and `audioToUi`.
- State save/load uses `patchToOutStream()` and `inStreamToPatch()`.

## Parameter Model

Sidequest parameters are `Param` objects with metadata:

- numeric `meta.id`
- display `meta.name`
- type and flags from `ParamMetaData`
- min/default/max values
- CLAP flags such as `CLAP_PARAM_IS_AUTOMATABLE`

The current template sorts `patch.params` for presentation, so Prettyscope should
not use construction order as identity. Prettyscope descriptor stable string IDs
should remain the human/state identity, and descriptor numeric IDs can map to
Sidequest/CLAP parameter IDs.

Recommended adapter direction:

- Build Sidequest `Param` metadata from Prettyscope visual descriptors.
- Use `VisualParameterStableId::value` as the CLAP-facing numeric ID where
  possible.
- Keep descriptor string IDs for Prettyscope state/presets/docs.
- Keep normalized conversion in Prettyscope descriptors, not in scattered plugin
  UI code.

## Normalized Values

Sidequest patch values appear to be stored as raw parameter values, not purely
normalized transport values. The CLAP wrapper delegates `paramsValue()`,
`paramsInfo()`, `paramsValueToText()`, and `paramsTextToValue()` to the patch
support helpers.

Prettyscope should preserve the core rule:

- host automation transport uses normalized values at the Prettyscope adapter
  boundary
- Prettyscope render state uses raw `VisualParams`
- descriptor helpers own raw/normalized conversion

The existing Prettyscope host roundtrip example is the correct core-side proof
before binding to Sidequest.

## Gestures And UI Changes

The JUCE UI data bindings push explicit messages:

- `BEGIN_EDIT`
- `SET_PARAM`
- `END_EDIT`

`Engine::processUIQueue()` turns those into CLAP param value and gesture events.

Prettyscope UI controls in the plugin should use that existing gesture lane for
host-visible parameters, but non-automatable/internal controls should stay out of
the host parameter list unless Architect chooses otherwise.

## Threading And Audio/UI Handoff

Sidequest already has two queues:

- `mainToAudio`: editor/main thread to engine/audio side
- `audioToUi`: engine/audio side to editor/main thread

The template currently sends UI updates for params, VU, voice count, patch name,
dirty state, rescans, and sample rate. This is the right place to add an
editor-safe oscilloscope signal handoff rather than making the renderer read
audio-thread memory directly.

Recommended signal direction for Prettyscope:

- capture or downsample incoming plugin audio in the engine/process side
- send a compact signal snapshot to the editor side
- render from `SignalSnapshot` or a dedicated Sidequest signal adapter
- keep `ExternalSignalSource` as prototype/reference glue, not a hard realtime
  contract

## Audio Ports

The inspected template currently declares zero audio input ports and one stereo
output port. Prettyscope as a DAW visualizer will need an audio input path.

Likely first plugin target:

- add one stereo audio input port
- keep or remove output depending on whether the plugin should be insert-style,
  instrument-style, or visualizer-only
- copy/observe host input samples for editor visualization

## JUCE/OpenGL Lifecycle

The current editor is a JUCE `WindowPanel` hosted through the CLAP/JUCE shim.
There is no existing OpenGL component in the inspected template.

Prettyscope plugin work will need a JUCE OpenGL component or equivalent child
component that:

- owns a `prettyscope::ScopeEngine`
- calls `initialize()` and `shutdown()` while the OpenGL context is current
- handles resize before render
- preserves visual params across OpenGL context rebuilds
- renders from an editor-safe signal snapshot

## State

Sidequest saves the patch model through plugininfra patch serialization.
Prettyscope should save visual parameters by descriptor identity, not UI order.

Likely options:

- adapt Prettyscope descriptors into Sidequest `Patch` params and let patch
  serialization carry them
- or keep a Prettyscope visual state block alongside the Sidequest patch state

The first option is probably simpler if all host-visible visual params become
Sidequest `Param`s. Internal/non-automatable controls may still need an explicit
visual state block.

## First Adapter Boundary Recommendation

Start with a thin plugin adapter layer:

- Sidequest `Patch` owns host-exposed parameter values.
- Adapter builds Sidequest params from Prettyscope descriptors.
- Editor copies Sidequest param values into `prettyscope::VisualParams`.
- Audio/process side sends signal snapshots to the editor side.
- Renderer depends on Prettyscope core only, not JUCE or Sidequest parameter
  objects.

This keeps Prettyscope descriptors host-ready but not host-owned.
