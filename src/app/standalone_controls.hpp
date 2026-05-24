#pragma once

#include "platform/win32_gl_window.hpp"
#include "visual/test_signal.hpp"
#include "visual/visual_params.hpp"

#include <array>
#include <string>

namespace prettyscope
{
class StandaloneControls
{
public:
    void update(Win32GlWindow& window, VisualParams& params, TestSignalGenerator& generator);
    std::string titleText(const VisualParams& params, const TestSignalGenerator& generator) const;

private:
    bool pressed(const Win32GlWindow& window, int key);
    void cyclePalette(VisualParams& params);

    std::array<bool, 256> previousKeys_ = {};
    int paletteIndex_ = 0;
};
}
