#include "UIStrings.hpp"
#include "JsonUtils.hpp"
#include "LangCache.hpp"

#include <Wt/Json/Object.h>
#include <Wt/Json/Value.h>

namespace {

static UiText readUiText(const Wt::Json::Object& obj,
                         const std::string& key,
                         const std::string& fallbackRaw)
{
    UiText t;
    t.raw  = JsonUtils::getString(obj, key, fallbackRaw);
    t.html = JsonUtils::getHtmlString(obj, key, fallbackRaw);
    return t;
}

static Wt::Json::Object getObjectOrEmpty(const Wt::Json::Object& parent,
                                        const std::string& key)
{
    auto it = parent.find(key);
    if (it != parent.end() && it->second.type() == Wt::Json::Type::Object) {
        return it->second;
    }
    return Wt::Json::Object{};
}

static UiStrings makeDefaults()
{
    UiStrings ui;

    ui.header.name    = UiText{"Antoine Jumeaux", "Antoine Jumeaux"};
    ui.header.tagline = UiText{"Software engineer – C++ / Systems / Games",
                            "Software engineer – C++ / Systems / Games"};

    ui.nav.home     = UiText{"Home", "Home"};
    ui.nav.about    = UiText{"About", "About"};
    ui.nav.projects = UiText{"Projects", "Projects"};
    ui.nav.contact  = UiText{"Contact", "Contact"};

    ui.buttons.backHome       = UiText{"← Back to home", "← Back to home"};
    ui.buttons.readMore       = UiText{"Read more →", "Read more →"};
    ui.buttons.browseProjects = UiText{"Browse all projects →", "Browse all projects →"};
    ui.buttons.contactCta     = UiText{"Contact →", "Contact →"};
    ui.buttons.viewOnGithub   = UiText{"View on GitHub", "View on GitHub"};

    ui.titles.projectsPage = UiText{"Projects", "Projects"};
    ui.titles.aboutHighlights = UiText{"Highlights", "Highlights"};
    ui.titles.aboutExperience = UiText{"Experience", "Experience"};

    ui.errors.projectNotFound = UiText{"Project not found.", "Project not found."};

    return ui;
}

static UiStrings loadUiFromJson(const std::string& lang)
{
    UiStrings ui = makeDefaults();

    Wt::Json::Object root;
    std::string error;
    const std::string path = "content/ui/" + lang + ".json";

    if (!JsonUtils::loadJsonObjectFromFile(path, root, &error)) {
        return ui; // fallback defaults
    }

    // header
    {
        Wt::Json::Object hObj = getObjectOrEmpty(root, "header");
        ui.header.name    = readUiText(hObj, "name", ui.header.name.raw);
        ui.header.tagline = readUiText(hObj, "tagline", ui.header.tagline.raw);
    }

    // nav
    {
        Wt::Json::Object navObj = getObjectOrEmpty(root, "nav");
        ui.nav.home     = readUiText(navObj, "home", ui.nav.home.raw);
        ui.nav.about    = readUiText(navObj, "about", ui.nav.about.raw);
        ui.nav.projects = readUiText(navObj, "projects", ui.nav.projects.raw);
        ui.nav.contact  = readUiText(navObj, "contact", ui.nav.contact.raw);
    }

    // buttons
    {
        Wt::Json::Object btnObj = getObjectOrEmpty(root, "buttons");
        ui.buttons.backHome       = readUiText(btnObj, "backHome", ui.buttons.backHome.raw);
        ui.buttons.readMore       = readUiText(btnObj, "readMore", ui.buttons.readMore.raw);
        ui.buttons.browseProjects = readUiText(btnObj, "browseProjects", ui.buttons.browseProjects.raw);
        ui.buttons.contactCta     = readUiText(btnObj, "contactCta", ui.buttons.contactCta.raw);
        ui.buttons.viewOnGithub   = readUiText(btnObj, "viewOnGithub", ui.buttons.viewOnGithub.raw);
    }

    // titles
    {
        Wt::Json::Object titlesObj = getObjectOrEmpty(root, "titles");
        ui.titles.projectsPage = readUiText(titlesObj, "projectsPage", ui.titles.projectsPage.raw);
        ui.titles.aboutHighlights = readUiText(titlesObj, "aboutHighlights", ui.titles.aboutHighlights.raw);
        ui.titles.aboutExperience = readUiText(titlesObj, "aboutExperience", ui.titles.aboutExperience.raw);
    }

    // errors
    {
        Wt::Json::Object errObj = getObjectOrEmpty(root, "errors");
        ui.errors.projectNotFound = readUiText(errObj, "projectNotFound", ui.errors.projectNotFound.raw);
    }

    return ui;
}

} // namespace

const UiStrings& getUiStrings(const std::string& lang)
{
    return LangCache::get<UiStrings>(lang, [&]() {
        return loadUiFromJson(lang);
    });
}
