#include "visual/scope_engine.hpp"

#include <iostream>

namespace
{
bool expectEqual(const char* name, int actual, int expected)
{
    if (actual == expected)
    {
        return true;
    }

    std::cerr << name << " expected " << expected << " but got " << actual << '\n';
    return false;
}
}

int main()
{
    prettyscope::ScopeEngine scope(1);

    bool passed = true;
    passed = expectEqual("minimum sample count", scope.sampleCount(), 2) && passed;

    scope.setSampleCount(2048);
    passed = expectEqual("expanded sample count", scope.sampleCount(), 2048) && passed;
    passed = expectEqual("signal size follows sample count", scope.signal().size(), 2048) && passed;

    scope.setSampleCount(-8);
    passed = expectEqual("negative sample count clamps", scope.sampleCount(), 2) && passed;

    return passed ? 0 : 1;
}
