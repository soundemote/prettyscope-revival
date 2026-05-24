#include "visual/scope_engine.hpp"

namespace prettyscope
{
ScopeEngine::ScopeEngine(int sampleCount)
    : signal_(sampleCount)
{
}

void ScopeEngine::initialize()
{
    renderer_.initialize();
}

void ScopeEngine::render(SignalSource& source, float dt, int width, int height)
{
    source.advance(signal_, dt);
    renderer_.render(signal_, params_, width, height);
}

VisualParams& ScopeEngine::params()
{
    return params_;
}

const VisualParams& ScopeEngine::params() const
{
    return params_;
}

SignalBuffer& ScopeEngine::signal()
{
    return signal_;
}

const SignalBuffer& ScopeEngine::signal() const
{
    return signal_;
}
}
