#include "visual/test_signal.hpp"

#include <cmath>

namespace prettyscope
{
namespace
{
constexpr float kPi = 3.14159265358979323846f;
}

void TestSignalGenerator::advance(SignalBuffer& signal, float dt)
{
    phase_ += dt;

    for (int i = 0; i < signal.size(); ++i)
    {
        const float x = static_cast<float>(i) / static_cast<float>(signal.size() - 1);
        const float slowSweep = std::sin((x * 2.0f + phase_ * 0.37f) * kPi);
        const float carrier = std::sin((x * 7.0f + phase_ * 0.85f) * 2.0f * kPi);
        const float shimmer = std::sin((x * 31.0f - phase_ * 1.7f) * 2.0f * kPi);
        signal[i] = 0.56f * carrier + 0.26f * slowSweep + 0.08f * shimmer;
    }
}
}
