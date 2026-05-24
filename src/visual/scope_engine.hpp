#pragma once

#include "visual/scope_renderer.hpp"
#include "visual/signal_buffer.hpp"
#include "visual/signal_source.hpp"
#include "visual/visual_params.hpp"

namespace prettyscope
{
class ScopeEngine
{
public:
    explicit ScopeEngine(int sampleCount);

    ScopeEngine(const ScopeEngine&) = delete;
    ScopeEngine& operator=(const ScopeEngine&) = delete;

    void initialize();
    void render(SignalSource& source, float dt, int width, int height);

    VisualParams& params();
    const VisualParams& params() const;

    SignalBuffer& signal();
    const SignalBuffer& signal() const;

private:
    VisualParams params_;
    SignalBuffer signal_;
    ScopeRenderer renderer_;
};
}
