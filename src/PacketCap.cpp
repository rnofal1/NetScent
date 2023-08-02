//Local
#include "PacketCap.h"


int PacketCap::num_packets = 0;
int PacketCap::link_header_len = 0;
pcap_t* PacketCap::handle = nullptr;
QTextBrowser* PacketCap::infoPane = nullptr;
MainWindow* PacketCap::mainWindow = nullptr;

/*  PacketCap constructor
*/
PacketCap::PacketCap(MainWindow* mainWindowInit) {
    count = 0;
    *device = 0;
    *filter = 0;
    mainWindow = mainWindowInit;
    infoPane = mainWindow->getUiPointer()->textBrowser;
}


/*  Creates a packet capture endpoint to receive packets described by a packet
*   capture filter.
*/
pcap_t* PacketCap::create_pcap_handle(char* device,
                                      char* filter,
                                      int promisc = 1) {

    char error_buff[PCAP_ERRBUF_SIZE];
    pcap_if_t* devices = NULL; //List of devices
    struct bpf_program bpf; //https://en.wikipedia.org/wiki/Berkeley_Packet_Filter
    bpf_u_int32 netmask;
    bpf_u_int32 src_ip;

    /*  If no network interface (device) is specified, get the first one.
    *
    *   pcap_findalldevs() constructs a list of network devices, it returns 0 if
    *   no devices are found (success) and PCAP_ERROR if 1 or more devices are
    *   found (failure). On failure, error_buff is filled appropriately.
    */
    if (!*device) {
        if (pcap_findalldevs(&devices, error_buff) == PCAP_ERROR) {
            fprintf(stderr, "pcap_findalldevs(): %s\n", error_buff);
            return NULL;
        }
        strcpy(device, devices[0].name);
    }

    /*  Get network device source IP address and netmask.
    *
    *   pcap_lookupnet() determines the IPv4 network number and mask associated
    *   with a network device. Returns 0 on success and PCAP_ERROR on failure.
    */
    if (pcap_lookupnet(device, &src_ip, &netmask, error_buff) == PCAP_ERROR) {
        fprintf(stderr, "pcap_lookupnet(): %s\n", error_buff);
        return NULL;
    }

    /*  Open the device for live capture
    *
    *   pcap_open_live() obtains a packet capture handle to look at packets on
    *   the network. Returns pcap_t* on success and NULL on failure.
    */
    pcap_t* handle = pcap_open_live(device, BUFSIZ, promisc, PACKET_BUFF_TIMEOUT, error_buff);
    if (handle == NULL) {
        fprintf(stderr, "pcap_open_live(): %s\n", error_buff);
        return NULL;
    }

    /*  Convert the packet filter epxression into a packet filter binary.
    *
    *   pcap_compile() compiles a string into a filter program. Returns 0 on
    *   success, PCAP_ERROR on failure
    */
    if (pcap_compile(handle, &bpf, filter, 1, netmask) == PCAP_ERROR) {
        fprintf(stderr, "pcap_compile(): %s\n", pcap_geterr(handle));
        return NULL;
    }

    /*  Bind the packet filter to the libpcap handle.
    *
    *   pcap_setfilter() specifies a filter program. Returns 0 on success,
    *   PCAP_ERROR_NOT_ACTIVATED if called on capture handles that has been
    *   created but not activated, and PCAP_ERROR on other errors.
    */
    if (pcap_setfilter(handle, &bpf) == PCAP_ERROR) {
        fprintf(stderr, "pcap_setfilter(): %s\n", pcap_geterr(handle));
        return NULL;
    }

    return handle;
}


/*  Gets the link header type and size to be used during packet capture and
*   parsing
*
*   https://www.networxsecurity.org/members-area/glossary/d/data-link-layer.html
*   https://www.tcpdump.org/linktypes.html
*/
void PacketCap::get_link_header_len(pcap_t* handle) {
    int link_type = pcap_datalink(handle);

    // Determine the datalink layer type.
    if(link_type == PCAP_ERROR_NOT_ACTIVATED) {
        printf("pcap_datalink(): %s\n", pcap_geterr(handle));
        return;
    }

    switch (link_type) {
    case DLT_NULL:
        link_header_len = SIZE_LOOPBACK_HEADER;
        break;
    case DLT_SLIP:
        link_header_len = SIZE_SLIP_HEADER;
        break;
    case DLT_EN10MB:
        link_header_len = SIZE_ETHERNET_HEADER;
        break;
    case DLT_PPP:
        link_header_len = SIZE_PPP_HEADER;
        break;
    default:
        printf("Unsupported datalink (%d)\n", link_type);
        link_header_len = 0;
    }
}

template<typename T>
void PacketCap::send_to_ui(const struct ip& ip_header, const T& network_protocol_header) {
    QMetaObject::invokeMethod(mainWindow,  [=]()
                              {
                                  mainWindow->addPacket(ip_header, network_protocol_header, num_packets++);
                              });
}

void PacketCap::send_to_ui(const struct ip& ip_header) {
    QMetaObject::invokeMethod(mainWindow,  [=]()
                              {
                                  mainWindow->addPacket(ip_header, num_packets++);
                              });
}


/*  Call-back function to parse and display the contents of each captured
*   packet
*
*   Basically untouched from https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
*/
void PacketCap::packet_handler(u_char *user,
                               const struct pcap_pkthdr *packet_header,
                               const u_char *packet_ptr) {
    struct ip* ip_header;

    // Skip the datalink layer header and get the IP header fields.
    packet_ptr += link_header_len;
    ip_header = (struct ip*)(packet_ptr); //ToDo look into this cast

    // Advance to the transport layer header then parse and display
    // the fields based on the type of header: tcp, udp or icmp.
    packet_ptr += 4*ip_header->ip_hl;

    switch (ip_header->ip_p) {
    case IPPROTO_TCP:
        std::cout << "TCP\n";
        send_to_ui(*ip_header, *(struct tcphdr*)packet_ptr);
        break;

    case IPPROTO_UDP:
        std::cout << "UDP\n";
        send_to_ui(*ip_header, *(struct udphdr*)packet_ptr);
        break;

    case IPPROTO_ICMP:
        send_to_ui(*ip_header, *(struct icmp*)packet_ptr);
        break;

    default:
        send_to_ui(*ip_header);
        break;
    }
}


/*  Registered as the handler function for each of the signals SIGINT, SIGTERM,
*   and SIGQUIT, which are raised when a process is interrupted. Also called
*   when the program terminates normally after a specified number of packets
*   are captured
*/
void PacketCap::stop_capture(int signo) {
    struct pcap_stat stats;

    if (pcap_stats(handle, &stats) >= 0) {
        printf("\n%d packets captured\n", num_packets);
        printf("%d packets received by filter\n", stats.ps_recv);
        printf("%d packets dropped\n\n", stats.ps_drop);
    }

    pcap_close(handle);
    exit(0);
}

void PacketCap::set_device(const char* network_interface) {
    strcpy(device, network_interface);
}

void PacketCap::set_desired_num_packets(const char* desired_num_packets) {
    count = atoi(desired_num_packets);
}

void PacketCap::set_filter(const int optind, const int argc, char** argv) {
    for (int i = optind; i < argc; i++) {
        strcat(filter, argv[i]);
        strcat(filter, " ");
    }
}

int PacketCap::run_packet_cap() {
    //Binds stop_capture() as handler function to the following signals
    signal(SIGINT, PacketCap::stop_capture);
    signal(SIGTERM, PacketCap::stop_capture);
    signal(SIGQUIT, PacketCap::stop_capture);

    // Create packet capture handle.
    handle = create_pcap_handle(device, filter);
    if (handle == NULL) {
        return -1;
    }

    // Get the type of link layer.
    get_link_header_len(handle);
    if (link_header_len == 0) {
        return -1;
    }

    // Start the packet capture with a set count or continually if the count is 0.
    if (pcap_loop(handle, count, packet_handler, (u_char*)NULL) == PCAP_ERROR) {
        fprintf(stderr, "pcap_loop failed: %s\n", pcap_geterr(handle));
        return -1;
    }

    stop_capture(0);

    return 0;
}
