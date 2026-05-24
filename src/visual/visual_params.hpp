#pragma once

namespace prettyscope
{
enum class TraceMode
{
    Time,
    Xy,
};

enum class DecayStyle
{
    Classic,
    Phosphor,
};

struct RgbColor
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct VisualParams
{
    RgbColor backgroundColor = {0.018f, 0.012f, 0.026f};
    RgbColor traceColor = {1.0f, 0.22f, 0.70f};
    RgbColor glowColor = {0.18f, 0.80f, 1.0f};
    float traceGain = 0.10f;
    float glowStrength = 0.35f;
    float traceWidth = 2.0f;
    float glowWidth = 7.0f;
    float persistence = 0.98f;
    float fastDecay = 0.25f;
    float afterglow = 0.95f;
    bool persistenceEnabled = true;
    DecayStyle decayStyle = DecayStyle::Phosphor;
    TraceMode traceMode = TraceMode::Xy;
    int clearRevision = 0;
    bool showFps = true;
    float fps = 0.0f;
    bool showGrid = false;
    float gridIntensity = 0.22f;
};
}
