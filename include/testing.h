#pragma once

#define TEST_SET(test_set_name) \
namespace test_autogen { \
    class test_set_name : public Tester { \
        public: \
            virtual const char* getName() const { return #test_set_name ; } \
            test_set_name(); \
    }; \
} \
test_autogen::test_set_name::test_set_name()

#define TEST(test_name) TestInserter{this, #test_name } << []() -> bool

#include <vector>
#include <functional>

class Tester {
    public:
        struct Test {
            std::function<bool()> test;
            const char* name;
        };
        struct TestInserter {
            Tester* tester;
            const char* name;
            void operator <<(std::function<bool()> func) {
                tester->registerTest({func, name});
            }
        };
    private:
        std::vector<Test> tests_;
    public:
        virtual const char* getName() const { return ""; }
        void registerTest(const Test& test);
        void runTests();
};
