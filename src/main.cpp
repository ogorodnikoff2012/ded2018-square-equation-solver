#include <console.h>
#include <helpapp.h>
#include <setterapp.h>
#include <getterapp.h>
#include <solverapp.h>

#include <cstring>
#include <fstream>
#include <iostream>

static const char* HELP_TEXT =
"Square Equation Solver by Vladimir Ogorodnikov, 2018\n"
"Usage: %s [-h] [-o filename]  [-q] [-i | filename] [args...]\n"
"   -o filename -- write output to file 'filename' instead of stdout\n"
"   -q          -- quiet mode (set 'verbosity' variable to 'ERROR')\n"
"   -i          -- interactive mode (use it to prevent treating first argument as filename)\n"
"   -h          -- print this help\n"
"All other arguments are passed as variables 'arg1', 'arg2', ... and so on. Number of arguments is stored in 'nargs'.\n";

int main(int argc, char* argv[]) {
    int currentArg = 1;
    bool quiet = false;
    bool interactive = false;
    const char* outFName = nullptr;
    const char* inFName = nullptr;

    while (currentArg < argc) {
        if (std::strcmp(argv[currentArg], "-o") == 0) {
            ++currentArg;
            if (currentArg >= argc) {
                std::cerr << "No output file specified" << std::endl;
                return 1;
            }
            outFName = argv[currentArg];
        } else if (std::strcmp(argv[currentArg], "-q") == 0) {
            quiet = true;
        } else if (std::strcmp(argv[currentArg], "-i") == 0) {
            interactive = true;
        } else if (std::strcmp(argv[currentArg], "-h") == 0) {
            std::printf(HELP_TEXT, argv[0]);
            return 0;
        } else {
            break;
        }
        ++currentArg;
    }

    if (currentArg < argc && !interactive) {
        inFName = argv[currentArg];
        ++currentArg;
    }

    std::istream* in  = &std::cin;
    std::ostream* out = &std::cout;

    if (inFName != nullptr) {
        in = new std::ifstream(inFName);
    }

    if (outFName != nullptr) {
        out = new std::ofstream(outFName);
    }

    bool ioOK = true;

    if (!*in) {
        ioOK = false;
        std::cerr << "Bad file: " << inFName << std::endl;
    }

    if (!*out) {
        ioOK = false;
        std::cerr << "Bad file: " << outFName << std::endl;

    }

    if (!ioOK) {
        delete in;
        delete out;
        return 1;
    }

    Console console(*in, *out);
    if (quiet) {
        console.setVariable("verbosity", "ERROR");
    }
    console.emplaceApp<HelpApp>("help");
    console.emplaceApp<SetterApp>("set");
    console.emplaceApp<GetterApp>("get");
    console.emplaceApp<SolverApp>("solve");
    console.addAlias("?", "help");
    return console.exec(argc - currentArg - 1, argv + currentArg);
}
