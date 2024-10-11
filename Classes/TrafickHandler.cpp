#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include "RuntimeConfiguration.cpp"
class TrafickHandler {
private:
    struct traffic_stat {
        char src_ip[INET_ADDRSTRLEN];
        char dst_ip[INET_ADDRSTRLEN];
        uint64_t bytes;
    };

    static const int MAX_STATS = 100;
    RuntimeConfiguration* configuration;  // Uchováváme název rozhraní
    traffic_stat stats[MAX_STATS];
    int stats_count = 0;
    int keep_running = 1;
    pcap_t *handle;


public:
    TrafickHandler(RuntimeConfiguration* config) : configuration(config), stats_count(0), keep_running(1), handle(nullptr) {}

    static TrafickHandler *instance;

    static void handle_sigint_static(int sig) {
        if (instance) {
            instance->handle_sigint(sig);
        }
    }

    static void packet_handler_static(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
        if (instance) {
            instance->packet_handler(user_data, pkthdr, packet);
        }
    }

    void handle_sigint(int sig) {
        keep_running = 0;
    }

    void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
        struct ether_header *eth_header = (struct ether_header *) packet;

        if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
            return;
        }

        struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);

        uint32_t packet_size = pkthdr->len;
        update_stats(src_ip, dst_ip, packet_size);
    }

    void update_stats(char *src_ip, char *dst_ip, uint32_t packet_size) {
        for (int i = 0; i < stats_count; i++) {
            if (strcmp(stats[i].src_ip, src_ip) == 0 && strcmp(stats[i].dst_ip, dst_ip) == 0) {
                stats[i].bytes += packet_size;
                return;
            }
        }

        if (stats_count < MAX_STATS) {
            strcpy(stats[stats_count].src_ip, src_ip);
            strcpy(stats[stats_count].dst_ip, dst_ip);
            stats[stats_count].bytes = packet_size;
            stats_count++;
        }
    }

    void display_stats() {
        printf("\033[H\033[J");
        printf("IP Source         -> IP Destination       | Bytes Transferred\n");
        printf("-------------------------------------------------------------\n");

        for (int i = 0; i < stats_count; i++) {
            printf("%-16s -> %-16s | %lu bytes\n", stats[i].src_ip, stats[i].dst_ip, stats[i].bytes);
        }
    }

    // Tato metoda byla přejmenována z 'main' na 'run'
    int run() 
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        struct bpf_program fp;
        char filter_exp[] = "ip";
        bpf_u_int32 net = 0;  // Declare as regular variable, not a pointer
        bpf_u_int32 mask = 0; // Variable to store the network mask

        instance = this;

        signal(SIGINT, handle_sigint_static);  // Set up signal handler for Ctrl+C

        // Get the network and mask using pcap_lookupnet
        if (pcap_lookupnet(configuration->Interface, &net, &mask, errbuf) == -1) {
            fprintf(stderr, "Nemohu získat netmask pro zařízení: %s\n", errbuf);
            net = 0;  // Default to zero if lookup fails
        }

        // Open the device for capturing packets using the interface from configuration
        handle = pcap_open_live(configuration->Interface, BUFSIZ, 1, 1000, errbuf);
        if (handle == nullptr) {
            fprintf(stderr, "Nemohu otevřít zařízení: %s\n", errbuf);
            return 2;
        }

        // Compile the filter expression into a BPF program
        if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
            fprintf(stderr, "Nemohu kompilovat filtr: %s\n", pcap_geterr(handle));
            return 2;
        }

        // Set the compiled filter to the pcap handle
        if (pcap_setfilter(handle, &fp) == -1) {
            fprintf(stderr, "Nemohu nastavit filtr: %s\n", pcap_geterr(handle));
            return 2;
        }

        // Capture packets in a loop until interrupted
        while (keep_running) {
            pcap_dispatch(handle, 10, packet_handler_static, NULL);
            display_stats();
            sleep(1);
        }

        // Close the pcap handle and clean up
        pcap_close(handle);
        printf("\nProgram byl ukončen.\n");
        return 0;
    }

};

TrafickHandler *TrafickHandler::instance = nullptr;
