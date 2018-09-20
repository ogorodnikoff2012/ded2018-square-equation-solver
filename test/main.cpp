#include <testing.h>
#include <iostream>
#include <solverapp.h>
#include <sstream>
#include <set>

TEST_SET(SimpleTestSet) {
    TEST(HelloWorld) {
        std::cout << "Hello world!" << std::endl;
        return true;
    };
}

TEST_SET(SolverAppSet) {
    TEST(SimpleTest) {
        std::stringstream data;
        Console console(std::cin, data);
        console.setVariable("verbosity", "ERROR");
        SolverApp app(&console);
        app.exec({"solve", "1", "0", "-1"});

        std::cerr << "Stream: " << data.str() << std::endl;

        std::set<double> answer;
        double buffer;
        while (data >> buffer) {
            answer.insert(buffer);
        }
        return answer == std::set<double>{-1, 1};
    };
}

int main() {
    test_autogen::SimpleTestSet().runTests();
    test_autogen::SolverAppSet().runTests();
    return 0;
}
