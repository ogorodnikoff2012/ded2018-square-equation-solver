#pragma once

#include <app.h>
#include <console.h>

/**
 * Приложение, позволяющее узнать значение переменных (см. \ref Console)
 * */
class GetterApp : public IApp {
    public:
        /** Аргументы не соответствуют требуемуемому формату */
        static constexpr int STATUS_BAD_ARGUMENTS = 1;
        GetterApp(const Console* parent) : parent_(parent) {}
        virtual int exec(const std::vector<std::string>& args);
        virtual const char* getStatusCodeDescription(int statusCode);
        virtual const char* getHelp();
    private:
        const Console* parent_;
};
