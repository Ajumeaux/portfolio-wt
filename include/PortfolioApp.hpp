#ifndef PORTFOLIO_APP_HPP
#define PORTFOLIO_APP_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include <Wt/WApplication.h>
#include <Wt/WComboBox.h>

namespace Wt {
    class WEnvironment;
    class WContainerWidget;
}

class PortfolioApp : public Wt::WApplication {
public:
    explicit PortfolioApp(const Wt::WEnvironment& env);

private:
    Wt::WAnchor* navHome_ = nullptr;
    Wt::WAnchor* navAbout_ = nullptr;
    Wt::WAnchor* navProjects_ = nullptr;
    Wt::WAnchor* navContact_ = nullptr;
    Wt::WComboBox* langSelect_ = nullptr;

    void updateActiveNav(const std::string& cleanInternalPath);

    std::string lang_;
    Wt::WContainerWidget* content_;

    static inline const std::unordered_set<std::string> kSupportedLangs = {
        "en", "fr"
    };

    static bool isSupportedLang(const std::string& lang) {
        return kSupportedLangs.find(lang) != kSupportedLangs.end();
    }

    static std::string normalizeLang(std::string lang);

    static std::string detectLang(const Wt::WEnvironment& env);

    Wt::WLink makeInternalLink(const std::string& internalPath) const;

    void setupHeader();
    void showHome();
    void showProjects();
    void showProject(const std::string& slug);
    void showAbout();
    void showContact();
    void handleInternalPath(const std::string& path);
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env);

#endif // PORTFOLIO_APP_HPP
