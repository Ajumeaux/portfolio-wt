#include "ProjectPage.hpp"
#include "UIStrings.hpp"

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WLink.h>

ProjectPage::ProjectPage(const Project& project, const std::string& lang)
{
    const auto& ui = getUiStrings(lang);

    addStyleClass("project-page");
    addStyleClass("panel");
    addStyleClass("panel--page");

    addWidget(std::make_unique<Wt::WText>(
        "<h1>" + project.title + "</h1>",
        Wt::TextFormat::XHTML
    ));

    if (!project.shortDescription.empty()) {
        addWidget(std::make_unique<Wt::WText>(
            "<p class=\"subtitle\">" + project.shortDescription + "</p>",
            Wt::TextFormat::XHTML
        ));
    }

    if (!project.contentHtml.empty()) {
        addWidget(std::make_unique<Wt::WText>(
            project.contentHtml,
            Wt::TextFormat::XHTML
        ));
    }

    if (!project.githubUrl.empty()) {
        auto git = addWidget(std::make_unique<Wt::WAnchor>(
            Wt::WLink(project.githubUrl),
            ui.buttons.viewOnGithub.raw
        ));
        git->addStyleClass("project-github-link");

        // compat Wt (pas de setTarget chez toi)
        git->setAttributeValue("target", "_blank");
        git->setAttributeValue("rel", "noopener noreferrer");
    }
}
