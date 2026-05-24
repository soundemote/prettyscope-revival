#pragma once

#include "visual/test_signal.hpp"
#include "visual/visual_params.hpp"

namespace prettyscope
{
class PresetStore
{
public:
    static bool saveDefault(const VisualParams& params, const TestSignalGenerator& generator);
    static bool loadDefault(VisualParams& params, TestSignalGenerator& generator);
};
}
