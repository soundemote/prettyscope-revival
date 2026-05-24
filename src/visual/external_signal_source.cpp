#include "visual/external_signal_source.hpp"

#include <algorithm>

namespace prettyscope
{
namespace
{
int positiveCount(int sampleCount)
{
    return std::max(0, sampleCount);
}

void copySamples(std::vector<float>& destination, const float* source, int sampleCount)
{
    destination.resize(static_cast<size_t>(sampleCount));
    std::copy(source, source + sampleCount, destination.begin());
}
}

void ExternalSignalSource::reserve(int maxSampleCount)
{
    const int count = positiveCount(maxSampleCount);
    left_.reserve(static_cast<size_t>(count));
    right_.reserve(static_cast<size_t>(count));
}

void ExternalSignalSource::setInputChannels(const float* const* channels, int channelCount, int sampleCount)
{
    if (channels == nullptr || channelCount <= 0)
    {
        setStereoInput(nullptr, nullptr, sampleCount);
        return;
    }

    const float* left = channels[0];
    const float* right = channelCount > 1 ? channels[1] : nullptr;

    if (channelCount == 1)
    {
        setMonoInput(left, sampleCount);
        return;
    }

    setStereoInput(left, right, sampleCount);
}

void ExternalSignalSource::setMonoInput(const float* samples, int sampleCount)
{
    const int count = positiveCount(sampleCount);
    if (samples == nullptr || count == 0)
    {
        left_.clear();
        right_.clear();
        layout_ = SignalLayout::Mono;
        return;
    }

    copySamples(left_, samples, count);
    right_.assign(left_.begin(), left_.end());
    layout_ = SignalLayout::Mono;
}

void ExternalSignalSource::setStereoInput(const float* left, const float* right, int sampleCount)
{
    const int count = positiveCount(sampleCount);
    if (count == 0 || (left == nullptr && right == nullptr))
    {
        left_.clear();
        right_.clear();
        layout_ = SignalLayout::Stereo;
        return;
    }

    if (left == nullptr)
    {
        copySamples(right_, right, count);
        left_.assign(right_.begin(), right_.end());
        layout_ = SignalLayout::Stereo;
        return;
    }

    copySamples(left_, left, count);
    if (right == nullptr)
    {
        right_.assign(left_.begin(), left_.end());
    }
    else
    {
        copySamples(right_, right, count);
    }
    layout_ = SignalLayout::Stereo;
}

int ExternalSignalSource::sampleCount() const
{
    return static_cast<int>(left_.size());
}

SignalSourceInfo ExternalSignalSource::info() const
{
    return {"external", layout_, layout_ == SignalLayout::Stereo ? TraceMode::Xy : TraceMode::Time};
}

void ExternalSignalSource::advance(SignalBuffer& signal, float)
{
    if (left_.empty())
    {
        for (int i = 0; i < signal.size(); ++i)
        {
            signal[i] = 0.0f;
            signal.right(i) = 0.0f;
        }
        return;
    }

    for (int i = 0; i < signal.size(); ++i)
    {
        const int sourceIndex = i * static_cast<int>(left_.size()) / signal.size();
        signal[i] = left_[static_cast<size_t>(sourceIndex)];
        signal.right(i) = right_[static_cast<size_t>(sourceIndex)];
    }
}
}
