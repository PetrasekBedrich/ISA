#include <pcap.h>
#include <iostream>
#include <cstring>
class PacketHandler
{
    public:
        PacketHandler(RuntimeConfiguration* runtimeConfiguration)
        {
            this->runtimeConfiguration = runtimeConfiguration;
        }
        void CapturePacketsTest()
        {
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_if_t *alldevs;  
            pcap_if_t *device = nullptr;
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                fprintf(stderr, "ERROR: Unable to find devices");
            }
            while (alldevs->next)
            {
                if(strcmp(alldevs->name,runtimeConfiguration->Interface) == 0)
                {
                    device = alldevs;
                }
                alldevs = alldevs->next;
            }
            if(device == nullptr)
            {
                fprintf(stderr,"ERROR: Interface could not be found");
                exit(1);
            }
        }
    private:
        RuntimeConfiguration* runtimeConfiguration;
};