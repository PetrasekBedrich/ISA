#include "ArgumentParser.cpp"
#include "PacketHandler.cpp"
#include "TrafickHandler.cpp"
class Program
{
    public:
        int main(int argc, char *argv[])
        {
            
            parser->Parse(argc,argv,runtimeConfigration);
            packetHandler = new PacketHandler(runtimeConfigration);
            packetHandler->CapturePackets();
            TrafickHandler handler(runtimeConfigration);
            return handler.run();
        }
    private:
        ArgumentParser* parser = new ArgumentParser();
        RuntimeConfiguration* runtimeConfigration = new RuntimeConfiguration();
        PacketHandler* packetHandler ;

};