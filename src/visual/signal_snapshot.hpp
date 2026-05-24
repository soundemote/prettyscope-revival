#pragma once

#include "visual/signal_buffer.hpp"
#include "visual/signal_source.hpp"

namespace prettyscope
{
class SignalSnapshot : public SignalSource
{
public:
    explicit SignalSnapshot(int sampleCount = 1024);

    void setLayout(SignalLayout layout);
    SignalLayout layout() const;

    SignalBuffer& buffer();
    const SignalBuffer& buffer() const;

    SignalSourceInfo info() const override;
    void advance(SignalBuffer& signal, float dt) override;

private:
    SignalBuffer buffer_;
    SignalLayout layout_ = SignalLayout::Stereo;
};
}
