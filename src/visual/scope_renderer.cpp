#include "visual/scope_renderer.hpp"

#include <glad/glad.h>

#include <stdexcept>
#include <string>

namespace prettyscope
{
namespace
{
constexpr const char* kVertexShader = R"GLSL(
#version 150

in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
)GLSL";

constexpr const char* kFragmentShader = R"GLSL(
#version 150

uniform vec3 color;
uniform float alpha;

out vec4 fragColor;

void main()
{
    fragColor = vec4(color, alpha);
}
)GLSL";

unsigned int compileShader(unsigned int type, const char* source)
{
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char log[1024] = {};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("OpenGL shader compile failed: ") + log);
    }

    return shader;
}

unsigned int createProgram()
{
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShader);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    const unsigned int program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, 0, "position");
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char log[1024] = {};
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);
        throw std::runtime_error(std::string("OpenGL shader link failed: ") + log);
    }

    return program;
}

void setColorUniform(unsigned int program, const char* name, const RgbColor& color)
{
    glUniform3f(glGetUniformLocation(program, name), color.r, color.g, color.b);
}
}

ScopeRenderer::~ScopeRenderer()
{
    destroy();
}

void ScopeRenderer::initialize()
{
    program_ = createProgram();

    glGenVertexArrays(1, &vertexArray_);
    glBindVertexArray(vertexArray_);

    glGenBuffers(1, &vertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ScopeRenderer::render(const SignalBuffer& signal, const VisualParams& params, int width, int height)
{
    if (width < 1 || height < 1)
    {
        return;
    }

    ensureVertexCapacity(signal.size());
    uploadTrace(signal, params.traceGain);

    glViewport(0, 0, width, height);
    glClearColor(
        params.backgroundColor.r,
        params.backgroundColor.g,
        params.backgroundColor.b,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);
    glBindVertexArray(vertexArray_);

    setColorUniform(program_, "color", params.glowColor);
    glUniform1f(glGetUniformLocation(program_, "alpha"), params.glowStrength);
    glLineWidth(7.0f);
    glDrawArrays(GL_LINE_STRIP, 0, signal.size());

    setColorUniform(program_, "color", params.traceColor);
    glUniform1f(glGetUniformLocation(program_, "alpha"), 0.95f);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, signal.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

void ScopeRenderer::ensureVertexCapacity(int sampleCount)
{
    if (sampleCount <= vertexCapacity_)
    {
        return;
    }

    vertexCapacity_ = sampleCount;
    vertices_.resize(static_cast<size_t>(sampleCount) * 2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
}

void ScopeRenderer::uploadTrace(const SignalBuffer& signal, float gain)
{
    for (int i = 0; i < signal.size(); ++i)
    {
        const float x = static_cast<float>(i) / static_cast<float>(signal.size() - 1);
        vertices_[static_cast<size_t>(i) * 2] = x * 1.84f - 0.92f;
        vertices_[static_cast<size_t>(i) * 2 + 1] = signal[i] * gain;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(float), vertices_.data());
}

void ScopeRenderer::destroy()
{
    if (vertexBuffer_ != 0)
    {
        glDeleteBuffers(1, &vertexBuffer_);
        vertexBuffer_ = 0;
    }

    if (vertexArray_ != 0)
    {
        glDeleteVertexArrays(1, &vertexArray_);
        vertexArray_ = 0;
    }

    if (program_ != 0)
    {
        glDeleteProgram(program_);
        program_ = 0;
    }
}
}
