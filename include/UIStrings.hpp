#ifndef UI_STRINGS_HPP
#define UI_STRINGS_HPP

#include <string>

struct UiText {
    std::string raw;   // pour WAnchor(label), etc.
    std::string html;  // pour concat dans du XHTML
};

struct UiStrings {
    struct Header {
        UiText name;
        UiText tagline;
    } header;
    
    struct Nav {
        UiText home;
        UiText about;
        UiText projects;
        UiText contact;
    } nav;

    struct Buttons {
        UiText backHome;
        UiText readMore;
        UiText browseProjects;
        UiText contactCta;
        UiText viewOnGithub;
    } buttons;

    struct Titles {
        UiText projectsPage;
        UiText aboutHighlights;
        UiText aboutExperience;
    } titles;

    struct Errors {
        UiText projectNotFound;
    } errors;
};

const UiStrings& getUiStrings(const std::string& lang);

#endif // UI_STRINGS_HPP
