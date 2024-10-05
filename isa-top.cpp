#include <iostream>
#include <string>
class Program
{
    public:
        int main(int argc, char *argv[])
        {
            printf("Hello, world!\n");
            return 0;
        }

};
int main(int argc, char *argv[])
{
    Program* program = new Program();
    program->main(argc, argv);
}