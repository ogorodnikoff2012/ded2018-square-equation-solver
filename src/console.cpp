#include <console.h>
#include <iostream>
#include <iomanip>
#include <sstream>

class NullOutputStream : public std::ostream {
    public:
        NullOutputStream() { }
        template <class T>
        NullOutputStream& operator <<(T&&) {
            return *this;
        }
};

static NullOutputStream devNull;

Console::~Console() {
}

std::ostream& Console::output() const {
    return out_;
}

std::ostream& Console::log(Verbosity verbosity, const char* prompt) const {
    if (verbosity >= getVerbosity()) {
        return out_ << prompt;
    } else {
        return devNull;
    }
}

std::ostream& Console::debug() const {
    return log(VERB_DEBUG, "# [DEBUG] ");
}

std::ostream& Console::info() const {
    return log(VERB_INFO, "# [INFO ] ");
}


std::ostream& Console::error() const {
    return log(VERB_ERROR, "# [ERROR] ");
}

std::istream& Console::input() const {
    return in_;
}


Verbosity Console::getVerbosity() const {
    std::string str = getVariable("verbosity", "DEBUG");
    if (str == "ERROR") {
        return VERB_ERROR;
    } else if (str == "INFO") {
        return VERB_INFO;
    }
    return VERB_DEBUG;
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

int Console::exec(int argc, char* argv[]) {
    info() << "Square equation solver\n";
    info() << "by Vladimir Ogorodnikov, 2018\n";
    info() << "Type \"help\" for more information\n";

    setVariable("nargs", std::to_string(argc));

    for (int i = 0; i < argc; ++i) {
        std::string name = "arg";
        name += (i + 1);
        setVariable(name, argv[i]);
    }

    std::string input;
    while (true) {
        log(VERB_INFO, getVariable("PS1", "> ").c_str());
        if (!std::getline(this->input(), input)) {
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
            error() << "No such app: " << tokens[0] << '\n';
        } else {
            IApp* app = appIter->second;
            int statusCode = app->exec(tokens);
            if (statusCode != IApp::STATUS_OK) {
                error() << "Status code " << statusCode << ": " << app->getStatusCodeDescription(statusCode) << std::endl;
            }
            setVariable("status", std::to_string(statusCode));
        }
    }
    info() << "Bye!\n";
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
