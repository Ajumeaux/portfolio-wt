#ifndef PROJECTPAGE_HPP
#define PROJECTPAGE_HPP

#include <string>
#include <Wt/WContainerWidget.h>
#include "Projects.hpp"

class ProjectPage : public Wt::WContainerWidget {
public:
    ProjectPage(const Project& project, const std::string& lang);
};

#endif // PROJECTPAGE_HPP
