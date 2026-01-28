#include "JsonUtils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <Wt/Json/Parser.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Value.h>
#include <Wt/Utils.h>

namespace JsonUtils {

std::string loadFileToString(const std::string& path)
{
    std::ifstream f(path);
    if (!f) {
        std::cerr << "[JsonUtils] Failed to open file: " << path << "\n";
        return {};
    }

    std::ostringstream oss;
    oss << f.rdbuf();

    std::string result = oss.str();
    std::cerr << "[JsonUtils] Read " << result.size()
              << " bytes from file: " << path << "\n";
    return result;
}

bool loadJsonFromPaths(const std::initializer_list<std::string>& paths,
                       Wt::Json::Value& outValue)
{
    std::string jsonText;

    std::cerr << "[JsonUtils] loadJsonFromPaths: trying paths:\n";
    for (const auto& path : paths) {
        std::cerr << "  - " << path << "\n";
        jsonText = loadFileToString(path);
        if (!jsonText.empty()) {
            std::cerr << "[JsonUtils] Successfully loaded JSON text from: "
                      << path << "\n";
            break;
        }
    }

    if (jsonText.empty()) {
        std::cerr << "[JsonUtils] No JSON file could be loaded from given paths.\n";
        return false;
    }

    std::string error;
    // Dans ta version de Wt, parse(...) retourne void et remplit 'error'.
    Wt::Json::parse(jsonText, outValue, &error);

    if (!error.empty()) {
        std::cerr << "[JsonUtils] JSON parse error: " << error << "\n";
        return false;
    }

    std::cerr << "[JsonUtils] JSON parsed successfully.\n";
    return true;
}

bool loadJsonObjectFromFile(const std::string& path,
                            Wt::Json::Object& out,
                            std::string* errorMessage)
{
    std::ifstream file(path);
    if (!file) {
        if (errorMessage) {
            *errorMessage = "Cannot open file: " + path;
        }
        std::cerr << "[JsonUtils] loadJsonObjectFromFile: cannot open file: "
                  << path << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    std::cerr << "[JsonUtils] loadJsonObjectFromFile: read "
              << text.size() << " bytes from " << path << "\n";

    std::string errors;
    // idem ici : parse(...) retourne void
    Wt::Json::parse(text, out, &errors);
    if (!errors.empty()) {
        if (errorMessage) {
            *errorMessage = errors;
        }
        std::cerr << "[JsonUtils] JSON parse error in "
                  << path << ": " << errors << "\n";
        return false;
    }

    std::cerr << "[JsonUtils] JSON object parsed successfully from "
              << path << "\n";
    return true;
}

std::string getString(const Wt::Json::Object& obj,
                      const std::string& key,
                      const std::string& defaultValue)
{
    auto it = obj.find(key);
    if (it == obj.end()) {
        std::cerr << "[JsonUtils] getString: key '" << key
                  << "' not found, using default.\n";
        return defaultValue;
    }
    if (it->second.type() != Wt::Json::Type::String) {
        std::cerr << "[JsonUtils] getString: key '" << key
                  << "' is not a string, using default.\n";
        return defaultValue;
    }

    std::string value = it->second.toString();
    std::cerr << "[JsonUtils] getString: key '" << key
              << "' = '" << value << "'\n";
    return value;
}

std::vector<std::string> getStringArray(const Wt::Json::Object& obj,
                                        const std::string& key)
{
    std::vector<std::string> result;

    auto it = obj.find(key);
    if (it == obj.end()) {
        std::cerr << "[JsonUtils] getStringArray: key '" << key
                  << "' not found, returning empty vector.\n";
        return result;
    }
    if (it->second.type() != Wt::Json::Type::Array) {
        std::cerr << "[JsonUtils] getStringArray: key '" << key
                  << "' is not an array, returning empty vector.\n";
        return result;
    }

    Wt::Json::Array arr = it->second; // Value -> Array
    std::cerr << "[JsonUtils] getStringArray: key '" << key
              << "' array size = " << arr.size() << "\n";

    for (const auto& v : arr) {
        if (v.type() == Wt::Json::Type::String) {
            std::string s = v.toString();
            result.push_back(s);
        } else {
            std::cerr << "[JsonUtils] getStringArray: non-string element ignored.\n";
        }
    }

    return result;
}

std::string getHtmlString(const Wt::Json::Object& obj,
                          const std::string& key,
                          const std::string& defaultValue)
{
    return Wt::Utils::htmlEncode(getString(obj, key, defaultValue));
}

std::vector<std::string> getHtmlStringArray(const Wt::Json::Object& obj,
                                            const std::string& key)
{
    std::vector<std::string> raw = getStringArray(obj, key);
    for (auto& s : raw) {
        s = Wt::Utils::htmlEncode(s);
    }
    return raw;
}


} // namespace JsonUtils
