#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <string>
#include <vector>
#include <initializer_list>

#include <Wt/Json/Value.h>
#include <Wt/Json/Object.h>

namespace JsonUtils {

std::string loadFileToString(const std::string& path);

bool loadJsonFromPaths(const std::initializer_list<std::string>& paths,
                       Wt::Json::Value& outValue);

bool loadJsonObjectFromFile(const std::string& path,
                            Wt::Json::Object& out,
                            std::string* errorMessage = nullptr);

std::string getString(const Wt::Json::Object& obj,
                      const std::string& key,
                      const std::string& defaultValue = std::string());

std::vector<std::string> getStringArray(const Wt::Json::Object& obj,
                                        const std::string& key);

std::string getHtmlString(const Wt::Json::Object& obj,
                          const std::string& key,
                          const std::string& defaultValue = std::string());

std::vector<std::string> getHtmlStringArray(const Wt::Json::Object& obj,
                                            const std::string& key);
} // namespace JsonUtils

#endif // JSONUTILS_HPP
