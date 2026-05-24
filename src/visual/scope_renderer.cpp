#include "visual/scope_renderer.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace prettyscope
{
namespace
{
constexpr const char* kVertexShader = R"GLSL(
#version 150

in vec2 segmentStart;
in vec2 segmentEnd;
in float cornerIndex;

uniform vec2 viewportSize;
uniform float beamSize;

out vec3 beamFrame;

void main()
{
    vec2 startPx = (segmentStart * 0.5 + 0.5) * viewportSize;
    vec2 endPx = (segmentEnd * 0.5 + 0.5) * viewportSize;
    vec2 dir = endPx - startPx;
    float len = length(dir);

    if (len > 0.0001) {
        dir /= len;
    } else {
        dir = vec2(1.0, 0.0);
    }

    vec2 norm = vec2(-dir.y, dir.x);
    float idx = mod(cornerIndex, 4.0);
    float side = (mod(idx, 2.0) - 0.5) * 2.0;
    float tang;
    vec2 current;

    if (idx >= 2.0) {
        current = endPx;
        tang = 1.0;
        beamFrame.x = -beamSize;
    } else {
        current = startPx;
        tang = -1.0;
        beamFrame.x = len + beamSize;
    }

    beamFrame.y = side * beamSize;
    beamFrame.z = len;

    vec2 positionPx = current + (tang * dir + side * norm) * beamSize;
    vec2 clip = positionPx / viewportSize * 2.0 - 1.0;
    gl_Position = vec4(clip, 0.0, 1.0);
}
)GLSL";

constexpr const char* kFragmentShader = R"GLSL(
#version 150

#define EPS 1e-6
#define SQRT2 1.4142135623730951

uniform vec3 beamColor;
uniform float beamSize;
uniform float beamIntensity;

in vec3 beamFrame;

out vec4 fragColor;

float erfApprox(float x)
{
    float s = sign(x);
    float a = abs(x);
    float b = 1.0 + (0.278393 + (0.230389 + 0.078108 * a * a) * a) * a;
    b *= b;
    return s - s / (b * b);
}

void main()
{
    float len = beamFrame.z;
    float sigma = max(beamSize * 0.25, 0.001);
    float alpha;

    if (len < EPS) {
        alpha = exp(-dot(beamFrame.xy, beamFrame.xy) / (2.0 * sigma * sigma)) / max(2.0 * sqrt(beamSize), 1.0);
    } else {
        alpha = erfApprox(beamFrame.x / (SQRT2 * sigma)) - erfApprox((beamFrame.x - len) / (SQRT2 * sigma));
        alpha *= exp(-beamFrame.y * beamFrame.y / (2.0 * sigma * sigma)) * beamSize / (2.0 * len);
    }

    alpha = clamp(alpha * beamIntensity, 0.0, 1.0);
    fragColor = vec4(beamColor, alpha);
}
)GLSL";

constexpr const char* kScreenVertexShader = R"GLSL(
#version 150

in vec2 position;
in vec2 texcoord;

out vec2 uv;

void main()
{
    uv = texcoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
)GLSL";

constexpr const char* kTextureFragmentShader = R"GLSL(
#version 150

uniform sampler2D image;

in vec2 uv;

out vec4 fragColor;

void main()
{
    fragColor = texture(image, uv);
}
)GLSL";

constexpr const char* kFadeFragmentShader = R"GLSL(
#version 150

uniform vec3 backgroundColor;
uniform float fadeAlpha;

out vec4 fragColor;

void main()
{
    fragColor = vec4(backgroundColor, fadeAlpha);
}
)GLSL";

constexpr const char* kDecayFragmentShader = R"GLSL(
#version 150

uniform sampler2D image;
uniform vec3 backgroundColor;
uniform float persistence;
uniform float fastDecay;
uniform float afterglow;
uniform int decayStyle;

in vec2 uv;

out vec4 fragColor;

void main()
{
    vec3 src = texture(image, uv).rgb;
    vec3 signal = max(src - backgroundColor, vec3(0.0));

    if (decayStyle == 1) {
        float brightness = max(max(signal.r, signal.g), signal.b);
        float dimTail = 1.0 - smoothstep(0.015, 0.34, brightness);
        float softTail = 1.0 - smoothstep(0.18, 0.82, brightness);
        float brightDrain = brightness * mix(0.035, 0.24, fastDecay);
        float tailBoost = dimTail * mix(0.0, 0.055, afterglow) + softTail * afterglow * 0.012;
        float keep = clamp(persistence + tailBoost - brightDrain, 0.0, mix(0.982, 0.9975, afterglow));
        signal *= keep;
        signal = max(signal - vec3(mix(0.0009, 0.00018, afterglow)), vec3(0.0));
        signal = pow(signal, vec3(mix(1.035, 1.012, afterglow)));
    } else {
        signal *= persistence;
    }

    fragColor = vec4(backgroundColor + signal, 1.0);
}
)GLSL";

constexpr const char* kSolidVertexShader = R"GLSL(
#version 150

in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
)GLSL";

constexpr const char* kSolidFragmentShader = R"GLSL(
#version 150

uniform vec4 fillColor;

out vec4 fragColor;

void main()
{
    fragColor = fillColor;
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

unsigned int createProgram(const char* vertexSource, const char* fragmentSource)
{
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    const unsigned int program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "texcoord");
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

unsigned int createBeamProgram()
{
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShader);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    const unsigned int program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, 0, "segmentStart");
    glBindAttribLocation(program, 1, "segmentEnd");
    glBindAttribLocation(program, 2, "cornerIndex");
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

void setColorUniform(unsigned int program, const RgbColor& color)
{
    glUniform3f(glGetUniformLocation(program, "beamColor"), color.r, color.g, color.b);
}
}

ScopeRenderer::~ScopeRenderer()
{
    destroy();
}

void ScopeRenderer::initialize()
{
    beamProgram_ = createBeamProgram();
    screenProgram_ = createProgram(kScreenVertexShader, kTextureFragmentShader);
    fadeProgram_ = createProgram(kScreenVertexShader, kFadeFragmentShader);
    decayProgram_ = createProgram(kScreenVertexShader, kDecayFragmentShader);
    solidProgram_ = createProgram(kSolidVertexShader, kSolidFragmentShader);

    glGenVertexArrays(1, &vertexArray_);
    glBindVertexArray(vertexArray_);

    glGenBuffers(1, &vertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BeamVertex), reinterpret_cast<void*>(offsetof(BeamVertex, startX)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BeamVertex), reinterpret_cast<void*>(offsetof(BeamVertex, endX)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(BeamVertex), reinterpret_cast<void*>(offsetof(BeamVertex, corner)));

    glBindVertexArray(0);

    constexpr float screenVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    glGenVertexArrays(1, &screenVertexArray_);
    glBindVertexArray(screenVertexArray_);

    glGenBuffers(1, &screenVertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, screenVertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));

    glBindVertexArray(0);

    glGenVertexArrays(1, &overlayVertexArray_);
    glBindVertexArray(overlayVertexArray_);

    glGenBuffers(1, &overlayVertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, overlayVertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1024, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
}

void ScopeRenderer::render(const SignalBuffer& signal, const VisualParams& params, int width, int height)
{
    if (width < 1 || height < 1)
    {
        return;
    }

    const int segmentCount = signal.size() - 1;
    ensureVertexCapacity(segmentCount);
    const int vertexCount = uploadSegments(signal, params, width, height);

    if (params.persistenceEnabled)
    {
        renderPersistentFrame(signal, params, width, height, vertexCount);
    }
    else
    {
        renderImmediateFrame(signal, params, width, height, vertexCount);
    }

    drawGridOverlay(params, width, height);
    drawFpsOverlay(params, width, height);
}

void ScopeRenderer::renderPersistentFrame(const SignalBuffer&, const VisualParams& params, int width, int height, int vertexCount)
{
    ensurePersistenceTargets(width, height);

    if (lastClearRevision_ != params.clearRevision)
    {
        clearPersistenceTarget(params, width, height);
        lastClearRevision_ = params.clearRevision;
    }

    decayPersistence(params, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, persistenceFramebuffers_[activePersistenceTarget_]);
    glViewport(0, 0, width, height);

    glUseProgram(beamProgram_);
    glBindVertexArray(vertexArray_);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    drawBeam(vertexCount, params.glowColor, params.glowWidth, params.glowStrength, width, height);
    drawBeam(vertexCount, params.traceColor, params.traceWidth, 1.15f, width, height);

    glBindVertexArray(0);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    presentTexture(persistenceTextures_[activePersistenceTarget_]);
}

void ScopeRenderer::renderImmediateFrame(const SignalBuffer&, const VisualParams& params, int width, int height, int vertexCount)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(
        params.backgroundColor.r,
        params.backgroundColor.g,
        params.backgroundColor.b,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(beamProgram_);
    glBindVertexArray(vertexArray_);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    drawBeam(vertexCount, params.glowColor, params.glowWidth, params.glowStrength, width, height);
    drawBeam(vertexCount, params.traceColor, params.traceWidth, 1.15f, width, height);

    glBindVertexArray(0);
    glUseProgram(0);
}

void ScopeRenderer::ensureVertexCapacity(int segmentCount)
{
    const int requiredVertexCount = segmentCount * 6;
    if (requiredVertexCount <= vertexCapacity_)
    {
        return;
    }

    vertexCapacity_ = requiredVertexCount;
    vertices_.resize(static_cast<size_t>(requiredVertexCount));
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(BeamVertex), nullptr, GL_DYNAMIC_DRAW);
}

void ScopeRenderer::ensurePersistenceTargets(int width, int height)
{
    if (persistenceFramebuffers_[0] != 0 && persistenceWidth_ == width && persistenceHeight_ == height)
    {
        return;
    }

    for (int i = 0; i < 2; ++i)
    {
        if (persistenceFramebuffers_[i] != 0)
        {
            glDeleteFramebuffers(1, &persistenceFramebuffers_[i]);
            persistenceFramebuffers_[i] = 0;
        }

        if (persistenceTextures_[i] != 0)
        {
            glDeleteTextures(1, &persistenceTextures_[i]);
            persistenceTextures_[i] = 0;
        }
    }

    createPersistenceTarget(0, width, height);
    createPersistenceTarget(1, width, height);
    persistenceWidth_ = width;
    persistenceHeight_ = height;
    activePersistenceTarget_ = 0;

    clearPersistenceTarget(VisualParams{}, width, height);
    lastClearRevision_ = -1;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScopeRenderer::createPersistenceTarget(int index, int width, int height)
{
    glGenTextures(1, &persistenceTextures_[index]);
    glBindTexture(GL_TEXTURE_2D, persistenceTextures_[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &persistenceFramebuffers_[index]);
    glBindFramebuffer(GL_FRAMEBUFFER, persistenceFramebuffers_[index]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, persistenceTextures_[index], 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("Could not create persistence framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScopeRenderer::clearPersistenceTarget(const VisualParams& params, int width, int height)
{
    for (unsigned int framebuffer : persistenceFramebuffers_)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, width, height);
        glClearColor(
            params.backgroundColor.r,
            params.backgroundColor.g,
            params.backgroundColor.b,
            1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void ScopeRenderer::fadePersistence(const VisualParams& params)
{
    glUseProgram(fadeProgram_);
    glBindVertexArray(screenVertexArray_);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform3f(
        glGetUniformLocation(fadeProgram_, "backgroundColor"),
        params.backgroundColor.r,
        params.backgroundColor.g,
        params.backgroundColor.b);
    glUniform1f(glGetUniformLocation(fadeProgram_, "fadeAlpha"), 1.0f - params.persistence);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

void ScopeRenderer::decayPersistence(const VisualParams& params, int width, int height)
{
    const int source = activePersistenceTarget_;
    const int destination = 1 - activePersistenceTarget_;

    glBindFramebuffer(GL_FRAMEBUFFER, persistenceFramebuffers_[destination]);
    glViewport(0, 0, width, height);
    glUseProgram(decayProgram_);
    glBindVertexArray(screenVertexArray_);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, persistenceTextures_[source]);
    glUniform1i(glGetUniformLocation(decayProgram_, "image"), 0);
    glUniform3f(
        glGetUniformLocation(decayProgram_, "backgroundColor"),
        params.backgroundColor.r,
        params.backgroundColor.g,
        params.backgroundColor.b);
    glUniform1f(glGetUniformLocation(decayProgram_, "persistence"), params.persistence);
    glUniform1f(glGetUniformLocation(decayProgram_, "fastDecay"), params.fastDecay);
    glUniform1f(glGetUniformLocation(decayProgram_, "afterglow"), params.afterglow);
    glUniform1i(
        glGetUniformLocation(decayProgram_, "decayStyle"),
        params.decayStyle == DecayStyle::Phosphor ? 1 : 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glUseProgram(0);

    activePersistenceTarget_ = destination;
}

void ScopeRenderer::presentTexture(unsigned int texture)
{
    glUseProgram(screenProgram_);
    glBindVertexArray(screenVertexArray_);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(screenProgram_, "image"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glUseProgram(0);
}

void ScopeRenderer::drawGridOverlay(const VisualParams& params, int width, int height)
{
    if (!params.showGrid)
    {
        return;
    }

    overlayVertices_.clear();

    const float line = 1.0f;

    if (params.traceMode == TraceMode::Xy)
    {
        constexpr int divisions = 8;
        const float side = static_cast<float>(std::min(width, height));
        const float left = (static_cast<float>(width) - side) * 0.5f;
        const float top = (static_cast<float>(height) - side) * 0.5f;

        for (int i = 0; i <= divisions; ++i)
        {
            const float offset = static_cast<float>(i) / static_cast<float>(divisions) * side;
            const float thickness = i == divisions / 2 ? 2.0f : line;
            appendRect(left + offset - thickness * 0.5f, top, thickness, side, width, height);
            appendRect(left, top + offset - thickness * 0.5f, side, thickness, width, height);
        }
    }
    else
    {
        constexpr int divisionsX = 10;
        constexpr int divisionsY = 8;

        for (int i = 0; i <= divisionsX; ++i)
        {
            const float x = static_cast<float>(i) / static_cast<float>(divisionsX) * static_cast<float>(width);
            const float thickness = i == divisionsX / 2 ? 2.0f : line;
            appendRect(x - thickness * 0.5f, 0.0f, thickness, static_cast<float>(height), width, height);
        }

        for (int i = 0; i <= divisionsY; ++i)
        {
            const float y = static_cast<float>(i) / static_cast<float>(divisionsY) * static_cast<float>(height);
            const float thickness = i == divisionsY / 2 ? 2.0f : line;
            appendRect(0.0f, y - thickness * 0.5f, static_cast<float>(width), thickness, width, height);
        }
    }

    const float alpha = std::max(0.0f, std::min(1.0f, params.gridIntensity));
    drawOverlayVertices(0.45f, 0.95f, 0.72f, alpha);
}

void ScopeRenderer::drawFpsOverlay(const VisualParams& params, int width, int height)
{
    if (!params.showFps)
    {
        return;
    }

    char text[8] = {};
    const int fps = std::max(0, std::min(999, static_cast<int>(params.fps + 0.5f)));
    std::snprintf(text, sizeof(text), "FPS %03d", fps);

    overlayVertices_.clear();

    const float scale = 1.2f;
    const float glyphWidth = 12.0f * scale;
    const float spacing = 4.0f * scale;
    const int glyphCount = static_cast<int>(std::string(text).size());
    const float totalWidth = static_cast<float>(glyphCount) * glyphWidth + static_cast<float>(glyphCount - 1) * spacing;
    const float y = 16.0f;
    float x = static_cast<float>(width) - totalWidth - 18.0f;

    for (char glyph : std::string(text))
    {
        appendGlyph(glyph, x, y, scale, width, height);
        x += glyphWidth + spacing;
    }

    drawOverlayVertices(0.82f, 1.0f, 0.88f, 0.78f);
}

void ScopeRenderer::drawOverlayVertices(float r, float g, float b, float a)
{
    if (overlayVertices_.empty())
    {
        return;
    }

    glUseProgram(solidProgram_);
    glBindVertexArray(overlayVertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, overlayVertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, overlayVertices_.size() * sizeof(float), overlayVertices_.data(), GL_DYNAMIC_DRAW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform4f(glGetUniformLocation(solidProgram_, "fillColor"), r, g, b, a);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(overlayVertices_.size() / 2));
    glBindVertexArray(0);
    glUseProgram(0);
}

void ScopeRenderer::appendGlyph(char glyph, float x, float y, float scale, int width, int height)
{
    unsigned char mask = 0;

    switch (glyph)
    {
    case '0': mask = 0b0111111; break;
    case '1': mask = 0b0000110; break;
    case '2': mask = 0b1011011; break;
    case '3': mask = 0b1001111; break;
    case '4': mask = 0b1100110; break;
    case '5': mask = 0b1101101; break;
    case '6': mask = 0b1111101; break;
    case '7': mask = 0b0000111; break;
    case '8': mask = 0b1111111; break;
    case '9': mask = 0b1101111; break;
    case 'F': mask = 0b1110001; break;
    case 'P': mask = 0b1110011; break;
    case 'S': mask = 0b1101101; break;
    default: break;
    }

    for (int segment = 0; segment < 7; ++segment)
    {
        if ((mask & (1 << segment)) != 0)
        {
            appendSegmentRect(segment, x, y, scale, width, height);
        }
    }
}

void ScopeRenderer::appendSegmentRect(int segment, float x, float y, float scale, int width, int height)
{
    const float t = 2.5f * scale;
    const float w = 12.0f * scale;
    const float h = 20.0f * scale;

    switch (segment)
    {
    case 0: appendRect(x + t, y, w - t * 2.0f, t, width, height); break;
    case 1: appendRect(x + w - t, y + t, t, h * 0.5f - t, width, height); break;
    case 2: appendRect(x + w - t, y + h * 0.5f, t, h * 0.5f - t, width, height); break;
    case 3: appendRect(x + t, y + h - t, w - t * 2.0f, t, width, height); break;
    case 4: appendRect(x, y + h * 0.5f, t, h * 0.5f - t, width, height); break;
    case 5: appendRect(x, y + t, t, h * 0.5f - t, width, height); break;
    case 6: appendRect(x + t, y + h * 0.5f - t * 0.5f, w - t * 2.0f, t, width, height); break;
    default: break;
    }
}

void ScopeRenderer::appendRect(float x, float y, float w, float h, int width, int height)
{
    const float x0 = x / static_cast<float>(width) * 2.0f - 1.0f;
    const float x1 = (x + w) / static_cast<float>(width) * 2.0f - 1.0f;
    const float y0 = 1.0f - y / static_cast<float>(height) * 2.0f;
    const float y1 = 1.0f - (y + h) / static_cast<float>(height) * 2.0f;

    const float rect[] = {
        x0, y1,
        x1, y1,
        x0, y0,
        x0, y0,
        x1, y1,
        x1, y0,
    };

    overlayVertices_.insert(overlayVertices_.end(), std::begin(rect), std::end(rect));
}

ScopeRenderer::ScopePoint ScopeRenderer::mapSample(const SignalBuffer& signal, const VisualParams& params, int index, int width, int height) const
{
    if (params.traceMode == TraceMode::Xy)
    {
        const float aspect = width > height
            ? static_cast<float>(height) / static_cast<float>(width)
            : 1.0f;
        const float verticalAspect = height > width
            ? static_cast<float>(width) / static_cast<float>(height)
            : 1.0f;

        return {
            signal[index] * params.traceGain * 0.82f * aspect,
            signal.right(index) * params.traceGain * 0.82f * verticalAspect};
    }

    return {
        static_cast<float>(index) / static_cast<float>(signal.size() - 1) * 1.84f - 0.92f,
        signal[index] * params.traceGain};
}

int ScopeRenderer::uploadSegments(const SignalBuffer& signal, const VisualParams& params, int width, int height)
{
    int vertexIndex = 0;
    constexpr float corners[6] = {0.0f, 1.0f, 2.0f, 2.0f, 1.0f, 3.0f};

    for (int i = 0; i < signal.size() - 1; ++i)
    {
        const ScopePoint start = mapSample(signal, params, i, width, height);
        const ScopePoint end = mapSample(signal, params, i + 1, width, height);

        for (float corner : corners)
        {
            vertices_[static_cast<size_t>(vertexIndex++)] = {start.x, start.y, end.x, end.y, corner};
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<size_t>(vertexIndex) * sizeof(BeamVertex), vertices_.data());
    return vertexIndex;
}

void ScopeRenderer::drawBeam(int vertexCount, const RgbColor& color, float size, float intensity, int width, int height)
{
    setColorUniform(beamProgram_, color);
    glUniform1f(glGetUniformLocation(beamProgram_, "beamSize"), size);
    glUniform1f(glGetUniformLocation(beamProgram_, "beamIntensity"), intensity);
    glUniform2f(glGetUniformLocation(beamProgram_, "viewportSize"), static_cast<float>(width), static_cast<float>(height));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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

    if (screenVertexBuffer_ != 0)
    {
        glDeleteBuffers(1, &screenVertexBuffer_);
        screenVertexBuffer_ = 0;
    }

    if (screenVertexArray_ != 0)
    {
        glDeleteVertexArrays(1, &screenVertexArray_);
        screenVertexArray_ = 0;
    }

    if (overlayVertexBuffer_ != 0)
    {
        glDeleteBuffers(1, &overlayVertexBuffer_);
        overlayVertexBuffer_ = 0;
    }

    if (overlayVertexArray_ != 0)
    {
        glDeleteVertexArrays(1, &overlayVertexArray_);
        overlayVertexArray_ = 0;
    }

    for (int i = 0; i < 2; ++i)
    {
        if (persistenceFramebuffers_[i] != 0)
        {
            glDeleteFramebuffers(1, &persistenceFramebuffers_[i]);
            persistenceFramebuffers_[i] = 0;
        }

        if (persistenceTextures_[i] != 0)
        {
            glDeleteTextures(1, &persistenceTextures_[i]);
            persistenceTextures_[i] = 0;
        }
    }

    if (beamProgram_ != 0)
    {
        glDeleteProgram(beamProgram_);
        beamProgram_ = 0;
    }

    if (screenProgram_ != 0)
    {
        glDeleteProgram(screenProgram_);
        screenProgram_ = 0;
    }

    if (fadeProgram_ != 0)
    {
        glDeleteProgram(fadeProgram_);
        fadeProgram_ = 0;
    }

    if (decayProgram_ != 0)
    {
        glDeleteProgram(decayProgram_);
        decayProgram_ = 0;
    }

    if (solidProgram_ != 0)
    {
        glDeleteProgram(solidProgram_);
        solidProgram_ = 0;
    }
}
}
