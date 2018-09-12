#include <helpapp.h>
#include <iostream>

HelpApp::~HelpApp() {
}

int HelpApp::exec(const std::vector<std::string>& args) {
    const auto& apps = parent_->getApps();
    if (args.size() == 1) {
        std::cout << "List of available commands:\n";
        for (const auto& app : apps) {
            std::cout << app.first << std::endl;
        }
        std::cout <<    "To get help about a specific command, type: help <command-name>\n"
                        "To exit from this app, type: exit\n";
    } else {
        for (auto iter = args.begin() + 1; iter != args.end(); ++iter) {
            std::cout << "=== " << *iter << " ===\n";
            auto appIter = apps.find(*iter);
            if (appIter == apps.end()) {
                std::cout << "No such command!\n";
            } else {
                std::cout << appIter->second->getHelp() << std::endl;
            }
        }
    }

    return 0;
}

const char* HelpApp::getStatusCodeDescription(int statusCode) {
    return (statusCode == 0) ? "OK" : "Invalid status code";
}

const char* HelpApp::getHelp() {
    return  "Usage: help [<list-of-commands>]\n"
            "If no arguments given, prints list of avaliable commands.\n"
            "Otherwise prints description for each named command.";
}
