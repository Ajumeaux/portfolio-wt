#include "PortfolioApp.hpp"
#include "Projects.hpp"
#include "ProjectPage.hpp"
#include "About.hpp"
#include "Contact.hpp"
#include "UIStrings.hpp"
#include "TypewriterText.hpp"

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WLink.h>
#include <Wt/WEnvironment.h>
#include <Wt/WString.h>
#include <Wt/Utils.h>
#include <Wt/WComboBox.h>
#include <Wt/WSignal.h>

#include <algorithm>
#include <cctype>

PortfolioApp::PortfolioApp(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
    , lang_(detectLang(env))
    , content_(nullptr)
{
    setTitle("Antoine Jumeaux – Portfolio");

    doJavaScript(
        "var l = document.createElement('link');"
        "l.rel = 'icon';"
        "l.type = 'image/x-icon';"
        "l.href = 'favicon.ico?v=2';"
        "document.head.appendChild(l);"
    );

    useStyleSheet(Wt::WLink("css/app.css"));

    root()->addStyleClass("app-root");

    setupHeader();

    content_ = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    content_->addStyleClass("content");

    internalPathChanged().connect(this, &PortfolioApp::handleInternalPath);
    handleInternalPath(internalPath());
}

std::string PortfolioApp::normalizeLang(std::string lang)
{
    std::transform(lang.begin(), lang.end(), lang.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    auto pos = lang.find_first_of("_-");
    if (pos != std::string::npos) {
        lang = lang.substr(0, pos);
    }
    return lang;
}

std::string PortfolioApp::detectLang(const Wt::WEnvironment& env)
{
    //?lang=xx
    auto it = env.getParameterMap().find("lang");
    if (it != env.getParameterMap().end() && !it->second.empty()) {
        std::string v = normalizeLang(it->second[0]);
        if (isSupportedLang(v)) return v;
    }

    std::string loc = normalizeLang(env.locale().name()); 
    if (isSupportedLang(loc)) return loc;

    return "en";
}

static std::string addOrReplaceQueryParam(std::string url,
                                         const std::string& key,
                                         const std::string& value)
{
    std::string frag;
    if (auto h = url.find('#'); h != std::string::npos) {
        frag = url.substr(h);
        url  = url.substr(0, h);
    }

    const std::string k = key + "=";
    const std::string encV = Wt::Utils::urlEncode(value);

    auto q = url.find('?');
    if (q == std::string::npos) {
        url += "?" + k + encV;
        return url + frag;
    }

    std::string base  = url.substr(0, q);
    std::string query = url.substr(q + 1);

    auto pos = query.find(k);
    if (pos != std::string::npos) {
        auto end = query.find('&', pos);
        std::string before = query.substr(0, pos);
        std::string after  = (end == std::string::npos) ? "" : query.substr(end);
        query = before + k + encV + after;
    } else {
        if (!query.empty()) query += "&";
        query += k + encV;
    }

    return base + "?" + query + frag;
}

void PortfolioApp::updateActiveNav(const std::string& cleanPath)
{
    auto setActive = [&](Wt::WAnchor* a, bool active) {
        if (!a) return;
        if (active) a->addStyleClass("is-active");
        else        a->removeStyleClass("is-active");
    };

    setActive(navHome_,     cleanPath == "/");
    setActive(navAbout_,    cleanPath == "/about");
    setActive(navProjects_, cleanPath == "/projects" || cleanPath.rfind("/projects/", 0) == 0);
    setActive(navContact_,  cleanPath == "/contact");
}


static std::string stripQuery(std::string p)
{
    auto q = p.find('?');
    if (q != std::string::npos) p = p.substr(0, q);
    return p;
}

static std::string getQueryParam(const std::string& path, const std::string& key)
{
    auto q = path.find('?');
    if (q == std::string::npos) return "";

    std::string query = path.substr(q + 1); // "lang=fr&x=y"
    size_t pos = 0;

    while (pos < query.size()) {
        size_t amp = query.find('&', pos);
        std::string part = query.substr(pos, (amp == std::string::npos) ? std::string::npos : amp - pos);

        size_t eq = part.find('=');
        std::string k = (eq == std::string::npos) ? part : part.substr(0, eq);
        std::string v = (eq == std::string::npos) ? ""   : part.substr(eq + 1);

        if (k == key) return v;

        if (amp == std::string::npos) break;
        pos = amp + 1;
    }
    return "";
}

Wt::WLink PortfolioApp::makeInternalLink(const std::string& internalPath) const
{
    std::string clean = stripQuery(internalPath);
    std::string url = bookmarkUrl(clean);
    url = addOrReplaceQueryParam(url, "lang", lang_);
    return Wt::WLink(Wt::LinkType::Url, url);
}

void PortfolioApp::setupHeader()
{
    const auto& ui = getUiStrings(lang_);
    auto header = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    header->addStyleClass("header");

    auto h1 = header->addWidget(std::make_unique<Wt::WContainerWidget>());
    h1->setHtmlTagName("h1");
    h1->addStyleClass("title");

    TypewriterText::Options o;
    o.speedMs = 64;
    o.delayMs = 100;
    o.cursor = true;
    o.randomize = true;

    h1->addWidget(std::make_unique<TypewriterText>(ui.header.name.raw, o));

    header->addWidget(std::make_unique<Wt::WText>(
        "<p>" + ui.header.tagline.html + "</p>",
        Wt::TextFormat::XHTML
    ));

    auto nav = header->addWidget(std::make_unique<Wt::WContainerWidget>());
    nav->setHtmlTagName("nav");

    auto addSep = [&](){
        auto sep = nav->addWidget(std::make_unique<Wt::WText>("", Wt::TextFormat::Plain));
        sep->addStyleClass("nav-sep");
    };

    navHome_ = nav->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/"), ui.nav.home.raw));
    navHome_->addStyleClass("nav-link");
    addSep();

    navAbout_ = nav->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/about"), ui.nav.about.raw));
    navAbout_->addStyleClass("nav-link");
    addSep();

    navProjects_ = nav->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/projects"), ui.nav.projects.raw));
    navProjects_->addStyleClass("nav-link");
    addSep();

    navContact_ = nav->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/contact"), ui.nav.contact.raw));
    navContact_->addStyleClass("nav-link");

    addSep();

    auto langLabel = nav->addWidget(std::make_unique<Wt::WText>("LANG", Wt::TextFormat::Plain));
    langLabel->addStyleClass("nav-label");

    langSelect_ = nav->addWidget(std::make_unique<Wt::WComboBox>());
    langSelect_->addStyleClass("lang-select");

    langSelect_->addItem("FR");
    langSelect_->addItem("EN");
    langSelect_->setCurrentIndex(lang_ == "fr" ? 0 : 1);

    langSelect_->changed().connect([this] {
        const std::string newLang = (langSelect_->currentIndex() == 0) ? "fr" : "en";
        std::string clean = stripQuery(internalPath());
        std::string url = bookmarkUrl(clean);
        url = addOrReplaceQueryParam(url, "lang", newLang);
        redirect(url);
    });




    header->addWidget(std::make_unique<Wt::WText>("<hr />", Wt::TextFormat::XHTML));

}


void PortfolioApp::handleInternalPath(const std::string& path)
{
    const std::string clean = stripQuery(path);
    updateActiveNav(clean);

    if (clean.rfind("/projects/", 0) == 0) {
        std::string slug = clean.substr(std::string("/projects/").size());
        showProject(slug);
    } else if (clean == "/about") {
        showAbout();
    } else if (clean == "/contact") {
        showContact();
    } else if (clean == "/projects") {
        showProjects();
    } else {
        showHome();
    }
}

static void appendList(std::ostringstream& out, const std::vector<std::string>& items, int maxItems = -1)
{
    if (items.empty()) return;
    out << "<ul>";
    int count = 0;
    for (const auto& it : items) {
        if (maxItems >= 0 && count++ >= maxItems) break;
        out << "<li>" << Wt::Utils::htmlEncode(it) << "</li>";
    }
    out << "</ul>";
}

void PortfolioApp::showHome()
{
    content_->clear();

    const auto& ui = getUiStrings(lang_);

    // About card
    const auto& about = getAboutContent(lang_);

    auto aboutCard = content_->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/about")));
    aboutCard->addStyleClass("card-link");

    std::ostringstream aboutPreview;
    aboutPreview << "<div class=\"card panel panel--clickable\">";
    aboutPreview << "<h2 class=\"card-title\">" << about.title << "</h2>";
    aboutPreview << "<div class=\"clamp-5\">";

    aboutPreview << "<p>" << about.intro << "</p>";
    if (!about.highlights.empty()) {
        aboutPreview << "<h3 class=\"card-subtitle\">" << ui.titles.aboutHighlights.html << "</h3>";
        appendList(aboutPreview, about.highlights, 3);
    }

    aboutPreview << "</div>";
    aboutPreview << "<div class=\"more\">" << ui.buttons.readMore.html << "</div>";
    aboutPreview << "</div>";

    aboutCard->addWidget(std::make_unique<Wt::WText>(aboutPreview.str(), Wt::TextFormat::XHTML));

    // Projects card (preview)
    auto projectsCard = content_->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/projects")));
    projectsCard->addStyleClass("card-link");

    std::ostringstream projPreview;
    projPreview << "<div class=\"card panel panel--clickable\">";
    projPreview << "<h2 class=\"card-title\">" << ui.nav.projects.html << "</h2>";
    projPreview << "<div class=\"clamp-5\">";

    const auto& projects = getAllProjects(lang_);
    int count = 0;
    for (const auto& pair : projects) {
        if (count++ >= 4) break;
        projPreview << "<p>▶ " << pair.second.title << "</p>";
    }

    projPreview << "</div>";
    projPreview << "<div class=\"more\">" << ui.buttons.browseProjects.html << "</div>";
    projPreview << "</div>";

    projectsCard->addWidget(std::make_unique<Wt::WText>(projPreview.str(), Wt::TextFormat::XHTML));

    // Contact card
    const auto& contact = getContactContent(lang_);

    auto contactCard = content_->addWidget(std::make_unique<Wt::WAnchor>(makeInternalLink("/contact")));
    contactCard->addStyleClass("card-link");

    std::ostringstream contactPreview;
    contactPreview << "<div class=\"card panel panel--clickable\">";
    contactPreview << "<h2 class=\"card-title\">" << contact.title << "</h2>";
    contactPreview << "<div class=\"clamp-3\">";
    contactPreview << "<p>" << contact.text << "</p>";
    contactPreview << "</div>";
    contactPreview << "<div class=\"more\">" << ui.buttons.contactCta.html << "</div>";
    contactPreview << "</div>";

    contactCard->addWidget(std::make_unique<Wt::WText>(contactPreview.str(), Wt::TextFormat::XHTML));
}

void PortfolioApp::showProject(const std::string& slug)
{
    content_->clear();

    const auto& ui = getUiStrings(lang_);

    const Project* project = getProjectBySlug(lang_, slug);
    if (!project) {
        content_->addWidget(std::make_unique<Wt::WText>(
            "<p>" + ui.errors.projectNotFound.html + "</p>",
            Wt::TextFormat::XHTML
        ));

        content_->addWidget(std::make_unique<Wt::WAnchor>(
            makeInternalLink("/"),
            ui.buttons.backHome.raw
        ));
        return;
    }

    content_->addWidget(std::make_unique<ProjectPage>(*project, lang_));
}

void PortfolioApp::showProjects()
{
    content_->clear();

    const auto& ui = getUiStrings(lang_);

    auto page = content_->addWidget(std::make_unique<Wt::WContainerWidget>());
    page->addStyleClass("page");
    page->addStyleClass("panel");
    page->addStyleClass("panel--page");

    page->addWidget(std::make_unique<Wt::WText>(
        "<h1>" + ui.titles.projectsPage.html + "</h1>",
        Wt::TextFormat::XHTML
    ));

    auto listPanel = content_->addWidget(std::make_unique<Wt::WContainerWidget>());
    listPanel->addStyleClass("projects-section");
    listPanel->addStyleClass("panel");

    auto projectsList = listPanel->addWidget(std::make_unique<Wt::WContainerWidget>());
    projectsList->addStyleClass("projects-list");

    const auto& projects = getAllProjects(lang_);
    for (const auto& pair : projects) {
        const Project& p = pair.second;

        auto link = projectsList->addWidget(std::make_unique<Wt::WAnchor>(
            makeInternalLink("/projects/" + p.slug)
        ));
        link->addStyleClass("project-item");

        auto titleText = link->addWidget(std::make_unique<Wt::WText>(p.title, Wt::TextFormat::XHTML));
        titleText->addStyleClass("project-title");

        if (!p.shortDescription.empty()) {
            auto descText = link->addWidget(std::make_unique<Wt::WText>(
                " – " + p.shortDescription,
                Wt::TextFormat::XHTML
            ));
            descText->addStyleClass("project-short");
        }
    }

    auto back = content_->addWidget(std::make_unique<Wt::WAnchor>(
        makeInternalLink("/"),
        ui.buttons.backHome.raw
    ));
    back->addStyleClass("btn");
    back->addStyleClass("btn--back");

}

void PortfolioApp::showAbout()
{
    content_->clear();

    const auto& ui = getUiStrings(lang_);
    const auto& about = getAboutContent(lang_);

    std::ostringstream html;
    html << "<section class=\"page panel panel--page\">";
    html << "<h1>" << about.title << "</h1>";

    html << "<p>" << about.intro << "</p>";

    if (!about.highlights.empty()) {
        html << "<h2>" << ui.titles.aboutHighlights.html << "</h2>";
        appendList(html, about.highlights);
    }

    if (!about.experience.empty()) {
        html << "<h2>" << ui.titles.aboutExperience.html << "</h2>";
        appendList(html, about.experience);
    }

    for (const auto& p : about.extras) {
        html << "<p>" << p << "</p>";
    }

    html << "</section>";


    content_->addWidget(std::make_unique<Wt::WText>(html.str(), Wt::TextFormat::XHTML));

    auto back = content_->addWidget(std::make_unique<Wt::WAnchor>(
        makeInternalLink("/"),
        ui.buttons.backHome.raw
    ));
    back->addStyleClass("btn");
    back->addStyleClass("btn--back");

}

void PortfolioApp::showContact()
{
    content_->clear();

    const auto& ui = getUiStrings(lang_);
    const auto& contact = getContactContent(lang_);

    std::ostringstream html;
    html << "<section class=\"page panel panel--page\">";
    html << "<h1>" << contact.title << "</h1>";
    html << "<p>" << contact.text << "</p>";

    if (!contact.githubUrl.empty()) {
        std::string label = contact.githubUrl;
        if (label.rfind("https://", 0) == 0) label = label.substr(8);
        else if (label.rfind("http://", 0) == 0) label = label.substr(7);

        html << "<p><a href=\""
             << Wt::Utils::htmlAttributeValue(contact.githubUrl)
             << "\" target=\"_blank\" rel=\"noopener\">"
             << Wt::Utils::htmlEncode(label)
             << "</a></p>";
    }

    html << "</section>";

    content_->addWidget(std::make_unique<Wt::WText>(html.str(), Wt::TextFormat::XHTML));

    auto back = content_->addWidget(std::make_unique<Wt::WAnchor>(
        makeInternalLink("/"),
        ui.buttons.backHome.raw
    ));
    back->addStyleClass("btn");
    back->addStyleClass("btn--back");

}

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env)
{
    return std::make_unique<PortfolioApp>(env);
}
