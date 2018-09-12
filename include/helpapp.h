#pragma once

#include <app.h>
#include <console.h>

class HelpApp : public IApp {
    public:
        explicit HelpApp(const Console* parent) : parent_(parent) {}
        ~HelpApp();
        int exec(const std::vector<std::string>& args);
        const char* getStatusCodeDescription(int statusCode);
        const char* getHelp();
    private:
        const Console* parent_;
};
