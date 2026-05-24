#include "platform/win32_gl_window.hpp"

#include <glad/glad.h>

#include <stdexcept>

namespace prettyscope
{
namespace
{
constexpr const char* kWindowClassName = "PrettyscopeOpenGlWindow";
using WglSwapIntervalExt = BOOL(WINAPI*)(int interval);

void throwWindowError(const char* message)
{
    throw std::runtime_error(message);
}
}

Win32GlWindow::Win32GlWindow(const char* title, int width, int height)
{
    instance_ = GetModuleHandleA(nullptr);
    createWindow(title, width, height);
    createOpenGlContext();
    ShowWindow(hwnd_, SW_SHOW);
}

Win32GlWindow::~Win32GlWindow()
{
    destroy();
}

bool Win32GlWindow::processMessages()
{
    MSG message;
    while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT)
        {
            running_ = false;
        }

        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return running_;
}

void Win32GlWindow::swapBuffers()
{
    SwapBuffers(deviceContext_);
}

bool Win32GlWindow::isKeyDown(int key) const
{
    if (key < 0 || key >= static_cast<int>(keys_.size()))
    {
        return false;
    }

    return keys_[static_cast<size_t>(key)];
}

int Win32GlWindow::consumeMouseWheelDelta()
{
    const int delta = mouseWheelDelta_;
    mouseWheelDelta_ = 0;
    return delta;
}

void Win32GlWindow::setTitle(const std::string& title)
{
    SetWindowTextA(hwnd_, title.c_str());
}

FramebufferSize Win32GlWindow::framebufferSize() const
{
    RECT rect;
    GetClientRect(hwnd_, &rect);
    return {static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top)};
}

LRESULT CALLBACK Win32GlWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_NCCREATE)
    {
        const auto* create = reinterpret_cast<CREATESTRUCTA*>(lparam);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
    }

    auto* window = reinterpret_cast<Win32GlWindow*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CLOSE:
        if (window != nullptr)
        {
            window->running_ = false;
        }
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (window != nullptr && wparam < window->keys_.size())
        {
            window->keys_[static_cast<size_t>(wparam)] = true;
        }

        if (wparam == VK_ESCAPE && window != nullptr)
        {
            window->running_ = false;
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    case WM_KEYUP:
        if (window != nullptr && wparam < window->keys_.size())
        {
            window->keys_[static_cast<size_t>(wparam)] = false;
        }
        break;
    case WM_MOUSEWHEEL:
        if (window != nullptr)
        {
            window->mouseWheelDelta_ += GET_WHEEL_DELTA_WPARAM(wparam);
            return 0;
        }
        break;
    default:
        break;
    }

    return DefWindowProcA(hwnd, message, wparam, lparam);
}

void Win32GlWindow::createWindow(const char* title, int width, int height)
{
    WNDCLASSA windowClass = {};
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = &Win32GlWindow::windowProc;
    windowClass.hInstance = instance_;
    windowClass.lpszClassName = kWindowClassName;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassA(&windowClass);

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd_ = CreateWindowExA(
        0,
        kWindowClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instance_,
        this);

    if (hwnd_ == nullptr)
    {
        throwWindowError("Could not create Prettyscope window");
    }
}

void Win32GlWindow::createOpenGlContext()
{
    deviceContext_ = GetDC(hwnd_);
    if (deviceContext_ == nullptr)
    {
        throwWindowError("Could not acquire window device context");
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pixelFormat = ChoosePixelFormat(deviceContext_, &pfd);
    if (pixelFormat == 0 || SetPixelFormat(deviceContext_, pixelFormat, &pfd) == FALSE)
    {
        throwWindowError("Could not choose an OpenGL pixel format");
    }

    glContext_ = wglCreateContext(deviceContext_);
    if (glContext_ == nullptr || wglMakeCurrent(deviceContext_, glContext_) == FALSE)
    {
        throwWindowError("Could not create an OpenGL context");
    }

    if (gladLoadGL() == 0)
    {
        throw std::runtime_error("Could not load OpenGL functions with GLAD");
    }

    const auto swapInterval = reinterpret_cast<WglSwapIntervalExt>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (swapInterval != nullptr)
    {
        swapInterval(1);
    }
}

void Win32GlWindow::destroy()
{
    if (glContext_ != nullptr)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(glContext_);
        glContext_ = nullptr;
    }

    if (hwnd_ != nullptr && deviceContext_ != nullptr)
    {
        ReleaseDC(hwnd_, deviceContext_);
        deviceContext_ = nullptr;
    }

    if (hwnd_ != nullptr)
    {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
}
}
