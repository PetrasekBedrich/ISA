#include <iostream>
#include <string>
#include "Classes/Program.cpp"

int main(int argc, char *argv[])
{
    Program* program = new Program();
    program->main(argc, argv);
    return 0;
}