#include "Projects.hpp"
#include "JsonUtils.hpp"
#include "LangCache.hpp"

#include <Wt/Json/Object.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Value.h>

namespace {

static ProjectsBySlug loadProjectsFromJson(const std::string& lang)
{
    ProjectsBySlug target;

    Wt::Json::Object root;
    std::string error;

    const std::string path = "content/projects/" + lang + ".json";
    if (!JsonUtils::loadJsonObjectFromFile(path, root, &error)) {
        return target;
    }

    auto it = root.find("projects");
    if (it == root.end() || it->second.type() != Wt::Json::Type::Array) {
        return target;
    }

    Wt::Json::Array projectsArray = it->second;

    for (const Wt::Json::Value& v : projectsArray) {
        if (v.type() != Wt::Json::Type::Object) continue;

        Wt::Json::Object obj = v;

        const std::string slug = JsonUtils::getString(obj, "slug", "");
        if (slug.empty()) continue;

        Project p;
        p.slug = slug;

        p.githubUrl = JsonUtils::getString(obj, "githubUrl", "");
        p.tags      = JsonUtils::getStringArray(obj, "tags");

        auto textIt = obj.find("text");
        if (textIt != obj.end() && textIt->second.type() == Wt::Json::Type::Object) {
            Wt::Json::Object textObj = textIt->second;

            p.title            = JsonUtils::getHtmlString(textObj, "title", "");
            p.shortDescription = JsonUtils::getHtmlString(textObj, "shortDescription", "");

            auto paragraphs = JsonUtils::getHtmlStringArray(textObj, "paragraphs");
            auto bullets    = JsonUtils::getHtmlStringArray(textObj, "bullets");

            std::string html;
            for (const auto& s : paragraphs) {
                html += "<p>" + s + "</p>";
            }
            if (!bullets.empty()) {
                html += "<ul>";
                for (const auto& b : bullets) html += "<li>" + b + "</li>";
                html += "</ul>";
            }
            p.contentHtml = std::move(html);
        }

        target.emplace(p.slug, std::move(p));
    }

    return target;
}

} // namespace

const ProjectsBySlug& getAllProjects(const std::string& lang)
{
    return LangCache::get<ProjectsBySlug>(lang, [&]() {
        return loadProjectsFromJson(lang);
    });
}

const Project* getProjectBySlug(const std::string& lang, const std::string& slug)
{
    const auto& projects = getAllProjects(lang);
    auto it = projects.find(slug);
    if (it == projects.end()) return nullptr;
    return &it->second;
}
