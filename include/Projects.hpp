#ifndef PROJECTS_HPP
#define PROJECTS_HPP

#include <string>
#include <map>
#include <vector>

struct Project {
    std::string slug;
    std::string title;
    std::string shortDescription;
    std::string contentHtml;
    std::string githubUrl;
    std::vector<std::string> tags;
};

using ProjectsBySlug = std::map<std::string, Project>;

const std::map<std::string, Project>& getAllProjects(const std::string& lang);

const Project* getProjectBySlug(const std::string& lang, const std::string& slug);

#endif // PROJECTS_HPP
