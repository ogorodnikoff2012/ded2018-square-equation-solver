#pragma once

#include <vector>
#include <string>

class IApp {
    public:
        static constexpr int STATUS_OK = 0;
        virtual ~IApp() {};
        virtual int exec(const std::vector<std::string>& args) = 0;
        virtual const char* getStatusCodeDescription(int statusCode) = 0;
        virtual const char* getHelp() = 0;
};
