#include "ArgumentParser.cpp"
#include "PacketHandler.cpp"
class Program
{
    public:
        int main(int argc, char *argv[])
        {
            
            parser->Parse(argc,argv,runtimeConfigration);
            packetHandler = new PacketHandler(runtimeConfigration);
            packetHandler->CapturePacketsTest();
            return 0;
        }
    private:
        ArgumentParser* parser = new ArgumentParser();
        RuntimeConfiguration* runtimeConfigration = new RuntimeConfiguration();
        PacketHandler* packetHandler ;

};