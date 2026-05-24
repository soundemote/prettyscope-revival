#include "visual/visual_parameters.hpp"

#include "visual/visual_param_limits.hpp"

#include <cstdio>

namespace prettyscope
{
namespace
{
constexpr VisualParams kDefaults;

constexpr VisualFloatParameter kFloatParameters[] = {
    {VisualFloatParameterId::TraceGain, "traceGain", "Signal Gain", kMinTraceGain, kMaxTraceGain, kDefaults.traceGain},
    {VisualFloatParameterId::GlowStrength, "glowStrength", "Glow Strength", kMinGlowStrength, kMaxGlowStrength, kDefaults.glowStrength},
    {VisualFloatParameterId::TraceWidth, "traceWidth", "Trace Width", kMinTraceWidth, kMaxTraceWidth, kDefaults.traceWidth},
    {VisualFloatParameterId::GlowWidth, "glowWidth", "Glow Width", kMinTraceWidth, kMaxGlowWidth, kDefaults.glowWidth},
    {VisualFloatParameterId::Persistence, "persistence", "Decay", kMinPersistence, kMaxPersistence, kDefaults.persistence},
    {VisualFloatParameterId::FastDecay, "fastDecay", "Fast Decay", kMinDecayAmount, kMaxDecayAmount, kDefaults.fastDecay},
    {VisualFloatParameterId::Afterglow, "afterglow", "Tail Burn", kMinDecayAmount, kMaxDecayAmount, kDefaults.afterglow},
    {VisualFloatParameterId::GridIntensity, "gridIntensity", "Grid Intensity", 0.0f, 1.0f, kDefaults.gridIntensity},
};

constexpr VisualBoolParameter kBoolParameters[] = {
    {VisualBoolParameterId::PersistenceEnabled, "persistenceEnabled", "Persistence", kDefaults.persistenceEnabled},
    {VisualBoolParameterId::ShowFps, "showFps", "FPS Overlay", kDefaults.showFps},
    {VisualBoolParameterId::ShowGrid, "showGrid", "Grid Overlay", kDefaults.showGrid},
};

constexpr VisualChoiceOption kTraceModeOptions[] = {
    {static_cast<int>(TraceMode::Time), "time", "1D"},
    {static_cast<int>(TraceMode::Xy), "xy", "XY"},
};

constexpr VisualChoiceOption kDecayStyleOptions[] = {
    {static_cast<int>(DecayStyle::Classic), "classic", "Classic"},
    {static_cast<int>(DecayStyle::Phosphor), "phosphor", "Phosphor"},
};

constexpr VisualChoiceParameter kChoiceParameters[] = {
    {
        VisualChoiceParameterId::TraceMode,
        "traceMode",
        "Trace Mode",
        kTraceModeOptions,
        sizeof(kTraceModeOptions) / sizeof(kTraceModeOptions[0]),
        static_cast<int>(kDefaults.traceMode),
    },
    {
        VisualChoiceParameterId::DecayStyle,
        "decayStyle",
        "Decay Style",
        kDecayStyleOptions,
        sizeof(kDecayStyleOptions) / sizeof(kDecayStyleOptions[0]),
        static_cast<int>(kDefaults.decayStyle),
    },
};

bool choiceContainsValue(const VisualChoiceParameter& parameter, int value)
{
    for (size_t i = 0; i < parameter.optionCount; ++i)
    {
        if (parameter.options[i].value == value)
        {
            return true;
        }
    }

    return false;
}
}

const VisualFloatParameter* visualFloatParameters(size_t& count)
{
    count = sizeof(kFloatParameters) / sizeof(kFloatParameters[0]);
    return kFloatParameters;
}

const VisualFloatParameter* findVisualFloatParameter(VisualFloatParameterId id)
{
    size_t count = 0;
    const VisualFloatParameter* parameters = visualFloatParameters(count);
    for (size_t i = 0; i < count; ++i)
    {
        if (parameters[i].id == id)
        {
            return &parameters[i];
        }
    }

    return nullptr;
}

float getVisualFloatParameter(const VisualParams& params, VisualFloatParameterId id)
{
    switch (id)
    {
    case VisualFloatParameterId::TraceGain: return params.traceGain;
    case VisualFloatParameterId::GlowStrength: return params.glowStrength;
    case VisualFloatParameterId::TraceWidth: return params.traceWidth;
    case VisualFloatParameterId::GlowWidth: return params.glowWidth;
    case VisualFloatParameterId::Persistence: return params.persistence;
    case VisualFloatParameterId::FastDecay: return params.fastDecay;
    case VisualFloatParameterId::Afterglow: return params.afterglow;
    case VisualFloatParameterId::GridIntensity: return params.gridIntensity;
    }

    return 0.0f;
}

bool setVisualFloatParameter(VisualParams& params, VisualFloatParameterId id, float value)
{
    switch (id)
    {
    case VisualFloatParameterId::TraceGain: params.traceGain = value; break;
    case VisualFloatParameterId::GlowStrength: params.glowStrength = value; break;
    case VisualFloatParameterId::TraceWidth: params.traceWidth = value; break;
    case VisualFloatParameterId::GlowWidth: params.glowWidth = value; break;
    case VisualFloatParameterId::Persistence: params.persistence = value; break;
    case VisualFloatParameterId::FastDecay: params.fastDecay = value; break;
    case VisualFloatParameterId::Afterglow: params.afterglow = value; break;
    case VisualFloatParameterId::GridIntensity: params.gridIntensity = value; break;
    default: return false;
    }

    clampVisualParams(params);
    return true;
}

float normalizeVisualFloatParameter(const VisualFloatParameter& parameter, float value)
{
    if (parameter.maximum <= parameter.minimum)
    {
        return 0.0f;
    }

    const float clamped = clampValue(value, parameter.minimum, parameter.maximum);
    return (clamped - parameter.minimum) / (parameter.maximum - parameter.minimum);
}

float denormalizeVisualFloatParameter(const VisualFloatParameter& parameter, float normalizedValue)
{
    const float clamped = clampValue(normalizedValue, 0.0f, 1.0f);
    return parameter.minimum + clamped * (parameter.maximum - parameter.minimum);
}

float getNormalizedVisualFloatParameter(const VisualParams& params, VisualFloatParameterId id)
{
    const VisualFloatParameter* parameter = findVisualFloatParameter(id);
    if (parameter == nullptr)
    {
        return 0.0f;
    }

    return normalizeVisualFloatParameter(*parameter, getVisualFloatParameter(params, id));
}

bool setNormalizedVisualFloatParameter(VisualParams& params, VisualFloatParameterId id, float normalizedValue)
{
    const VisualFloatParameter* parameter = findVisualFloatParameter(id);
    if (parameter == nullptr)
    {
        return false;
    }

    return setVisualFloatParameter(params, id, denormalizeVisualFloatParameter(*parameter, normalizedValue));
}

std::string formatVisualFloatParameterValue(VisualFloatParameterId id, float value)
{
    char text[32] = {};

    switch (id)
    {
    case VisualFloatParameterId::TraceGain:
    case VisualFloatParameterId::GlowStrength:
    case VisualFloatParameterId::FastDecay:
    case VisualFloatParameterId::Afterglow:
    case VisualFloatParameterId::GridIntensity:
        std::snprintf(text, sizeof(text), "%.2f", value);
        break;

    case VisualFloatParameterId::TraceWidth:
    case VisualFloatParameterId::GlowWidth:
        std::snprintf(text, sizeof(text), "%.1f", value);
        break;

    case VisualFloatParameterId::Persistence:
        std::snprintf(text, sizeof(text), "%.3f", value);
        break;
    }

    return text;
}

const VisualBoolParameter* visualBoolParameters(size_t& count)
{
    count = sizeof(kBoolParameters) / sizeof(kBoolParameters[0]);
    return kBoolParameters;
}

const VisualBoolParameter* findVisualBoolParameter(VisualBoolParameterId id)
{
    size_t count = 0;
    const VisualBoolParameter* parameters = visualBoolParameters(count);
    for (size_t i = 0; i < count; ++i)
    {
        if (parameters[i].id == id)
        {
            return &parameters[i];
        }
    }

    return nullptr;
}

bool getVisualBoolParameter(const VisualParams& params, VisualBoolParameterId id)
{
    switch (id)
    {
    case VisualBoolParameterId::PersistenceEnabled: return params.persistenceEnabled;
    case VisualBoolParameterId::ShowFps: return params.showFps;
    case VisualBoolParameterId::ShowGrid: return params.showGrid;
    }

    return false;
}

bool setVisualBoolParameter(VisualParams& params, VisualBoolParameterId id, bool value)
{
    switch (id)
    {
    case VisualBoolParameterId::PersistenceEnabled: params.persistenceEnabled = value; return true;
    case VisualBoolParameterId::ShowFps: params.showFps = value; return true;
    case VisualBoolParameterId::ShowGrid: params.showGrid = value; return true;
    }

    return false;
}

const VisualChoiceParameter* visualChoiceParameters(size_t& count)
{
    count = sizeof(kChoiceParameters) / sizeof(kChoiceParameters[0]);
    return kChoiceParameters;
}

const VisualChoiceParameter* findVisualChoiceParameter(VisualChoiceParameterId id)
{
    size_t count = 0;
    const VisualChoiceParameter* parameters = visualChoiceParameters(count);
    for (size_t i = 0; i < count; ++i)
    {
        if (parameters[i].id == id)
        {
            return &parameters[i];
        }
    }

    return nullptr;
}

int getVisualChoiceParameter(const VisualParams& params, VisualChoiceParameterId id)
{
    switch (id)
    {
    case VisualChoiceParameterId::TraceMode: return static_cast<int>(params.traceMode);
    case VisualChoiceParameterId::DecayStyle: return static_cast<int>(params.decayStyle);
    }

    return 0;
}

bool setVisualChoiceParameter(VisualParams& params, VisualChoiceParameterId id, int value)
{
    const VisualChoiceParameter* parameter = findVisualChoiceParameter(id);
    if (parameter == nullptr || !choiceContainsValue(*parameter, value))
    {
        return false;
    }

    switch (id)
    {
    case VisualChoiceParameterId::TraceMode:
        params.traceMode = static_cast<TraceMode>(value);
        ++params.clearRevision;
        return true;
    case VisualChoiceParameterId::DecayStyle:
        params.decayStyle = static_cast<DecayStyle>(value);
        ++params.clearRevision;
        return true;
    }

    return false;
}
}
