#include <solverapp.h>
#include <iostream>
#include <complex>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <mod_arith.h>

static bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

template <class Field, class... Args>
Field SolverApp::parse(const std::string&, Args...) const {
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

template <>
Residue SolverApp::parse<Residue>(const std::string& input, const int modulo) const {
    return Residue(std::stoul(input), modulo);
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

template <>
bool SolverApp::isZero(const Residue& x) const {
    return x == 0;
}

template <class Field>
std::vector<Field> SolverApp::squareRoot(const Field&) const {
    throw std::runtime_error("Cannot take square root");
}

template <>
std::vector<double> SolverApp::squareRoot(const double& x) const {
    std::vector<double> result;
    if (isZero(x)) {
        result.push_back(0);
    } else if (x > 0) {
        double sqrt = std::sqrt(x);
        result.push_back(sqrt);
        result.push_back(-sqrt);
    }
    return result;
}

template <>
std::vector<std::complex<double>> SolverApp::squareRoot(const std::complex<double>& x) const {
    std::vector<std::complex<double>> result;
    if (isZero(x)) {
        result.push_back(0);
    } else {
        std::complex<double> sqrt = std::sqrt(x);
        result.push_back(sqrt);
        result.push_back(-sqrt);
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
std::vector<Field> SolverApp::solve(const std::array<Field, 3>& coefficients, bool* isDegenerateEquation) const {
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

template <class Field, class... Args>
int SolverApp::parseSolveAndPrint(const std::vector<std::string>& input, Args... args) const {
    std::array<Field, 3> coefficients;
    try {
        for (int i = 0; i < 3; ++i) {
            coefficients[i] = parse<Field>(input[1 + i], args...);
        }
    } catch (std::invalid_argument& exp) {
        return STATUS_PARSE_ERROR;
    }

    bool isDegenerateEquation = false;
    auto solution = solve(coefficients, &isDegenerateEquation);

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
    } else if (fieldCode[0] == 'Z' && fieldCode[1] == '_') {
        int modulo;
        try {
            modulo = std::stoi(fieldCode.substr(2));
            if (!isPrime(modulo)) {
                throw std::invalid_argument("is not prime!");
            }
            return parseSolveAndPrint<Residue>(args, modulo);
        } catch (std::invalid_argument& exp) {
            return STATUS_BAD_FIELD;
        } catch (std::out_of_range& exp) {
            return STATUS_BAD_FIELD;
        }
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
            " Z_p (e.g. Z_2, Z_17, etc) - ring of integers modulo p (Z/pZ).\n"
            "Notice: p should be prime integer; otherwise, Z_p wouldn't be a field.\n"
            "If variable \"field\" is not set, real numbers are used by default.";
}
