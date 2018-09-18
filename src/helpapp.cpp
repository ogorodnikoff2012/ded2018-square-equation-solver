#include <helpapp.h>
#include <iostream>

HelpApp::~HelpApp() {
}

int HelpApp::exec(const std::vector<std::string>& args) {
    const auto& apps = parent_->getApps();
    if (args.size() == 1) {
        parent_->output() << "List of available commands:\n";
        for (const auto& app : apps) {
            parent_->output() << app.first << std::endl;
        }
        parent_->output() << "To get help about a specific command, type: help <command-name>\n"
                             "To exit from this app, type: exit\n";
    } else {
        for (auto iter = args.begin() + 1; iter != args.end(); ++iter) {
            parent_->output() << "=== " << *iter << " ===\n";
            auto appIter = apps.find(*iter);
            if (appIter == apps.end()) {
                parent_->output() << "No such command!\n";
            } else {
                parent_->output() << appIter->second->getHelp() << std::endl;
            }
        }
    }

    return STATUS_OK;
}

const char* HelpApp::getStatusCodeDescription(int statusCode) {
    return (statusCode == STATUS_OK) ? "OK" : "Invalid status code";
}

const char* HelpApp::getHelp() {
    return  "Usage: help [<list-of-commands>]\n"
            "If no arguments given, prints list of avaliable commands.\n"
            "Otherwise prints description for each named command.";
}
