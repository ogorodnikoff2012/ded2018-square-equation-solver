#include <testing.h>
#include <iostream>

TEST_SET(SimpleTestSet) {
    TEST(HelloWorld) {
        std::cout << "Hello world!" << std::endl;
        return true;
    };
}

int main() {
    test_autogen::SimpleTestSet().runTests();
    return 0;
}
