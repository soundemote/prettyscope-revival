#include "app/preset_store.hpp"

#include "visual/visual_param_limits.hpp"
#include "visual/visual_parameters.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

namespace prettyscope
{
namespace
{
std::filesystem::path presetPath()
{
    return std::filesystem::current_path() / "presets" / "default.prettyscope.json";
}

const char* traceModeName(TraceMode mode)
{
    return mode == TraceMode::Xy ? "xy" : "time";
}

const char* decayStyleName(DecayStyle style)
{
    return style == DecayStyle::Phosphor ? "phosphor" : "classic";
}

const char* signalModeName(TestSignalGenerator::Mode mode)
{
    switch (mode)
    {
    case TestSignalGenerator::Mode::Lissajousish:
        return "fold";
    case TestSignalGenerator::Mode::BassPulse:
        return "bass";
    case TestSignalGenerator::Mode::Musical:
    default:
        return "musical";
    }
}

bool readFile(std::string& out)
{
    std::ifstream file(presetPath());
    if (!file)
    {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
    return true;
}

std::string regexEscape(const char* text)
{
    std::string escaped;
    for (const char* cursor = text; cursor != nullptr && *cursor != '\0'; ++cursor)
    {
        switch (*cursor)
        {
        case '.':
        case '\\':
        case '+':
        case '*':
        case '?':
        case '[':
        case '^':
        case ']':
        case '$':
        case '(':
        case ')':
        case '{':
        case '}':
        case '=':
        case '!':
        case '<':
        case '>':
        case '|':
        case ':':
        case '-':
            escaped.push_back('\\');
            break;
        default:
            break;
        }
        escaped.push_back(*cursor);
    }
    return escaped;
}

bool findString(const std::string& text, const char* key, std::string& value)
{
    const std::regex pattern(std::string("\"") + regexEscape(key) + "\"\\s*:\\s*\"([^\"]*)\"");
    std::smatch match;
    if (!std::regex_search(text, match, pattern))
    {
        return false;
    }

    value = match[1].str();
    return true;
}

bool findString(const std::string& text, const char* key, const char* legacyKey, std::string& value)
{
    return findString(text, key, value) || (legacyKey != nullptr && findString(text, legacyKey, value));
}

bool findFloat(const std::string& text, const char* key, float& value)
{
    const std::regex pattern(std::string("\"") + regexEscape(key) + "\"\\s*:\\s*(-?[0-9]+(?:\\.[0-9]+)?)");
    std::smatch match;
    if (!std::regex_search(text, match, pattern))
    {
        return false;
    }

    value = std::stof(match[1].str());
    return true;
}

bool findFloat(const std::string& text, const char* key, const char* legacyKey, float& value)
{
    return findFloat(text, key, value) || (legacyKey != nullptr && findFloat(text, legacyKey, value));
}

bool findBool(const std::string& text, const char* key, bool& value)
{
    const std::regex pattern(std::string("\"") + regexEscape(key) + "\"\\s*:\\s*(true|false)");
    std::smatch match;
    if (!std::regex_search(text, match, pattern))
    {
        return false;
    }

    value = match[1].str() == "true";
    return true;
}

bool findBool(const std::string& text, const char* key, const char* legacyKey, bool& value)
{
    return findBool(text, key, value) || (legacyKey != nullptr && findBool(text, legacyKey, value));
}

const char* stableId(VisualFloatParameterId id)
{
    const VisualFloatParameter* parameter = findVisualFloatParameter(id);
    return parameter != nullptr ? parameter->stableId : "";
}

const char* stableId(VisualBoolParameterId id)
{
    const VisualBoolParameter* parameter = findVisualBoolParameter(id);
    return parameter != nullptr ? parameter->stableId : "";
}

const char* stableId(VisualChoiceParameterId id)
{
    const VisualChoiceParameter* parameter = findVisualChoiceParameter(id);
    return parameter != nullptr ? parameter->stableId : "";
}

bool findColor(const std::string& text, const char* key, RgbColor& color)
{
    const std::regex pattern(std::string("\"") + regexEscape(key) + "\"\\s*:\\s*\\[\\s*(-?[0-9]+(?:\\.[0-9]+)?)\\s*,\\s*(-?[0-9]+(?:\\.[0-9]+)?)\\s*,\\s*(-?[0-9]+(?:\\.[0-9]+)?)\\s*\\]");
    std::smatch match;
    if (!std::regex_search(text, match, pattern))
    {
        return false;
    }

    color = {std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str())};
    return true;
}
}

bool PresetStore::saveDefault(const VisualParams& params, const TestSignalGenerator& generator)
{
    std::filesystem::create_directories(presetPath().parent_path());

    std::ofstream file(presetPath());
    if (!file)
    {
        return false;
    }

    file << "{\n";
    file << "  \"stateVersion\": " << kVisualParameterStateVersion << ",\n";
    file << "  \"" << stableId(VisualChoiceParameterId::TraceMode) << "\": \"" << traceModeName(params.traceMode) << "\",\n";
    file << "  \"signalMode\": \"" << signalModeName(generator.mode()) << "\",\n";
    file << "  \"" << stableId(VisualChoiceParameterId::DecayStyle) << "\": \"" << decayStyleName(params.decayStyle) << "\",\n";
    file << "  \"" << stableId(VisualFloatParameterId::TraceGain) << "\": " << params.traceGain << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::GlowStrength) << "\": " << params.glowStrength << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::TraceWidth) << "\": " << params.traceWidth << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::GlowWidth) << "\": " << params.glowWidth << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::Persistence) << "\": " << params.persistence << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::FastDecay) << "\": " << params.fastDecay << ",\n";
    file << "  \"" << stableId(VisualFloatParameterId::Afterglow) << "\": " << params.afterglow << ",\n";
    file << "  \"" << stableId(VisualBoolParameterId::PersistenceEnabled) << "\": " << (params.persistenceEnabled ? "true" : "false") << ",\n";
    file << "  \"" << stableId(VisualBoolParameterId::ShowFps) << "\": " << (params.showFps ? "true" : "false") << ",\n";
    file << "  \"" << stableId(VisualBoolParameterId::ShowGrid) << "\": " << (params.showGrid ? "true" : "false") << ",\n";
    file << "  \"traceColor\": [" << params.traceColor.r << ", " << params.traceColor.g << ", " << params.traceColor.b << "],\n";
    file << "  \"glowColor\": [" << params.glowColor.r << ", " << params.glowColor.g << ", " << params.glowColor.b << "],\n";
    file << "  \"backgroundColor\": [" << params.backgroundColor.r << ", " << params.backgroundColor.g << ", " << params.backgroundColor.b << "]\n";
    file << "}\n";
    return true;
}

bool PresetStore::loadDefault(VisualParams& params, TestSignalGenerator& generator)
{
    std::string text;
    if (!readFile(text))
    {
        return false;
    }

    std::string value;
    if (findString(text, stableId(VisualChoiceParameterId::TraceMode), "traceMode", value))
    {
        params.traceMode = value == "xy" ? TraceMode::Xy : TraceMode::Time;
    }
    if (findString(text, "signalMode", value))
    {
        if (value == "fold")
        {
            generator.setMode(TestSignalGenerator::Mode::Lissajousish);
        }
        else if (value == "bass")
        {
            generator.setMode(TestSignalGenerator::Mode::BassPulse);
        }
        else
        {
            generator.setMode(TestSignalGenerator::Mode::Musical);
        }
    }
    if (findString(text, stableId(VisualChoiceParameterId::DecayStyle), "decayStyle", value))
    {
        params.decayStyle = value == "phosphor" ? DecayStyle::Phosphor : DecayStyle::Classic;
    }

    findFloat(text, stableId(VisualFloatParameterId::TraceGain), "traceGain", params.traceGain);
    findFloat(text, stableId(VisualFloatParameterId::GlowStrength), "glowStrength", params.glowStrength);
    findFloat(text, stableId(VisualFloatParameterId::TraceWidth), "traceWidth", params.traceWidth);
    findFloat(text, stableId(VisualFloatParameterId::GlowWidth), "glowWidth", params.glowWidth);
    findFloat(text, stableId(VisualFloatParameterId::Persistence), "persistence", params.persistence);
    findFloat(text, stableId(VisualFloatParameterId::FastDecay), "fastDecay", params.fastDecay);
    findFloat(text, stableId(VisualFloatParameterId::Afterglow), "afterglow", params.afterglow);
    findBool(text, stableId(VisualBoolParameterId::PersistenceEnabled), "persistenceEnabled", params.persistenceEnabled);
    findBool(text, stableId(VisualBoolParameterId::ShowFps), nullptr, params.showFps);
    findBool(text, stableId(VisualBoolParameterId::ShowGrid), "showGrid", params.showGrid);
    findColor(text, "traceColor", params.traceColor);
    findColor(text, "glowColor", params.glowColor);
    findColor(text, "backgroundColor", params.backgroundColor);

    clampVisualParams(params);
    ++params.clearRevision;
    return true;
}
}
