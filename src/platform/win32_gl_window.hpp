#pragma once

#include <windows.h>

namespace prettyscope
{
struct FramebufferSize
{
    int width = 1;
    int height = 1;
};

class Win32GlWindow
{
public:
    Win32GlWindow(const char* title, int width, int height);
    ~Win32GlWindow();

    Win32GlWindow(const Win32GlWindow&) = delete;
    Win32GlWindow& operator=(const Win32GlWindow&) = delete;

    bool processMessages();
    void swapBuffers();
    FramebufferSize framebufferSize() const;

private:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

    void createWindow(const char* title, int width, int height);
    void createOpenGlContext();
    void destroy();

    HINSTANCE instance_ = nullptr;
    HWND hwnd_ = nullptr;
    HDC deviceContext_ = nullptr;
    HGLRC glContext_ = nullptr;
    bool running_ = true;
};
}
