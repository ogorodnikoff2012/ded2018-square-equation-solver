#include <console.h>
#include <iostream>
#include <iomanip>

Console::Console() {
}

Console::~Console() {
}

int Console::exec(int argc, char* argv[]) {
    std::cout << "## Square equation solver\n## by Vladimir Ogorodnikov, 2018\n";
    std::cout << "# [DEBUG] Arguments: \n";
    for (int i = 1; i < argc; ++i) {
        std::cout << "# [DEBUG] " << i << ": " << std::quoted(argv[i]) << '\n';
    }



    bool exitRequested = false;
    std::string input;
    while (!exitRequested) {
        std::cout << getVariable("PS1", "> ");
        std::getline(std::cin, input);
        exitRequested = (input == "exit");
        std::cout << "Input: " << input << std::endl;
    }
    std::cout << "Bye!\n";
    return 0;
}

std::string Console::getVariable(const std::string& name, const std::string& defaultValue) const {
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        return defaultValue;
    } else {
        return it->second;
    }
}

void Console::setVariable(const std::string& name, const std::string& value) {
    variables_[name] = value;
}
