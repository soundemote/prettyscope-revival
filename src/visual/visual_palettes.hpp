#pragma once

#include "visual/visual_params.hpp"
#include "visual/visual_parameters.hpp"

#include <cstddef>

namespace prettyscope
{
struct VisualPalette
{
    const char* key;
    VisualParameterStableId numericId;
    const char* label;
    const char* description;
    RgbColor traceColor;
    RgbColor glowColor;
    RgbColor backgroundColor;
};

const VisualPalette* visualPalettes(size_t& count);
const VisualPalette* visualPaletteAt(size_t index);
void applyVisualPalette(VisualParams& params, size_t index);
}
