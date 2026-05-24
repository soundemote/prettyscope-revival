#include "visual/signal_snapshot.hpp"

#include <iostream>

namespace
{
bool expectEqual(const char* name, float actual, float expected)
{
    if (actual == expected)
    {
        return true;
    }

    std::cerr << name << " expected " << expected << " but got " << actual << '\n';
    return false;
}

bool expectTrue(const char* name, bool value)
{
    if (value)
    {
        return true;
    }

    std::cerr << name << " expected true\n";
    return false;
}
}

int main()
{
    prettyscope::SignalSnapshot snapshot(4);
    snapshot.buffer()[0] = -0.25f;
    snapshot.buffer().right(0) = 0.25f;
    snapshot.buffer()[3] = 0.75f;
    snapshot.buffer().right(3) = -0.75f;

    prettyscope::SignalBuffer output(2);
    snapshot.advance(output, 0.0f);

    bool passed = true;
    passed = expectTrue("output resized", output.size() == 4) && passed;
    passed = expectEqual("left copied", output[0], -0.25f) && passed;
    passed = expectEqual("right copied", output.right(0), 0.25f) && passed;
    passed = expectEqual("last left copied", output[3], 0.75f) && passed;
    passed = expectTrue("stereo suggests xy", snapshot.info().suggestedTraceMode == prettyscope::TraceMode::Xy) && passed;

    snapshot.setLayout(prettyscope::SignalLayout::Mono);
    passed = expectTrue("mono layout", snapshot.layout() == prettyscope::SignalLayout::Mono) && passed;
    passed = expectTrue("mono suggests time", snapshot.info().suggestedTraceMode == prettyscope::TraceMode::Time) && passed;

    return passed ? 0 : 1;
}
