#include <testing.h>
#include <iostream>

class ANSIMod {
    private:
        const char* unmod_;
    public:
        ANSIMod(const char* mod, const char* unmod = "\e[0m") :
            unmod_(unmod) {
            std::cerr << mod;
        }
        ~ANSIMod() {
            std::cerr << unmod_;
        }
};

#define YELLOW  "\e[33;1m"
#define GREEN   "\e[32;1m"
#define RED     "\e[31;1m"
#define BLUE    "\e[34;1m"

void Tester::registerTest(const Test& test) {
    tests_.push_back(test);
}

void Tester::runTests() {
    {
        ANSIMod m(YELLOW);
        std::cerr << "Test set " << getName() << std::endl;
        std::cerr << "Running tests" << std::endl;
    }

    int failed = 0;
    int succeded = 0;

    for (const auto& test : tests_) {
        {
            ANSIMod m(BLUE);
            std::cerr << "Running test " << test.name << std::endl;
        }
        bool ok = test.test();
        if (ok) {
            ANSIMod m(GREEN);
            std::cerr << "TEST OK" << std::endl;
            ++succeded;
        } else {
            ANSIMod m(RED);
            std::cerr << "TEST FAILED" << std::endl;
            ++failed;
        }
    }
    {
        ANSIMod m(YELLOW);
        std::cerr << "========== Done! ==========" << std::endl;
    }
    std::cerr << "Total: " << (failed + succeded) << ", Succeeded: " << succeded << ", Failed: " << failed << std::endl;
}
