#pragma once

#include <string>
#include <map>
#include <type_traits>
#include "app.h"

class Console {
    public:
        Console();
        ~Console();
        
        template<class App, class... Args>
        bool emplaceApp(const std::string& appName, Args&& ...args) {
            if (apps_.find(appName) != apps_.end()) {
                return false;
            }
            apps_.emplace(std::make_pair(appName, new App(this, std::forward<Args>(args)...)));
            return true;
        }

        int exec(int argc, char* argv[]);
        std::string getVariable(const std::string& name, const std::string& defaultValue = "") const;
        void setVariable(const std::string& name, const std::string& value);

    private:
        std::map<std::string, IApp*> apps_;
        std::map<std::string, std::string> variables_;
};
