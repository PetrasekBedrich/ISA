#include "ArgumentParser.cpp"
class Program
{
    public:
        int main(int argc, char *argv[])
        {
            runtimeConfigration = new RuntimeConfiguration();
            parser = new ArgumentParser();
            parser->Parse(argc,argv,runtimeConfigration);
            return 0;
        }
    private:
        ArgumentParser* parser;
        RuntimeConfiguration* runtimeConfigration;

};