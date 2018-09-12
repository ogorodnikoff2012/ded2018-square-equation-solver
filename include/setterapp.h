#pragma once

#include <app.h>
#include <console.h>

class SetterApp : public IApp {
    public:
        static constexpr int STATUS_BAD_ARGUMENTS = 1;
        explicit SetterApp(Console* parent) : parent_(parent) {}
        virtual int exec(const std::vector<std::string>& args);
        virtual const char* getStatusCodeDescription(int statusCode);
        virtual const char* getHelp();
    private:
        Console* parent_;
};
