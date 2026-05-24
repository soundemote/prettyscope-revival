#pragma once

#include "visual/signal_buffer.hpp"
#include "visual/visual_params.hpp"

#include <vector>

namespace prettyscope
{
class ScopeRenderer
{
public:
    ScopeRenderer() = default;
    ~ScopeRenderer();

    ScopeRenderer(const ScopeRenderer&) = delete;
    ScopeRenderer& operator=(const ScopeRenderer&) = delete;

    void initialize();
    void render(const SignalBuffer& signal, const VisualParams& params, int width, int height);

private:
    void ensureVertexCapacity(int sampleCount);
    void uploadTrace(const SignalBuffer& signal, float gain);
    void destroy();

    unsigned int program_ = 0;
    unsigned int vertexArray_ = 0;
    unsigned int vertexBuffer_ = 0;
    int vertexCapacity_ = 0;
    std::vector<float> vertices_;
};
}
