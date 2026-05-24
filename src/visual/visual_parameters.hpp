#pragma once

#include "visual/visual_params.hpp"

#include <cstddef>
#include <string>

namespace prettyscope
{
enum class VisualFloatParameterId
{
    TraceGain,
    GlowStrength,
    TraceWidth,
    GlowWidth,
    Persistence,
    FastDecay,
    Afterglow,
    GridIntensity,
};

enum class VisualBoolParameterId
{
    PersistenceEnabled,
    ShowFps,
    ShowGrid,
};

enum class VisualChoiceParameterId
{
    TraceMode,
    DecayStyle,
};

struct VisualFloatParameter
{
    VisualFloatParameterId id;
    const char* key;
    const char* label;
    float minimum;
    float maximum;
    float defaultValue;
};

struct VisualBoolParameter
{
    VisualBoolParameterId id;
    const char* key;
    const char* label;
    bool defaultValue;
};

struct VisualChoiceOption
{
    int value;
    const char* key;
    const char* label;
};

struct VisualChoiceParameter
{
    VisualChoiceParameterId id;
    const char* key;
    const char* label;
    const VisualChoiceOption* options;
    size_t optionCount;
    int defaultValue;
};

const VisualFloatParameter* visualFloatParameters(size_t& count);
const VisualFloatParameter* findVisualFloatParameter(VisualFloatParameterId id);
float getVisualFloatParameter(const VisualParams& params, VisualFloatParameterId id);
bool setVisualFloatParameter(VisualParams& params, VisualFloatParameterId id, float value);
float normalizeVisualFloatParameter(const VisualFloatParameter& parameter, float value);
float denormalizeVisualFloatParameter(const VisualFloatParameter& parameter, float normalizedValue);
float getNormalizedVisualFloatParameter(const VisualParams& params, VisualFloatParameterId id);
bool setNormalizedVisualFloatParameter(VisualParams& params, VisualFloatParameterId id, float normalizedValue);
std::string formatVisualFloatParameterValue(VisualFloatParameterId id, float value);

const VisualBoolParameter* visualBoolParameters(size_t& count);
const VisualBoolParameter* findVisualBoolParameter(VisualBoolParameterId id);
bool getVisualBoolParameter(const VisualParams& params, VisualBoolParameterId id);
bool setVisualBoolParameter(VisualParams& params, VisualBoolParameterId id, bool value);

const VisualChoiceParameter* visualChoiceParameters(size_t& count);
const VisualChoiceParameter* findVisualChoiceParameter(VisualChoiceParameterId id);
int getVisualChoiceParameter(const VisualParams& params, VisualChoiceParameterId id);
bool setVisualChoiceParameter(VisualParams& params, VisualChoiceParameterId id, int value);
}
