#pragma once

#include <algorithm>
#include <vector>

namespace prettyscope
{
class SignalBuffer
{
public:
    explicit SignalBuffer(int sampleCount)
        : samples_(static_cast<size_t>(std::max(2, sampleCount)), 0.0f)
    {
    }

    int size() const
    {
        return static_cast<int>(samples_.size());
    }

    float* data()
    {
        return samples_.data();
    }

    const float* data() const
    {
        return samples_.data();
    }

    float& operator[](int index)
    {
        return samples_[static_cast<size_t>(index)];
    }

    float operator[](int index) const
    {
        return samples_[static_cast<size_t>(index)];
    }

private:
    std::vector<float> samples_;
};
}
