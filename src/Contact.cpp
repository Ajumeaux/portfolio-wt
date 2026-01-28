#include "Contact.hpp"
#include "JsonUtils.hpp"
#include "LangCache.hpp"

#include <Wt/Json/Object.h>
#include <iostream>

const ContactContent& getContactContent(const std::string& lang)
{
    return LangCache::get<ContactContent>(lang, [&]() {
        ContactContent contact;

        Wt::Json::Object obj;
        const std::string path = "content/contact/" + lang + ".json";
        if (!JsonUtils::loadJsonObjectFromFile(path, obj)) {
            std::cerr << "[Contact] Could not load " << path << "\n";
            contact.title     = "Contact";
            contact.text      = "You can find more details and code on my GitHub:";
            contact.githubUrl = "https://github.com/ajumeaux";
            return contact;
        }

        contact.title = JsonUtils::getHtmlString(obj, "title", "Contact");
        contact.text  = JsonUtils::getHtmlString(obj, "text",
            "You can find more details and code on my GitHub:");
        contact.githubUrl = JsonUtils::getHtmlString(obj, "githubUrl",
            "https://github.com/ajumeaux");

        return contact;
    });
}

