#include "visual/visual_palettes.hpp"

namespace prettyscope
{
namespace
{
constexpr VisualPalette kPalettes[] = {
    {"pinkCyan", "Pink/Cyan", {1.0f, 0.22f, 0.70f}, {0.18f, 0.80f, 1.0f}, {0.018f, 0.012f, 0.026f}},
    {"amberRed", "Amber/Red", {1.0f, 0.86f, 0.26f}, {1.0f, 0.18f, 0.08f}, {0.018f, 0.010f, 0.006f}},
    {"greenPhosphor", "Green Phosphor", {0.72f, 1.0f, 0.32f}, {0.16f, 0.92f, 0.62f}, {0.008f, 0.016f, 0.012f}},
    {"aquaMagenta", "Aqua/Magenta", {0.24f, 0.92f, 0.78f}, {0.95f, 0.20f, 0.72f}, {0.015f, 0.014f, 0.022f}},
};
}

const VisualPalette* visualPalettes(size_t& count)
{
    count = sizeof(kPalettes) / sizeof(kPalettes[0]);
    return kPalettes;
}

const VisualPalette* visualPaletteAt(size_t index)
{
    size_t count = 0;
    const VisualPalette* palettes = visualPalettes(count);
    if (index >= count)
    {
        return nullptr;
    }

    return &palettes[index];
}

void applyVisualPalette(VisualParams& params, size_t index)
{
    const VisualPalette* palette = visualPaletteAt(index);
    if (palette == nullptr)
    {
        return;
    }

    params.traceColor = palette->traceColor;
    params.glowColor = palette->glowColor;
    params.backgroundColor = palette->backgroundColor;
}
}
