#include "visual/visual_palettes.hpp"

#include <iostream>

namespace
{
bool expectEqual(const char* name, float actual, float expected)
{
    if (actual == expected)
    {
        return true;
    }

    std::cerr << name << " expected " << expected << " but got " << actual << '\n';
    return false;
}

bool expectTrue(const char* name, bool value)
{
    if (value)
    {
        return true;
    }

    std::cerr << name << " expected true\n";
    return false;
}
}

int main()
{
    size_t count = 0;
    const prettyscope::VisualPalette* palettes = prettyscope::visualPalettes(count);

    bool passed = true;
    passed = expectTrue("palettes exist", palettes != nullptr) && passed;
    passed = expectTrue("palette count", count == 4) && passed;

    const prettyscope::VisualPalette* first = prettyscope::visualPaletteAt(0);
    passed = expectTrue("first palette", first != nullptr) && passed;
    if (first != nullptr)
    {
        passed = expectEqual("first trace red", first->traceColor.r, 1.0f) && passed;
        passed = expectEqual("first glow green", first->glowColor.g, 0.80f) && passed;
        passed = expectEqual("first background blue", first->backgroundColor.b, 0.026f) && passed;
    }

    prettyscope::VisualParams params;
    prettyscope::applyVisualPalette(params, 2);
    passed = expectEqual("apply trace green", params.traceColor.g, 1.0f) && passed;
    passed = expectEqual("apply glow red", params.glowColor.r, 0.16f) && passed;

    prettyscope::applyVisualPalette(params, 99);
    passed = expectEqual("invalid palette leaves values", params.traceColor.g, 1.0f) && passed;

    return passed ? 0 : 1;
}
