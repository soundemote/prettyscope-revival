#pragma once

namespace prettyscope
{
struct RgbColor
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct VisualParams
{
    RgbColor backgroundColor = {0.0f, 0.0f, 0.0f};
    RgbColor traceColor = {0.3f, 1.0f, 0.8f};
    RgbColor glowColor = {1.0f, 0.2f, 0.7f};
    float traceGain = 0.68f;
    float glowStrength = 0.35f;
};
}
