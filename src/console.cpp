#include <console.h>
#include <iostream>
#include <iomanip>
#include <sstream>

Console::Console() {
}

Console::~Console() {
}

static std::vector<std::string> tokenize(std::string&& input) {
    std::vector<std::string> result;
    std::stringstream stream(std::move(input));
    std::string token;
    while (stream >> token) {
        result.push_back(std::move(token));
    }

    return result;
}

int Console::exec(int /*argc*/, char* /*argv*/[]) {
    std::cout << "## Square equation solver\n## by Vladimir Ogorodnikov, 2018\n## Type \"help\" for more information\n";

    std::string input;
    while (true) {
        std::cout << getVariable("PS1", "> ");
        if (!std::getline(std::cin, input)) {
            break;
        }
        std::vector<std::string> tokens = tokenize(std::move(input));
        if (tokens.empty()) {
            continue;
        }
        if (tokens[0] == "exit") {
            break;
        }
        auto appIter = apps_.find(tokens[0]);
        if (appIter == apps_.end()) {
            std::cout << "# [ERROR] No such app: " << tokens[0] << '\n';
        } else {
            IApp* app = appIter->second;
            int statusCode = app->exec(tokens);
            if (statusCode != IApp::STATUS_OK) {
                std::cout << "# [ERROR] Status code " << statusCode << ": " << app->getStatusCodeDescription(statusCode) << std::endl;
            }
            setVariable("status", std::to_string(statusCode));
        }
    }
    std::cout << "Bye!\n";
    return 0;
}

std::string Console::getVariable(const std::string& name, const std::string& defaultValue) const {
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        return defaultValue;
    } else {
        return it->second;
    }
}

void Console::setVariable(const std::string& name, const std::string& value) {
    variables_[name] = value;
}

const std::map<std::string, IApp*>& Console::getApps() const {
    return apps_;
}

const std::map<std::string, std::string>& Console::getAllVariables() const {
    return variables_;
}
