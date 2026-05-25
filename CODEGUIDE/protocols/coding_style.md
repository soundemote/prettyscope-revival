# Coding Style

Soundemote C++ coding style guide.

This guide captures Architect’s preferred code shape: compact, readable, explicit, low-ceremony, and suitable for realtime/audio/DSP work.

The style favors clarity over generic C++ fashion.

## Core Shape

Use camelCase for functions.

Use opening braces on the next line.

Do not put blank lines between closely related functions.

Prefer bracket initialization where practical.

Example:

```cpp
inline double clamp01(double value) noexcept
{
    if (value < 0.0)
        return 0.0;
    if (value > 1.0)
        return 1.0;
    return value;
}
inline double lerp(double a, double b, double t) noexcept
{
    return a + (b - a) * t;
}
```

## Function Names

Use camelCase for functions and methods.

Good:

```cpp
setSampleRate()
processSample()
applyBinding()
resetState()
normalizedToValue()
```

Avoid:

```cpp
SetSampleRate()
process_sample()
ProcessSample()
set_sample_rate()
```

## Braces

Function braces go on their own lines.

Good:

```cpp
void reset() noexcept
{
    phase_ = 0.0;
}
```

Avoid:

```cpp
void reset() noexcept {
    phase_ = 0.0;
}
```

Control-flow braces may be omitted for short simple branches when readability is better.

Good:

```cpp
if (gain_ >= 0.0f)
    return 0.0f;
```

Also good when the block needs structure:

```cpp
if (gain_ <= 0.0f)
{
    gain_ = 0.0f;
    return 0.0f;
}
```

## Blank Lines

Do not put blank lines between closely related functions.

Good:

```cpp
inline void reset() noexcept
{
    phase_ = 0.0;
}
inline double next() noexcept
{
    const auto out = std::sin(phase_);
    phase_ += phaseDelta_;
    return out;
}
```

Use blank lines to separate different conceptual sections.

Good:

```cpp
struct Oscillator
{
    double sampleRate_ { 48000.0 };
    double frequency_ { 440.0 };
    double phase_ { 0.0 };
    double phaseDelta_ { 0.0 };

    inline void prepare(double sampleRate) noexcept
    {
        sampleRate_ = sampleRate;
        updatePhaseDelta();
    }
    inline void setFrequency(double frequency) noexcept
    {
        frequency_ = frequency;
        updatePhaseDelta();
    }

    inline void reset() noexcept
    {
        phase_ = 0.0;
    }
    inline double next() noexcept
    {
        const auto out = std::sin(phase_);
        phase_ += phaseDelta_;
        return out;
    }
};
```

## Initialization

Prefer bracket initialization where possible.

Good:

```cpp
double sampleRate_ { 48000.0 };
double frequency_ { 440.0 };
float gain_ { 1.0f };
bool enabled_ { true };
std::size_t parameterCount_ { 0 };
```

Good:

```cpp
Parameter parameter {
    "gain",
    1.0f,
    0.0f,
    1.0f
};
```

Avoid unnecessary old-style assignment initialization:

```cpp
double sampleRate_ = 48000.0;
float gain_ = 1.0f;
```

Assignment is fine for mutation after initialization:

```cpp
gain_ = newGain;
```

## Member Variables

Use camelCase or lowercase member variables with trailing underscore.

Preferred:

```cpp
// structs and classes
double sampleRate { 48000.0 };
double phase { 0.0 };
double phaseDelta { 0.0 };
float outputGain { 1.0f };
bool isEnabled { true };

// lower level dsp structs
double samplerate_ { 48000.0 };
double phase_ { 0.0 };
double phasedelta_ { 0.0 };
float outputgain_ { 1.0f };
bool is_enabled_ { true };
bool decibels_to_ampltide{ 0 };
```

Trailing underscore marks member state.

Good:

```cpp
struct Gain
{
    float gain_ { 1.0f };
    inline float processSample(float x) const noexcept
    {
        return x * gain_;
    }
};
```

Avoid:

```cpp
float mGain;
float m_gain;
float _gain;
float Gain;
```

## Lowercase DSP Member Names

For low-level DSP structs, prefer lowercase member names when they feel clearer and more direct.

Good:

```cpp
double phase_ { 0.0 };
double frequency_ { 440.0 };
double samplerate_ { 48000.0 };
double phasedelta_ { 0.0 };
```

CamelCase is still allowed when readability improves:

```cpp
double sampleRate_ { 48000.0 };
double phaseDelta_ { 0.0 };
```

Use the form that reads best in the local DSP context.

## Underscores Inside Names

Avoid internal underscores unless they preserve readability between distinct logical concepts.

Normally prefer one readable word or camelCase.

Good:

```cpp
double phase_ { 0.0 };
double gain_ { 1.0 };
double cutoff_ { 1000.0 };
double resonance_ { 0.5 };
double phaseDelta_ { 0.0 };
```

Acceptable when separating concepts helps:

```cpp
double lowlevel_gain_ { 1.0 };
double dsp_cutoff_ { 1000.0 };
double audio_rate_ { 48000.0 };
double control_rate_ { 1000.0 };
```

Avoid excessive segmentation:

```cpp
double this_is_a_really_over_split_variable_ { 0.0 };
```

## Local Variables

Use camelCase for local variables.

Good:

```cpp
const auto inputSample = input[i];
const auto shapedValue = applyCurve(value);
auto parameterValue = defaultValue;
```

Short DSP locals should be avoided but not banned

OK:

```cpp
const double x = input[i];
const double y = x * gain_;
return y;
```

## Constants

Use `k_` prefix or clear lowercase/camelCase constants depending on file style.

Good:

```cpp
inline constexpr double kPi { 3.14159265358979323846 };
inline constexpr double kTwoPi { 2.0 * k_pi };
inline constexpr double kInvTwoPow53 { 1.0 / 9007199254740992.0 };
```

For internal detail namespaces, compact constants are okay when readable:

```cpp
inline constexpr double kInv_2pow53 { 1.0 / 9007199254740992.0 };
```

## Structs and Classes

Use PascalCase for struct/class names.

Good:

```cpp
struct TinyGainDsp
{
    float* gain { nullptr };
    inline float processSample(float x) const noexcept
    {
        return gain ? x * (*gain) : x;
    }
};
```

Good:

```cpp
struct DspBindingApplySummary
{
    bool ok { true };
    std::size_t parametersApplied { 0 };
    std::size_t messageCount { 0 };
};
```

## Plain Struct Preference

For low-level DSP and runtime data, prefer plain structs.

Good:

```cpp
struct ParameterRange
{
    float minValue { 0.0f };
    float midValue { 0.5f };
    float maxValue { 1.0f };
};
```

Avoid unnecessary class ceremony when a plain struct is enough.

## Realtime DSP Style

For low-level DSP objects:

* no heap allocation in hot paths
* no exceptions in hot paths
* no virtual dispatch in hot paths
* no hidden ownership
* no UI/editor/plugin dependencies
* externally owned memory is allowed
* small methods
* clear reset/process/next methods

Good:

```cpp
struct TinyBiasDsp
{
    float* bias { nullptr };
    inline float processSample(float x) const noexcept
    {
        return x + (bias ? *bias : 0.0f);
    }
};
```

## Namespaces

Use namespaces to express project structure.

Good:

```cpp
namespace soemdsp::runtime
{
struct Circuit
{
};
} //namespace soemdsp::runtime
```

Use compact closing namespace comments.

Preferred:

```cpp
} //namespace soemdsp::runtime
```

Instead of:

```cpp
} // namespace soemdsp::runtime
```

## Comments

Use comments sparingly.

Prefer comments that label sections or explain intent. Do not put unecessary newlines when comments already split the sections.

Good:

```cpp
//parameters
float gain_ { 1.0f };
float bias_ { 0.0f };
//state
double phase_ { 0.0 };
double phaseDelta_ { 0.0 };
```

Avoid comments that repeat the code.

Bad:

```cpp
//set gain to new gain
gain_ = newGain;
```

## Section Comments

Small section comments are allowed and encouraged when they improve scanning.

Preferred compact style:

```cpp
//settings
double sampleRate_ { 48000.0 };
double frequency_ { 440.0 };
//state
double phase_ { 0.0 };
//functions
inline void reset() noexcept
{
    phase_ = 0.0;
}
```

## Include Style

Use minimal includes.

Prefer direct includes for what the file uses.

Good:

```cpp
#pragma once
#include <cstddef>
#include <vector>
#include <string>
```

Avoid large includes when a smaller one is enough.

## Header Style

Header-only helpers are acceptable when they are small, inline, and consistent with existing runtime patterns.

Good:

```cpp
#pragma once
#include <ostream>
//page comments block comments with lots of text and information
//go here.
namespace soemdsp::runtime
{
inline void printSummary(const Summary& summary, std::ostream& os)
{
    os << "ok: " << (summary.ok ? "true" : "false") << '\n';
}
} //namespace soemdsp::runtime
```

## Function Size

Prefer small functions with one job.

Good:

```
inline bool isValid() const noexcept
{
    return minValue <= midValue && midValue <= maxValue;
}
inline float normalizedMidpoint() const noexcept
{
    return (midValue - minValue) / (maxValue - minValue);
}
```

If a function grows too large, split it by responsibility.

## Return Style

Return early when it makes the logic clearer.

Good:

```
inline bool writeTextFile(const Report& report, const std::filesystem::path& path)
{
    std::ofstream stream { path };
    if (!stream.is_open())
        return false;

    printReport(report, stream);
    return true;
}
```

## Booleans

Use clear boolean names.

Good:

```cpp
bool isValid { true };
bool hasErrors { false };
bool shouldReset { false };
```

Avoid vague names:

```cpp
bool flag { false };
bool thing { false };
```

## Error / Report Messages

Use clear report messages.

Good:

```cpp
result.messages.push_back("missing Circuit parameter: node 2 param modulation");
```

Avoid cryptic messages:

```cpp
result.messages.push_back("bad param");
```

## Example: Preferred Small DSP Struct

```cpp
struct TinyAffineDsp
{
    float* gain { nullptr };
    float* bias { nullptr };
    inline float processSample(float x) const noexcept
    {
        const auto g = gain ? *gain : 1.0f;
        const auto b = bias ? *bias : 0.0f;
        return x * g + b;
    }
};
```

## Example: Preferred Runtime Helper

```cpp
struct DspBindingApplySummary
{
    bool ok { true };
    std::size_t parametersApplied { 0 };
    std::size_t messageCount { 0 };
};
inline DspBindingApplySummary makeDspBindingApplySummary(
    const DspBindingApplyResult& result)
{
    DspBindingApplySummary summary;
    summary.ok = result.ok;
    summary.parametersApplied = result.parametersApplied;
    summary.messageCount = result.messages.size();
    return summary;
}
```

## Example: Preferred Print Helper

```cpp
inline void printDspBindingApplySummary(
    const DspBindingApplySummary& summary,
    std::ostream& os = std::cout)
{
    os << "[DSP BINDING APPLY SUMMARY]\n";
    os << "ok: " << (summary.ok ? "true" : "false") << '\n';
    os << "parameters applied: " << summary.parametersApplied << '\n';
    os << "messages: " << summary.messageCount << '\n';
}
```

## Example: Preferred Writer Helper

```cpp
inline bool writeDspBindingApplySummaryTextFile(
    const DspBindingApplySummary& summary,
    const std::filesystem::path& path)
{
    std::ofstream stream { path };
    if (!stream.is_open())
        return false;

    printDspBindingApplySummary(summary, stream);
    return true;
}
```

## General Rule

The code should feel:

* compact
* readable
* direct
* musical
* low-ceremony
* realtime-aware
* architecture-clean

Do not chase generic C++ style guides when they fight the local codebase.

Prefer the style that makes Soundemote code feel clear, alive, and easy to extend.

Add this near the end, before **General Rule**. It matches the style you already have and keeps `auto` as “avoid unless it improves clarity,” not a ban.

## What To Avoid

Avoid code that hides what is really happening.

Soundemote style prefers directness over cleverness.

### Avoid Unnecessary Indirection

Do not add extra layers unless they clearly pay for themselves.

Avoid:

```cpp
struct IGainProcessor
{
    virtual ~IGainProcessor() = default;
    virtual float process(float x) = 0;
};

struct GainProcessor final : IGainProcessor
{
    float gain { 1.0f };
    float process(float x) override
    {
        return x * gain;
    }
};
```

Prefer:

```cpp
struct Gain
{
    float gain { 1.0f };
    inline float processSample(float x) const noexcept
    {
        return x * gain;
    }
};
```

Indirection is allowed when it solves a real architecture problem. It should not appear just because C++ makes it possible.

### Avoid Unnecessary `auto`

Prefer explicit types when they make the code easier to read.

Good:

```cpp
float gainValue { 1.0f };
std::size_t parameterCount { parameters.size() };
DspBindingApplySummary summary { makeDspBindingApplySummary(result) };
```

Avoid when the type matters:

```cpp
auto gainValue = 1.0f;
auto parameterCount = parameters.size();
auto summary = makeDspBindingApplySummary(result);
```

`auto` is allowed when it avoids worse code.

Good:

```cpp
const auto parameter = circuit.findParameter(nodeId, parameterId);
```

Good:

```cpp
const auto it = parameterMap.find(parameterId);
```

Good:

```cpp
for (const auto& binding : objectBinding.parameterBindings)
{
    applyBinding(binding);
}
```

Use `auto` when the explicit type is noisy, duplicated, iterator-heavy, or would make the code less readable.

Do not use `auto` to make simple values vague.

### Avoid Clever C++

Avoid code that requires the reader to solve a puzzle.

Avoid:

```cpp
return enabled_ ? (x *= gain_, x + bias_) : x;
```

Prefer:

```cpp
if (!enabled_)
    return x;

const float gained { x * gain_ };
return gained + bias_;
```

### Avoid Generic Architecture for Local Problems

Do not build a framework when a function or struct is enough.

Avoid:

```cpp
struct IBindingExecutor
{
    virtual BindingResult execute(const BindingContext& context) = 0;
};
```

Prefer a direct helper until the architecture proves it needs more:

```cpp
inline DspBindingApplyResult applyDspParameterBindings(
    const DspObjectBinding& binding,
    const Circuit& circuit)
{
    //...
}
```

### Avoid Hidden Ownership

Ownership should be obvious.

Avoid unclear raw ownership:

```cpp
float* buffer { new float[512] };
```

Prefer external ownership, stack ownership, or clear containers depending on the layer:

```cpp
float* externalBuffer { nullptr };
```

```cpp
std::array<float, 512> buffer {};
```

For low-level DSP structs, externally owned memory is allowed, but it must be clear that the DSP object does not own it.

### Avoid Hot-Path Allocation

Do not allocate in realtime DSP hot paths.

Avoid:

```cpp
inline float processSample(float x)
{
    std::vector<float> temp;
    temp.push_back(x);
    return temp[0] * gain_;
}
```

Prefer:

```cpp
inline float processSample(float x) const noexcept
{
    return x * gain_;
}
```

### Avoid Exceptions in Realtime DSP Code

Do not throw from hot-path DSP methods.

Avoid:

```cpp
inline float processSample(float x)
{
    if (!gain)
        throw std::runtime_error("missing gain");
    return x * (*gain);
}
```

Prefer safe fallback behavior:

```cpp
inline float processSample(float x) const noexcept
{
    return gain ? x * (*gain) : x;
}
```

### Avoid Over-Abstract Naming

Names should say what the thing is.

Avoid:

```
Manager
Handler
ProcessorThing
DataObject
Utility
```

Prefer:

```
DspObjectBinding
CircuitReport
ParameterSnapshot
TinyGainDsp
ControlGraphApplyResult
```

Generic names are only acceptable when the local context makes them obvious.

### Avoid Style That Fights the Codebase

Do not chase generic C++ style if it makes Soundemote code less direct.

Avoid forcing:

* heavy OOP patterns
* template-heavy abstractions
* factory layers
* manager classes
* unnecessary virtual interfaces
* excessive type erasure
* excessive `auto`
* excessive `using`
* clever metaprogramming
* hidden ownership
* broad rewrites for style purity

Prefer code that is readable, explicit, compact, and close to the DSP/runtime problem being solved.
