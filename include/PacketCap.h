/* This file contains declarations for the PacketCap class
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETCAP_H
#define PACKETCAP_H


//3rd-Party
#include <pcap/pcap.h>

//Local
#include "MainWindow.h"
#include "NetworkAdapter.h"
#include "SharedQueue.h"

//Defines
#define PACKET_BUFF_TIMEOUT 1000 //https://www.tcpdump.org/manpages/pcap.3pcap.html
#define HEADER_FIELD_CHAR_BUFF 256
#define DEFAULT_CHAR_BUFF 256
#define SIZE_LOOPBACK_HEADER 4 //https://www.tcpdump.org/linktypes.html
#define SIZE_SLIP_HEADER 8 //https://www.networxsecurity.org/members-area/glossary/s/slip.html
#define SIZE_ETHERNET_HEADER 14 //https://www.firewall.cx/networking/ethernet/ieee-8023-snap-frame.html
#define SIZE_PPP_HEADER 24 //http://www.tcpipguide.com/free/t_PPPMultilinkProtocolMPFrameFormat-3.htm
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x)) //For Win32 IP Helper functions
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x)) //For Win32 IP Helper functions
#define EMPTY_GATEWAY "0.0.0.0"


/* Top-level packet-handler class.
 *  Implementation heavily derived from:
 *  https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
 */
class PacketCap {
public:
    PacketCap(MainWindow* main_window, SharedQueue<Packet*>* packet_queue);

    static void packet_handler( u_char *user,
                               const struct pcap_pkthdr *packet_header,
                               const u_char *packet_ptr);

    void set_device(const char* network_interface);

    void print_network_adapter_info(const NetworkAdapter& adapter); //Necessary due to printing within Qt debug

    void print_all_adapter_info();

    void set_filter(const int optind, const int argc, char** argv);

    void run_packet_cap();

    void stop_capture(int signo);

    void get_link_header_len(pcap_t* handle);

    void enqueue_packet(Packet* packet);

    pcap_t* create_pcap_handle(char* device, char* filter, int promisc);

    Packet* create_generic_packet(const struct ip& ip_header);

    Packet* create_tcp_packet(const struct ip& ip_header, const struct tcphdr& tcp_header);

    Packet* create_udp_packet(const struct ip& ip_header, const struct udphdr& udp_header);

    Packet* create_icmp_packet(const struct ip& ip_header, const struct icmp& icmp_header);

    std::vector<NetworkAdapter> get_network_adapters(); //ToDo: could create a new class derived from PIP_ADAPTER_INFO

    NetworkAdapter get_preferred_adapter();

private:
    MainWindow* main_window;

    SharedQueue<Packet*> *packet_queue;

    std::vector<NetworkAdapter> network_adapters;

    pcap_t* handle; //Packet capture channel identifier (used in all pcap calls)

    int link_header_len; //Offset to skip over datalink layer header to get to packet IP header
    int max_packets; //Number of packets at which to end packet capture
    int num_packets; //Total number of packets captured and processed
    int opt;

    char device[DEFAULT_CHAR_BUFF];
    char filter[DEFAULT_CHAR_BUFF];
};


#endif // PACKETCAP_H
