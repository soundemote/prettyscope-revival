#include "platform/win32_gl_window.hpp"
#include "visual/scope_renderer.hpp"
#include "visual/signal_buffer.hpp"
#include "visual/test_signal.hpp"
#include "visual/visual_params.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

int main()
{
    try
    {
        prettyscope::Win32GlWindow window("Prettyscope - OpenGL Trace", 1280, 720);
        prettyscope::ScopeRenderer renderer;
        prettyscope::SignalBuffer signal(1024);
        prettyscope::TestSignalGenerator generator;

        prettyscope::VisualParams params;
        params.traceColor = {0.24f, 0.92f, 0.78f};
        params.glowColor = {0.95f, 0.20f, 0.72f};
        params.backgroundColor = {0.015f, 0.014f, 0.022f};

        renderer.initialize();

        auto previous = std::chrono::steady_clock::now();
        while (window.processMessages())
        {
            const auto now = std::chrono::steady_clock::now();
            const float dt = std::chrono::duration<float>(now - previous).count();
            previous = now;

            generator.advance(signal, dt);

            const auto size = window.framebufferSize();
            renderer.render(signal, params, size.width, size.height);
            window.swapBuffers();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Prettyscope failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
