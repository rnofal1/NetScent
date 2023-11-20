/* This file contains declarations for the PacketCap class
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETCAP_H
#define PACKETCAP_H


/* 3rd-Party */
#include <pcap/pcap.h>

/* Local */
#include "MainWindow.h"
#include "NetworkAdapter.h"
#include "SharedQueue.h"

/* Defines */
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


/*  Top-level packet-handler class.
 *  Implementation derived from:
 *  https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
 */
class PacketCap : public QObject {
    Q_OBJECT
public:
    /* Functions */
    PacketCap(QPointer<MainWindow>& main_window, SharedQueue<Packet*>* packet_queue);

    //Store info about all adapters on system
    std::vector<NetworkAdapter> get_network_adapters();

    //Find the active network adapter
    NetworkAdapter get_preferred_adapter();

    //ToDo: could/should make this an overloaded function
    Packet* create_generic_packet(const struct ip& ip_header);
    Packet* create_tcp_packet(const struct ip& ip_header, const struct tcphdr& tcp_header);
    Packet* create_udp_packet(const struct ip& ip_header, const struct udphdr& udp_header);
    Packet* create_icmp_packet(const struct ip& ip_header, const struct icmp& icmp_header);

    pcap_t* create_pcap_handle(char* device, char* filter, int promisc);

    //This needs to be static due to npcap requirements
    static void packet_handler( u_char *user,
                               const struct pcap_pkthdr *packet_header,
                               const u_char *packet_ptr);

    int run_packet_cap();

    //Create the linked-list of detected network devices/adapters
    bool set_device_list();

    //Iterate through the linked-list until it points to device with device_name
    bool find_device(const std::string& device_name);

    //Set our device to be the active network adapter
    bool set_preferred_device();

    //Set our device to be the device with name device_name
    bool set_device(const std::string& device_name);

    void connect_signals_slots();

    //Print info for a single network adapter/device
    void print_network_adapter_info(const NetworkAdapter& adapter);

    //Print info for all detected network adapters/devices
    void print_all_adapter_info();

    //Only capture certain types of traffic (see Npcap docs)
    void set_filter(const int optind, const int argc, char** argv);

    void stop_capture(int signo);

    void get_link_header_len(pcap_t* handle);

    void enqueue_packet(Packet* packet);

    //Update our adapter list and emit the change to anyone listening if necessary
    void update_adapter_list(bool emit_change = false);

private:
    /* Variables */
    QPointer<MainWindow>& main_window;

    std::vector<NetworkAdapter> network_adapters;

    SharedQueue<Packet*> *packet_queue;

    //Packet capture channel identifier (used in all pcap calls)
    pcap_t* handle;

    pcap_if_t* devices;

    //Offset to skip over datalink layer header to get to packet IP header
    int link_header_len;

    //Number of packets at which to end packet capture
    int max_packets;

    //Total number of packets captured and processed
    int num_packets;

    int opt;

    char device[DEFAULT_CHAR_BUFF];
    char filter[DEFAULT_CHAR_BUFF];

    bool ui_open;
    bool run_capture;
    bool in_pcap_loop;

    //Indicator to wipe packets
    bool clear_packets;

signals:
    //<description, name> pair for each device
    void new_devices_found(const std::vector<std::pair<std::string, std::string>>& device_list);

    //Tell anyone listening that PacketCap has changed the network device/adapter it is listening to
    void device_set(const QString& device_name);

private slots:
    void device_changed_by_user(const QString& device_name);
    void ui_closed();
    void change_capture_state(const bool& capture_enabled);

    //Sets clear_packets variable to true
    void packets_cleared();
};

#endif // PACKETCAP_H
