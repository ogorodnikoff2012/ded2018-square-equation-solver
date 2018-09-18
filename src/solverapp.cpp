#include <solverapp.h>
#include <iostream>
#include <complex>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#define OPT_PTR(type, x) static type default_##x##_var; if ( x == nullptr ) x = &default_##x##_var;

template <class Field>
Field SolverApp::parse(const std::string&, bool*) const {
    return std::declval<Field>();
}

static inline double parseDouble(const char* begin, char** end, bool* ok) {
    double val = std::strtod(begin, end);
    *ok = *end != begin;
    return val;
}

template <>
double SolverApp::parse<double>(const std::string& input, bool* ok) const {
    OPT_PTR(bool, ok);
    char* ptrEnd;
    return parseDouble(input.c_str(), &ptrEnd, ok);
}

template <>
std::complex<double> SolverApp::parse<std::complex<double>>(const std::string& input, bool* ok) const {
    OPT_PTR(bool, ok);
    std::array<double, 2> vals;
    const char* strBegin = input.c_str();
    char* strEnd;

    vals[0] = parseDouble(strBegin, &strEnd, ok);
    if (!*ok) {
        return {0, 0};
    }
    strBegin = strEnd;

    vals[1] = parseDouble(strBegin, &strEnd, ok);
    if (*ok) {
        return {vals[0], vals[1]};
    } else {
        *ok = true;
        if (std::strcmp(strBegin, "j") == 0) {
            return {0, vals[0]};
        } else if (strBegin[0] == '\0') {
            return {vals[0], 0};
        } else {
            *ok = false;
            return {0, 0};
        }
    }
}

static bool isZero(const double& x) {
    return std::abs(x) < std::numeric_limits<double>::epsilon();
}

static bool isZero(const std::complex<double>& x) {
    return isZero(x.real()) && isZero(x.imag());
}

template <class Field>
static bool isValid(const Field&) {
    return true;
}

static bool isValid(double x) {
    return x == x; // check if x is NaN
}

template <class Field>
std::vector<Field> SolverApp::squareRoot(const Field& x) const {
    std::vector<Field> result;
    if (isZero(x)) {
        result.push_back(0);
    } else {
        Field sqrt = std::sqrt(x);
        if (isValid(sqrt)) {
            result.push_back(sqrt);
            result.push_back(-sqrt);
        }
    }
    return result;
}

template <class Field>
std::vector<Field> SolverApp::solveLinear(const Field& k, const Field& b, bool* isDegenerateEquation) const {
    std::vector<Field> result;
    if (isZero(k)) {
        *isDegenerateEquation = isZero(b);
    } else {
        result.push_back(-b / k);
    }
    return result;
}

template <class Field>
std::vector<Field> SolverApp::solveSquare(const std::array<Field, 3>& coefficients, bool* isDegenerateEquation) const {
    if (isZero(coefficients[0])) {
        return solveLinear(coefficients[1], coefficients[2], isDegenerateEquation);
    }
    Field discriminant = coefficients[1] * coefficients[1] - 4. * coefficients[0] * coefficients[2];
    std::vector<Field> result = squareRoot(discriminant);
    for (unsigned int i = 0; i < result.size(); ++i) {
        result[i] -= coefficients[1];
        result[i] /= coefficients[0];
        result[i] /= 2.;
    }

    return result;
}

inline std::ostream& operator <<(std::ostream& out, const std::complex<double>& val) {
    if (isZero(val)) {
        return out << '0';
    }

    if (!isZero(val.real())) {
        out << val.real();
    }

    if (!isZero(val.imag())) {
        if (!isZero(val.real()) && val.imag() > 0) {
            out << '+';
        }
        out << val.imag();
        out << 'j';
    }
    return out;
}

template <class Field>
int SolverApp::parseSolveAndPrint(const std::vector<std::string>& input) const {
    std::array<Field, 3> coefficients;
    for (int i = 0; i < 3; ++i) {
        bool ok = true;
        coefficients[i] = parse<Field>(input[1 + i], &ok);
        if (!ok) {
            return STATUS_PARSE_ERROR;
        }
    }

    bool isDegenerateEquation = false;
    auto solution = solveSquare(coefficients, &isDegenerateEquation);

    if (isDegenerateEquation) {
        parent_->info() << "Equation is degenerate: every value is its solution\n";
    } else {
        parent_->info() << "Equation has " << solution.size() << " solution" << (solution.size() == 1 ? "" : "s") << ":\n";
        bool first = true;
        for (auto iter = solution.begin(); iter != solution.end(); ++iter) {
            if (!first) {
                parent_->output() << ' ';
            }
            first = false;
            parent_->output() << *iter;
        }
        parent_->output() << std::endl;
    }
    return STATUS_OK;
}

int SolverApp::exec(const std::vector<std::string>& args) {
    if (args.size() != 4) {
        return STATUS_BAD_ARGUMENTS;
    }

    std::string fieldCode = parent_->getVariable("field", "R");
    if (fieldCode == "R") {
        return parseSolveAndPrint<double>(args);
    } else if (fieldCode == "C") {
        return parseSolveAndPrint<std::complex<double>>(args);
    }

    return STATUS_BAD_FIELD;
}

const char* SolverApp::getStatusCodeDescription(int statusCode) {
    switch (statusCode) {
        case STATUS_OK:
            return "OK";
        case STATUS_BAD_ARGUMENTS:
            return "Number of arguments should be exactly 3";
        case STATUS_BAD_FIELD:
            return "'field' value is invalid";
        case STATUS_PARSE_ERROR:
            return "Error while parsing coefficients";
        default:
            return "Invalid status code";
    }
}

const char* SolverApp::getHelp() {
    return  "Usage: solve a b c\n"
            "Finds all solutions for equation a*x^2 + b*x + c = 0\n"
            "You can select a field where the coefficients are from.\n"
            "To do it, you should set variable \"field\" to one of the \n"
            "following values (type 'set field <one-of-these-values>'):\n"
            " R - Real numbers ('double' in C++)\n"
            " C - Complex numbers ('std::complex<double>' in C++; it's just a pair of doubles)\n"
            "If variable \"field\" is not set, real numbers are used by default.";
}
