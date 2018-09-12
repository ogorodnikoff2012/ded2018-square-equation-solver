#include <setterapp.h>

int SetterApp::exec(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        return STATUS_BAD_ARGUMENTS;
    }

    parent_->setVariable(args[1], args[2]);
    return STATUS_OK;
}

const char* SetterApp::getStatusCodeDescription(int statusCode) {
    switch (statusCode) {
        case STATUS_OK:
            return "OK";
        case STATUS_BAD_ARGUMENTS:
            return "Number of arguments is incorrect, should be exactly 2";
        default:
            return "Invalid status code";
    }
}

const char* SetterApp::getHelp() {
    return "Usage: set <name> <value>";
}
