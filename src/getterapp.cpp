#include <getterapp.h>
#include <iostream>

int GetterApp::exec(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        return STATUS_BAD_ARGUMENTS;
    }
    bool needToPrintName = args.size() > 2;
    for (auto iter = args.begin() + 1; iter != args.end(); ++iter) {
        if (needToPrintName) {
            std::cout << *iter << ": ";
        }
        std::cout << parent_->getVariable(*iter, "") << std::endl;
    }
    return STATUS_OK;
}

const char* GetterApp::getStatusCodeDescription(int statusCode) {
    switch (statusCode) {
        case STATUS_OK:
            return "OK";
        case STATUS_BAD_ARGUMENTS:
            return "No arguments given, need at least 1";
        default:
            return "Invalid status code";
    }
}

const char* GetterApp::getHelp() {
    return  "Usage: get <list-of-variables>\n"
            "If 1 argument is given, prints only value of this variable.\n"
            "Otherwise, prints pairs \"name: value\".";
}
