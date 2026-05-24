#include "prettyscope.hpp"

#include <array>

int main()
{
    prettyscope::ScopeEngine scope(8);
    prettyscope::ExternalSignalSource source;
    source.reserve(8);

    std::array<float, 8> left = {-0.4f, -0.3f, -0.2f, -0.1f, 0.1f, 0.2f, 0.3f, 0.4f};
    std::array<float, 8> right = {0.4f, 0.3f, 0.2f, 0.1f, -0.1f, -0.2f, -0.3f, -0.4f};
    const float* channels[] = {left.data(), right.data()};

    source.setInputChannels(channels, 2, static_cast<int>(left.size()));
    scope.advance(source, 0.0f);

    prettyscope::SignalSnapshot editorSnapshot(8);
    for (int i = 0; i < editorSnapshot.buffer().size(); ++i)
    {
        editorSnapshot.buffer()[i] = left[static_cast<size_t>(i)];
        editorSnapshot.buffer().right(i) = right[static_cast<size_t>(i)];
    }
    scope.advance(editorSnapshot, 0.0f);

    prettyscope::setNormalizedVisualFloatParameter(
        scope.params(),
        prettyscope::VisualFloatParameterId::TraceGain,
        0.25f);
    prettyscope::setVisualBoolParameter(scope.params(), prettyscope::VisualBoolParameterId::ShowGrid, true);
    prettyscope::applyVisualPalette(scope.params(), 0);

    return scope.signal().size() == 8 && source.sampleCount() == 8 ? 0 : 1;
}
