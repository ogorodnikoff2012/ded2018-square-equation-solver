#include <solverapp.h>
#include <iostream>
#include <complex>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cstring>

template <class Field>
Field SolverApp::parse(const std::string&) const {
    throw std::runtime_error("Appropriate parser not found");
}

template <>
double SolverApp::parse<double>(const std::string& input) const {
    return std::stod(input);
}

template <>
std::complex<double> SolverApp::parse<std::complex<double>>(const std::string& input) const {
    std::array<double, 2> vals;
    const char* strBegin = input.c_str();
    char* strEnd;

    for (int i = 0; i < 2; ++i) {
        vals[i] = std::strtod(strBegin, &strEnd);
        if (errno == ERANGE || strEnd == strBegin) {
            throw std::invalid_argument("Parse error");
        } else {
            strBegin = strEnd;
        }
    }

    if (std::strcmp(strBegin, "i")) {
        throw std::invalid_argument("Parse error");
    }
    return std::complex<double>(vals[0], vals[1]);
}

template <class Field>
bool SolverApp::isZero(const Field&) const {
    throw std::runtime_error("Cannot compare to zero");
}

template <>
bool SolverApp::isZero(const double& x) const {
    return std::abs(x) < std::numeric_limits<double>::epsilon();
}

template <>
bool SolverApp::isZero(const std::complex<double>& x) const {
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

template <class Field>
int SolverApp::parseSolveAndPrint(const std::vector<std::string>& input) const {
    std::array<Field, 3> coefficients;
    try {
        for (int i = 0; i < 3; ++i) {
            coefficients[i] = parse<Field>(input[1 + i]);
        }
    } catch (std::invalid_argument& exp) {
        return STATUS_PARSE_ERROR;
    }

    bool isDegenerateEquation = false;
    auto solution = solveSquare(coefficients, &isDegenerateEquation);

    if (isDegenerateEquation) {
        std::cout << "Equation is degenerate: every value is its solution\n";
    } else {
        std::cout << "Equation has " << solution.size() << " solutions:\n";
        for (auto iter = solution.begin(); iter != solution.end(); ++iter) {
            std::cout << *iter << ' ';
        }
        std::cout << std::endl;
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
