#include "visual/signal_snapshot.hpp"

namespace prettyscope
{
SignalSnapshot::SignalSnapshot(int sampleCount)
    : buffer_(sampleCount)
{
}

void SignalSnapshot::setLayout(SignalLayout layout)
{
    layout_ = layout;
}

SignalLayout SignalSnapshot::layout() const
{
    return layout_;
}

SignalBuffer& SignalSnapshot::buffer()
{
    return buffer_;
}

const SignalBuffer& SignalSnapshot::buffer() const
{
    return buffer_;
}

SignalSourceInfo SignalSnapshot::info() const
{
    return {"snapshot", layout_, layout_ == SignalLayout::Stereo ? TraceMode::Xy : TraceMode::Time};
}

void SignalSnapshot::advance(SignalBuffer& signal, float)
{
    if (signal.size() != buffer_.size())
    {
        signal.resize(buffer_.size());
    }

    for (int i = 0; i < buffer_.size(); ++i)
    {
        signal[i] = buffer_[i];
        signal.right(i) = buffer_.right(i);
    }
}
}
