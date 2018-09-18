#pragma once

#include <string>
#include <map>
#include <type_traits>
#include "app.h"

enum Verbosity {
    VERB_DEBUG,
    VERB_INFO,
    VERB_ERROR
};

class Console {
    public:
        Console(std::istream& in, std::ostream& out) : out_(out), in_(in) {}
        ~Console();

        template<class App, class... Args>
        bool emplaceApp(const std::string& appName, Args&& ...args) {
            if (apps_.find(appName) != apps_.end()) {
                return false;
            }
            apps_.emplace(std::make_pair(appName, new App(this, std::forward<Args>(args)...)));
            return true;
        }

        std::ostream& output() const;
        std::ostream& log(Verbosity verbosity, const char* prompt = "") const;
        std::ostream& debug() const;
        std::ostream& info() const;
        std::ostream& error() const;
        std::istream& input() const;
        Verbosity getVerbosity() const;
        int exec(int argc, char* argv[]);
        std::string getVariable(const std::string& name, const std::string& defaultValue = "") const;
        void setVariable(const std::string& name, const std::string& value);
        const std::map<std::string, IApp*>& getApps() const;
        const std::map<std::string, std::string>& getAllVariables() const;

    private:
        std::ostream& out_;
        std::istream& in_;
        std::map<std::string, IApp*> apps_;
        std::map<std::string, std::string> variables_;
};
