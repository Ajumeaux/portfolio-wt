#include "About.hpp"
#include "JsonUtils.hpp"
#include "LangCache.hpp"

#include <Wt/Json/Object.h>

const AboutContent& getAboutContent(const std::string& lang)
{
    return LangCache::get<AboutContent>(lang, [&]() {
        AboutContent content;

        Wt::Json::Object obj;
        const std::string path = "content/about/" + lang + ".json";
        if (!JsonUtils::loadJsonObjectFromFile(path, obj)) {
            content.title = (lang == "fr") ? "À propos" : "About";
            content.intro = "I enjoy low-level programming and game development.";
            return content;
        }

        content.title = JsonUtils::getHtmlString(obj, "title", (lang == "fr") ? "À propos" : "About");
        content.intro = JsonUtils::getHtmlString(obj, "intro", "");

        content.highlights = JsonUtils::getStringArray(obj, "highlights");
        content.experience = JsonUtils::getStringArray(obj, "experience");
        content.extras     = JsonUtils::getStringArray(obj, "extras");

        if (content.intro.empty()) {
            content.intro = "I enjoy low-level programming and game development.";
        }

        return content;
    });
}
