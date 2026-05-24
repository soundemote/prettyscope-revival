#pragma once

#include "visual/signal_buffer.hpp"

namespace prettyscope
{
class TestSignalGenerator
{
public:
    void advance(SignalBuffer& signal, float dt);

private:
    float phase_ = 0.0f;
};
}
