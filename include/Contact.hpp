#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>

struct ContactContent {
    std::string title;
    std::string text;
    std::string githubUrl;
};

const ContactContent& getContactContent(const std::string& lang);

#endif // CONTACT_HPP
