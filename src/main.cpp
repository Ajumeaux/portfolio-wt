#include <Wt/WServer.h>
#include "PortfolioApp.hpp"

#include <vector>
#include <string>

static bool hasArg(const std::vector<std::string>& args, const std::string& key) {
    for (const auto& a : args) {
        if (a == key) return true;
        if (a.rfind(key + "=", 0) == 0) return true; // --docroot=...
    }
    return false;
}

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc);

    if (!hasArg(args, "--docroot")) {
        args.push_back("--docroot");
        args.push_back("public");
    }

    std::vector<char*> cargs;
    cargs.reserve(args.size());
    for (auto& s : args) cargs.push_back(s.data());

    return Wt::WRun((int)cargs.size(), cargs.data(), &createApplication);
}
