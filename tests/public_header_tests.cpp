#include "prettyscope.hpp"

int main()
{
    prettyscope::ScopeEngine scope(128);
    prettyscope::ExternalSignalSource source;
    prettyscope::VisualParams params;
    prettyscope::clampVisualParams(params);

    return scope.sampleCount() == 128 && source.info().layout == prettyscope::SignalLayout::Stereo ? 0 : 1;
}
