#pragma once

#include "visual/signal_buffer.hpp"

namespace prettyscope
{
class TestSignalGenerator
{
public:
    enum class Mode
    {
        Musical,
        Lissajousish,
        BassPulse,
    };

    void advance(SignalBuffer& signal, float dt);
    void nextMode();
    void setMode(Mode mode);
    Mode mode() const;
    const char* modeName() const;

private:
    float phase_ = 0.0f;
    Mode mode_ = Mode::Lissajousish;
};
}
