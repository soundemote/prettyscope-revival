#include "app/standalone_controls.hpp"

#include "app/preset_store.hpp"

#include "visual/visual_param_limits.hpp"
#include "visual/visual_parameters.hpp"
#include "visual/visual_palettes.hpp"

#include <cstdio>

namespace prettyscope
{
void StandaloneControls::update(Win32GlWindow& window, VisualParams& params, TestSignalGenerator& generator)
{
    const int wheelDelta = window.consumeMouseWheelDelta();
    if (wheelDelta != 0)
    {
        const float wheelSteps = static_cast<float>(wheelDelta) / static_cast<float>(WHEEL_DELTA);
        params.traceGain = clampValue(params.traceGain + wheelSteps * 0.08f, kMinTraceGain, kMaxTraceGain);
    }

    if (pressed(window, '1'))
    {
        params.traceGain = clampValue(params.traceGain - 0.08f, kMinTraceGain, kMaxTraceGain);
    }
    if (pressed(window, '2'))
    {
        params.traceGain = clampValue(params.traceGain + 0.08f, kMinTraceGain, kMaxTraceGain);
    }

    if (pressed(window, '3'))
    {
        params.glowStrength = clampValue(params.glowStrength - 0.06f, kMinGlowStrength, kMaxGlowStrength);
    }
    if (pressed(window, '4'))
    {
        params.glowStrength = clampValue(params.glowStrength + 0.06f, kMinGlowStrength, kMaxGlowStrength);
    }

    if (pressed(window, '5'))
    {
        params.traceWidth = clampValue(params.traceWidth - 0.5f, kMinTraceWidth, kMaxTraceWidth);
    }
    if (pressed(window, '6'))
    {
        params.traceWidth = clampValue(params.traceWidth + 0.5f, kMinTraceWidth, kMaxTraceWidth);
    }

    if (pressed(window, '7'))
    {
        params.glowWidth = clampValue(params.glowWidth - 1.0f, params.traceWidth, kMaxGlowWidth);
    }
    if (pressed(window, '8'))
    {
        params.glowWidth = clampValue(params.glowWidth + 1.0f, params.traceWidth, kMaxGlowWidth);
    }

    if (pressed(window, '9'))
    {
        params.persistence = clampValue(params.persistence - 0.03f, kMinPersistence, kMaxPersistence);
    }
    if (pressed(window, '0'))
    {
        params.persistence = clampValue(params.persistence + 0.03f, kMinPersistence, kMaxPersistence);
    }
    if (pressed(window, VK_OEM_4))
    {
        params.persistence = clampValue(params.persistence - 0.005f, kMinPersistence, kMaxPersistence);
    }
    if (pressed(window, VK_OEM_6))
    {
        params.persistence = clampValue(params.persistence + 0.005f, kMinPersistence, kMaxPersistence);
    }
    if (pressed(window, 'T'))
    {
        params.fastDecay = clampValue(params.fastDecay - 0.05f, kMinDecayAmount, kMaxDecayAmount);
    }
    if (pressed(window, 'Y'))
    {
        params.fastDecay = clampValue(params.fastDecay + 0.05f, kMinDecayAmount, kMaxDecayAmount);
    }
    if (pressed(window, VK_OEM_MINUS))
    {
        params.afterglow = clampValue(params.afterglow - 0.05f, kMinDecayAmount, kMaxDecayAmount);
    }
    if (pressed(window, VK_OEM_PLUS))
    {
        params.afterglow = clampValue(params.afterglow + 0.05f, kMinDecayAmount, kMaxDecayAmount);
    }
    if (pressed(window, 'P'))
    {
        setVisualBoolParameter(
            params,
            VisualBoolParameterId::PersistenceEnabled,
            !getVisualBoolParameter(params, VisualBoolParameterId::PersistenceEnabled));
    }
    if (pressed(window, 'D'))
    {
        const DecayStyle nextStyle = params.decayStyle == DecayStyle::Classic ? DecayStyle::Phosphor : DecayStyle::Classic;
        setVisualChoiceParameter(params, VisualChoiceParameterId::DecayStyle, static_cast<int>(nextStyle));
    }
    if (pressed(window, 'F'))
    {
        setVisualBoolParameter(params, VisualBoolParameterId::ShowFps, !getVisualBoolParameter(params, VisualBoolParameterId::ShowFps));
    }
    if (pressed(window, 'G'))
    {
        setVisualBoolParameter(params, VisualBoolParameterId::ShowGrid, !getVisualBoolParameter(params, VisualBoolParameterId::ShowGrid));
    }
    if (pressed(window, 'S'))
    {
        PresetStore::saveDefault(params, generator);
    }
    if (pressed(window, 'L'))
    {
        PresetStore::loadDefault(params, generator);
        clampVisualParams(params);
    }

    if (pressed(window, VK_SPACE))
    {
        generator.nextMode();
        ++params.clearRevision;
    }
    if (pressed(window, 'X'))
    {
        const TraceMode nextMode = params.traceMode == TraceMode::Time ? TraceMode::Xy : TraceMode::Time;
        setVisualChoiceParameter(params, VisualChoiceParameterId::TraceMode, static_cast<int>(nextMode));
    }
    if (pressed(window, 'C'))
    {
        cyclePalette(params);
        ++params.clearRevision;
    }
    if (pressed(window, 'R'))
    {
        const int nextClearRevision = params.clearRevision + 1;
        params = {};
        params.clearRevision = nextClearRevision;
        paletteIndex_ = 0;
    }
}

std::string StandaloneControls::titleText(const VisualParams& params, const TestSignalGenerator& generator) const
{
    char text[512] = {};
    std::snprintf(
        text,
        sizeof(text),
        "Prettyscope | %s | signal %s Space | color C | gain 1/2 %s | glow 3/4 %s | trace 5/6 %s | bloom 7/8 %s | decay 9/0 [/] %s | fast T/Y %s | tail -/= %s | %s P | style D %s | F fps | G grid | X mode | S save | L load | R reset",
        params.traceMode == TraceMode::Time ? "1D" : "XY",
        generator.modeName(),
        formatVisualFloatParameterValue(VisualFloatParameterId::TraceGain, params.traceGain).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::GlowStrength, params.glowStrength).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::TraceWidth, params.traceWidth).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::GlowWidth, params.glowWidth).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::Persistence, params.persistence).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::FastDecay, params.fastDecay).c_str(),
        formatVisualFloatParameterValue(VisualFloatParameterId::Afterglow, params.afterglow).c_str(),
        params.persistenceEnabled ? "on" : "off",
        params.decayStyle == DecayStyle::Classic ? "classic" : "phosphor");
    return text;
}

bool StandaloneControls::pressed(const Win32GlWindow& window, int key)
{
    const bool down = window.isKeyDown(key);
    const bool wasDown = previousKeys_[static_cast<size_t>(key)];
    previousKeys_[static_cast<size_t>(key)] = down;
    return down && !wasDown;
}

void StandaloneControls::cyclePalette(VisualParams& params)
{
    size_t count = 0;
    visualPalettes(count);
    if (count == 0)
    {
        return;
    }

    paletteIndex_ = (paletteIndex_ + 1) % count;
    applyVisualPalette(params, paletteIndex_);
}
}
