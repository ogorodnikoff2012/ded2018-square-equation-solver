#include <console.h>
#include <helpapp.h>
#include <setterapp.h>
#include <getterapp.h>
#include <solverapp.h>

int main(int argc, char* argv[]) {
    Console console;
    console.emplaceApp<HelpApp>("help");
    console.emplaceApp<SetterApp>("set");
    console.emplaceApp<GetterApp>("get");
    console.emplaceApp<SolverApp>("solve");
    return console.exec(argc, argv);
}
