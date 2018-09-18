#pragma once

#include <app.h>
#include <console.h>

class SolverApp : public IApp {
    public:
        static constexpr int STATUS_BAD_ARGUMENTS = 1;
        static constexpr int STATUS_BAD_FIELD = 2;
        static constexpr int STATUS_PARSE_ERROR = 3;
        SolverApp(const Console* parent) : parent_(parent) {}
        virtual int exec(const std::vector<std::string>& args);
        virtual const char* getStatusCodeDescription(int statusCode);
        virtual const char* getHelp();
    private:
        template <class Field>
        Field parse(const std::string& input, bool* ok = nullptr) const;

        template <class Field>
        std::vector<Field> solveSquare(const std::array<Field, 3>& coefficients, bool* isDegenerateEquation) const;

        template <class Field>
        std::vector<Field> solveLinear(const Field& k, const Field& b, bool* isDegenerateEquation) const;

        template <class Field>
        std::vector<Field> squareRoot(const Field& x) const;

        template <class Field>
        int parseSolveAndPrint(const std::vector<std::string>& input) const;
        const Console* parent_;
};
