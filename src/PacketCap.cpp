/* This file contains definitions for the PacketCap class
 *
 * I've tried to limit the amount of C-style paradigms used here
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketCap.h"


//Static variable declarations
MainWindow* PacketCap::main_window = nullptr;
QTextBrowser* PacketCap::infoPane = nullptr;
pcap_t* PacketCap::handle = nullptr;
int PacketCap::num_packets = 0;
int PacketCap::link_header_len = 0;


PacketCap::PacketCap(MainWindow* main_window_init) {
    main_window = main_window_init;
    count = 0;
    *device = 0;
    *filter = 0;
    infoPane = main_window->get_ui_pointer()->infoPane;
}

/* Creates a packet capture endpoint to receive packets described by a packet
 * capture filter
 */
pcap_t* PacketCap::create_pcap_handle(char* device, char* filter, int promisc = 1) {
    pcap_if_t* devices = NULL; //List of devices
    struct bpf_program bpf; //https://en.wikipedia.org/wiki/Berkeley_Packet_Filter
    bpf_u_int32 netmask;
    bpf_u_int32 src_ip;
    char error_buff[PCAP_ERRBUF_SIZE];

    /* If no network interface (device) is specified, get the first one
     *
     * pcap_findalldevs() constructs a list of network devices, it returns 0 if
     * no devices are found (success) and PCAP_ERROR if 1 or more devices are
     * found (failure). On failure, error_buff is filled appropriately
     */
    if (!*device) {
        if (pcap_findalldevs(&devices, error_buff) == PCAP_ERROR) {
            std::cerr << "pcap_findalldevs(): " << error_buff << "\n";
            return NULL;
        }
        while(devices && std::string(devices->description) != "802.11ac Wireless LAN Card") {
            qInfo() << "DEVICE " << devices->description << "\n";
            devices = devices->next;
        }

        strncpy(device, devices->name, DEFAULT_CHAR_BUFF);
    }

    /* Get network device source IP address and netmask
     *
     * pcap_lookupnet() determines the IPv4 network number and mask associated
     * with a network device. Returns 0 on success and PCAP_ERROR on failure
     */
    if (pcap_lookupnet(device, &src_ip, &netmask, error_buff) == PCAP_ERROR) {
        std::cerr << "pcap_lookupnet(): " << error_buff << "\n";
        return NULL;
    }

    /* Open the device for live capture
     *
     * pcap_open_live() obtains a packet capture handle to look at packets on
     * the network. Returns pcap_t* on success and NULL on failure
     */
    pcap_t* handle = pcap_open_live(device, BUFSIZ, promisc, PACKET_BUFF_TIMEOUT, error_buff);
    if (handle == NULL) {
        std::cerr << "pcap_open_live(): " << error_buff << "\n";
        return NULL;
    }

    /* Convert the packet filter epxression into a packet filter binary
     *
     * pcap_compile() compiles a string into a filter program. Returns 0 on
     * success, PCAP_ERROR on failure
     */
    if (pcap_compile(handle, &bpf, filter, 1, netmask) == PCAP_ERROR) {
        std::cerr << "pcap_compile(): " << pcap_geterr(handle) << "\n";
        return NULL;
    }

    /* Bind the packet filter to the libpcap handle.
     *
     * pcap_setfilter() specifies a filter program. Returns 0 on success,
     * PCAP_ERROR_NOT_ACTIVATED if called on capture handles that has been
     * created but not activated, and PCAP_ERROR on other errors.
     */
    if (pcap_setfilter(handle, &bpf) == PCAP_ERROR) {
        std::cerr << "pcap_setfilter(): " << pcap_geterr(handle) << "\n";
        return NULL;
    }

    return handle;
}

//ToDo: would prefer to have stop_capture(0) guaranteed to run on window close
int PacketCap::run_packet_cap() {
    //Binds stop_capture() as handler function to the following signals
    signal(SIGINT, PacketCap::stop_capture);
    signal(SIGTERM, PacketCap::stop_capture);
    signal(SIGBREAK, PacketCap::stop_capture);

    //Start the packet capture
    while(main_window && !main_window->closed) {
        if(main_window->run_capture){
            //Reset packet count if necessary
            if(main_window->clear_packets) {
                num_packets = 0;
                main_window->clear_packets = false;
            }

            qInfo() << "Starting packet capture...\n\n";

            //Create packet capture handle.
            handle = create_pcap_handle(device, filter);
            if (handle == NULL) {
                return -1;
            }

            //Get the type of link layer.
            get_link_header_len(handle);
            if (link_header_len == 0) {
                return -1;
            }

            qInfo() << "Initiating packet capture loop...\n\n";

            //This runs continuously until reaching one of our stop conditions
            if (pcap_loop(handle, count, packet_handler, (u_char*)NULL) == PCAP_ERROR) {
                std::cerr << "pcap_loop failed: " << pcap_geterr(handle) << "\n";
                return -1;
            }

            stop_capture(0);
            qInfo() << "Packet capture halted.\n\n";
        }
    }

    std::cout << "Program window closed.\n\n";
    return 0;
}

/* Gets the link header type and size to be used during packet capture and
 * parsing
 *
 * https://www.networxsecurity.org/members-area/glossary/d/data-link-layer.html
 * https://www.tcpdump.org/linktypes.html
 */
void PacketCap::get_link_header_len(pcap_t* handle) {
    int link_type = pcap_datalink(handle);

    //Determine the datalink layer type.
    if(link_type == PCAP_ERROR_NOT_ACTIVATED) {
        std::cout << "pcap_datalink(): " << pcap_geterr(handle) << "\n";
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
        std::cout << "Unsupported datalink(): " << link_type << "\n";
        link_header_len = 0;
    }
}

//ToDo: consider moving this around to follow normal templated function conventions
template<typename T>
void PacketCap::send_to_ui(const struct ip& ip_header, const T& network_protocol_header) {
    QMetaObject::invokeMethod(main_window,  [=]()
                    {
                        main_window->add_packet(ip_header, network_protocol_header, num_packets++);
                    });
}

//ToDo: would prefer this to be a single (templated) function
void PacketCap::send_to_ui(const struct ip& ip_header) {
    QMetaObject::invokeMethod(main_window,  [=]()
                    {
                        main_window->add_packet(ip_header, num_packets++);
                    });
}

/* Call-back function to parse and display the contents of each captured
 * packet
 *
 */
void PacketCap::packet_handler(u_char *user,
                               const struct pcap_pkthdr *packet_header,
                               const u_char *packet_ptr) {
    Q_UNUSED(user); Q_UNUSED(packet_header);

    if(!main_window || main_window->closed || !main_window->run_capture || main_window->clear_packets) {
        pcap_breakloop(handle);
    }


    struct ip* ip_header;

    // Skip the datalink layer header and get the IP header fields.
    packet_ptr += link_header_len;
    ip_header = (struct ip*)(packet_ptr); //ToDo look into this cast

    // Advance to the transport layer header then parse and display
    // the fields based on the type of header: tcp, udp or icmp.
    packet_ptr += 4*ip_header->ip_hl;

    switch (ip_header->ip_p) {
    case IPPROTO_TCP:
        std::cout << "In TCP packet cap\n";
        send_to_ui(*ip_header, *(struct tcphdr*)packet_ptr);
        break;

    case IPPROTO_UDP:
        std::cout << "In UDP packet cap\n";
        send_to_ui(*ip_header, *(struct udphdr*)packet_ptr);
        break;

    case IPPROTO_ICMP:
        std::cout << "In ICMP packet cap\n";
        send_to_ui(*ip_header, *(struct icmp*)packet_ptr);
        break;

    //ToDo: do something about non-TCP/UDP/ICMP packets/headers
    // default: ...
    }
}

/* Registered as the handler function for each of the signals SIGINT, SIGTERM,
 * and SIGQUIT, which are raised when a process is interrupted. Also called
 * when the program terminates normally after a specified number of packets
 * are captured
 */
void PacketCap::stop_capture(int signo) {
    Q_UNUSED(signo);

    struct pcap_stat stats;

    if (pcap_stats(handle, &stats) >= 0) {
        std::cout << "\n" << num_packets << " packets captured\n"
                  << stats.ps_recv << " packets received by filter\n"
                  << stats.ps_drop << " packets dropped\n\n";
    }

    pcap_close(handle);
}

void PacketCap::set_desired_num_packets(const char* desired_num_packets) {
    count = atoi(desired_num_packets);
}

void PacketCap::set_device(const char* network_interface) {
    strncpy(device, network_interface, DEFAULT_CHAR_BUFF);
}

void PacketCap::set_filter(const int optind, const int argc, char** argv) {
    for (int i = optind; i < argc; i++) {
        strcat(filter, argv[i]);
        strcat(filter, " ");
    }
}
