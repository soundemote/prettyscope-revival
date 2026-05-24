#pragma once

#include "visual/signal_buffer.hpp"
#include "visual/visual_params.hpp"

#include <vector>

namespace prettyscope
{
class ScopeRenderer
{
public:
    ScopeRenderer() = default;
    ~ScopeRenderer();

    ScopeRenderer(const ScopeRenderer&) = delete;
    ScopeRenderer& operator=(const ScopeRenderer&) = delete;

    void initialize();
    void render(const SignalBuffer& signal, const VisualParams& params, int width, int height);

private:
    struct BeamVertex
    {
        float startX = 0.0f;
        float startY = 0.0f;
        float endX = 0.0f;
        float endY = 0.0f;
        float corner = 0.0f;
    };

    struct ScopePoint
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    void ensureVertexCapacity(int segmentCount);
    void ensurePersistenceTargets(int width, int height);
    void createPersistenceTarget(int index, int width, int height);
    void renderPersistentFrame(const SignalBuffer& signal, const VisualParams& params, int width, int height, int vertexCount);
    void renderImmediateFrame(const SignalBuffer& signal, const VisualParams& params, int width, int height, int vertexCount);
    void clearPersistenceTarget(const VisualParams& params, int width, int height);
    void fadePersistence(const VisualParams& params);
    void decayPersistence(const VisualParams& params, int width, int height);
    void presentTexture(unsigned int texture);
    void drawGridOverlay(const VisualParams& params, int width, int height);
    void drawFpsOverlay(const VisualParams& params, int width, int height);
    void drawOverlayVertices(float r, float g, float b, float a);
    void appendGlyph(char glyph, float x, float y, float scale, int width, int height);
    void appendSegmentRect(int segment, float x, float y, float scale, int width, int height);
    void appendRect(float x, float y, float w, float h, int width, int height);
    ScopePoint mapSample(const SignalBuffer& signal, const VisualParams& params, int index, int width, int height) const;
    int uploadSegments(const SignalBuffer& signal, const VisualParams& params, int width, int height);
    void drawBeam(int vertexCount, const RgbColor& color, float size, float intensity, int width, int height);
    void destroy();

    unsigned int beamProgram_ = 0;
    unsigned int screenProgram_ = 0;
    unsigned int fadeProgram_ = 0;
    unsigned int decayProgram_ = 0;
    unsigned int solidProgram_ = 0;
    unsigned int vertexArray_ = 0;
    unsigned int vertexBuffer_ = 0;
    unsigned int screenVertexArray_ = 0;
    unsigned int screenVertexBuffer_ = 0;
    unsigned int overlayVertexArray_ = 0;
    unsigned int overlayVertexBuffer_ = 0;
    unsigned int persistenceFramebuffers_[2] = {};
    unsigned int persistenceTextures_[2] = {};
    int persistenceWidth_ = 0;
    int persistenceHeight_ = 0;
    int activePersistenceTarget_ = 0;
    int lastClearRevision_ = -1;
    int vertexCapacity_ = 0;
    std::vector<BeamVertex> vertices_;
    std::vector<float> overlayVertices_;
};
}
