#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <string>
#include <vector>

struct AboutContent {
    std::string title;
    std::string intro;
    std::vector<std::string> highlights;
    std::vector<std::string> experience;
    std::vector<std::string> extras;
};

const AboutContent& getAboutContent(const std::string& lang);

#endif // ABOUT_HPP
