#pragma once

#include "visual/signal_source.hpp"

#include <mutex>
#include <vector>

namespace prettyscope
{
// Small host/demo bridge for copied mono/stereo buffers. This is intentionally
// simple; plugin wrappers may replace it with a host-specific lock-free source.
class ExternalSignalSource : public SignalSource
{
public:
    void reserve(int maxSampleCount);
    void setInputChannels(const float* const* channels, int channelCount, int sampleCount);
    void setMonoInput(const float* samples, int sampleCount);
    void setStereoInput(const float* left, const float* right, int sampleCount);
    int sampleCount() const;

    SignalSourceInfo info() const override;
    void advance(SignalBuffer& signal, float dt) override;

private:
    mutable std::mutex mutex_;
    std::vector<float> left_;
    std::vector<float> right_;
    SignalLayout layout_ = SignalLayout::Stereo;
};
}
