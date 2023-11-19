/* This file contains definitions for the PacketCap class
 *
 * I've tried to limit the amount of C-style paradigms used here
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>

//Local
#include "PacketCap.h"


PacketCap::PacketCap(QPointer<MainWindow>& main_window, SharedQueue<Packet*>* packet_queue) :
                                                        main_window(main_window),
                                                        packet_queue(packet_queue),
                                                        link_header_len(0),
                                                        max_packets(0), //0 == -1 == INF for pcap_loop()
                                                        num_packets(0),
                                                        ui_open(true),
                                                        run_capture(false),
                                                        in_pcap_loop(false){
    *device = 0;
    *filter = 0;
    connect_signals_slots();
}

void PacketCap::connect_signals_slots() {
    //this -> device select
    connect(this, SIGNAL(new_devices_found(std::vector<std::pair<std::string, std::string>>)),
            main_window->get_ui_pointer()->deviceSelect,
            SLOT(add_item_pairs(std::vector<std::pair<std::string, std::string>>)));

    //this -> device select
    connect(this, SIGNAL(device_set(QString)),
            main_window->get_ui_pointer()->deviceSelect,
            SLOT(packet_cap_picked_adapter(QString)));

    //device select -> this
    connect(main_window->get_ui_pointer()->deviceSelect,
            SIGNAL(user_picked_device(QString)), this,
            SLOT(device_changed_by_user(QString)));

    //main_window -> this
    connect(main_window.data(),
            SIGNAL(ui_closed()), this,
            SLOT(ui_closed()));

    //main_window -> this
    connect(main_window.data(),
            SIGNAL(change_capture_state(bool)), this,
            SLOT(change_capture_state(bool)));
}

/* Creates a packet capture endpoint to receive packets described by a packet
 * capture filter
 */
pcap_t* PacketCap::create_pcap_handle(char* device, char* filter, int promisc = 1) {
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
    if (!*device) { //Will only run the very first time (when no devices yet detected)
        if(set_preferred_device() == false) {
            return NULL;
        }
    }

    /* Get network device source IP address and netmask
     *
     * pcap_lookupnet() determines the IPv4 network number and mask associated
     * with a network device. Returns 0 on success and PCAP_ERROR on failure
     */
    if (pcap_lookupnet(device, &src_ip, &netmask, error_buff) == PCAP_ERROR) {
        qDebug() << "pcap_lookupnet(): " << error_buff;
        return NULL;
    }

    /* Open the device for live capture
     *
     * pcap_open_live() obtains a packet capture handle to look at packets on
     * the network. Returns pcap_t* on success and NULL on failure
     */
    pcap_t* handle = pcap_open_live(device, BUFSIZ, promisc, PACKET_BUFF_TIMEOUT, error_buff);
    if (handle == NULL) {
        qDebug() << "pcap_open_live(): " << error_buff;
        return NULL;
    }

    /* Convert the packet filter epxression into a packet filter binary
     *
     * pcap_compile() compiles a string into a filter program. Returns 0 on
     * success, PCAP_ERROR on failure
     */
    if (pcap_compile(handle, &bpf, filter, 1, netmask) == PCAP_ERROR) {
        qDebug() << "pcap_compile(): " << pcap_geterr(handle);
        return NULL;
    }

    /* Bind the packet filter to the libpcap handle.
     *
     * pcap_setfilter() specifies a filter program. Returns 0 on success,
     * PCAP_ERROR_NOT_ACTIVATED if called on capture handles that has been
     * created but not activated, and PCAP_ERROR on other errors.
     */
    if (pcap_setfilter(handle, &bpf) == PCAP_ERROR) {
        qDebug() << "pcap_setfilter(): " << pcap_geterr(handle);
        return NULL;
    }

    return handle;
}


/* Call-back function to parse and display the contents of each captured
 * packet
 *
 */
void PacketCap::packet_handler( u_char *user,
                        const struct pcap_pkthdr *packet_header,
                        const u_char *packet_ptr) {
    Q_UNUSED(packet_header);
    PacketCap *packet_cap_obj = reinterpret_cast<PacketCap *>(user);
    QPointer<MainWindow> main_window_obj = packet_cap_obj->main_window;
    if(!main_window_obj || main_window_obj->closed || !main_window_obj->run_capture || main_window_obj->clear_packets) {
        pcap_breakloop(packet_cap_obj->handle);
        return;
    }

    struct ip* ip_header;

    // Skip the datalink layer header and get the IP header fields.
    packet_ptr += packet_cap_obj->link_header_len;
    ip_header = (struct ip*)(packet_ptr); //ToDo look into this cast

    // Advance to the transport layer header then parse and display
    // the fields based on the type of header: tcp, udp or icmp.
    packet_ptr += 4*ip_header->ip_hl;

    Packet* packet_to_add;

    switch (ip_header->ip_p) {
    case IPPROTO_TCP:
        qDebug() << "In TCP packet cap";
        packet_to_add = packet_cap_obj->create_tcp_packet(*ip_header, *(struct tcphdr*)packet_ptr);
        packet_cap_obj->enqueue_packet(packet_to_add);
        break;

    case IPPROTO_UDP:
        qDebug() << "In UDP packet cap";
        packet_to_add = packet_cap_obj->create_udp_packet(*ip_header, *(struct udphdr*)packet_ptr);
        packet_cap_obj->enqueue_packet(packet_to_add);
        break;

    case IPPROTO_ICMP:
        qDebug() << "In ICMP packet cap";
        packet_to_add = packet_cap_obj->create_icmp_packet(*ip_header, *(struct icmp*)packet_ptr);
        packet_cap_obj->enqueue_packet(packet_to_add);
        break;

        //ToDo: do something about non-TCP/UDP/ICMP packets/headers
        // default: ...
    }
}

void PacketCap::enqueue_packet(Packet* packet) {
    packet_queue->push(packet);
}

Packet* PacketCap::create_generic_packet(const struct ip& ip_header) {
    Packet* packet = new Packet(ip_header, num_packets++);
    return packet;
}

Packet* PacketCap::create_tcp_packet(const struct ip& ip_header, const struct tcphdr& tcp_header) {
    TCPPacket* packet = new TCPPacket(ip_header, num_packets++, tcp_header);
    return packet;
}

Packet* PacketCap::create_udp_packet(const struct ip& ip_header, const struct udphdr& udp_header) {
    UDPPacket* packet = new UDPPacket(ip_header, num_packets++, udp_header);
    return packet;
}

Packet* PacketCap::create_icmp_packet(const struct ip& ip_header, const struct icmp& icmp_header) {
    ICMPPacket* packet = new ICMPPacket(ip_header, num_packets++, icmp_header);
    return packet;
}

int PacketCap::run_packet_cap() {
    signal(SIGINT, exit);
    signal(SIGTERM, exit);
    signal(SIGBREAK, exit);

    //Start the packet capture
    while(ui_open) {
        if(run_capture){
            //Reset num_packets if necessary
            if(main_window->clear_packets) {
                num_packets = 0;
                main_window->clear_packets = false;
            }

            qDebug() << "Starting packet capture...\n";

            //Create packet capture handle.
            handle = create_pcap_handle(device, filter);
            if (handle == NULL) {
                //QThread::currentThread()->exit(1);
                return 1;
            }

            //Get the type of link layer.
            get_link_header_len(handle);
            if (link_header_len == 0) {
               //QThread::currentThread()->exit(1);
                return 1;
            }

            qDebug() << "Initiating packet capture loop...\n";

            //This runs continuously until reaching one of our stop conditions
            in_pcap_loop = true;
            if (pcap_loop(handle, max_packets, packet_handler, reinterpret_cast<u_char *>(this)) == PCAP_ERROR) {
                qDebug() << "pcap_loop failed: " << pcap_geterr(handle);
                //QThread::currentThread()->exit(1);
                return 1;
            }
            in_pcap_loop = false;

            stop_capture(0);
            qDebug() << "Packet capture halted.\n";
        }
    }

    qDebug() << "PacketCap: Program window closed.\n";
    //QThread::currentThread()->quit();
    return 0;
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
        qDebug() << "\n" << num_packets << " packets captured\n"
                  << stats.ps_recv << " packets received by filter\n"
                  << stats.ps_drop << " packets dropped\n";
    }

    pcap_close(handle);

    num_packets = 0;
    packet_queue->clear();
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
        qDebug() << "pcap_datalink(): " << pcap_geterr(handle);
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
        qDebug() << "Unsupported datalink(): " << link_type;
        link_header_len = 0;
    }
}

//Devices is a linked-list; return true on success, false otherwise
bool PacketCap::set_device_list() {
    devices = NULL; //List of devices
    char error_buff[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&devices, error_buff) == PCAP_ERROR) {
        qDebug() << "pcap_findalldevs(): " << error_buff;
        return false;
    }
    return true;
}

//Devices is a linked-list, loop through until device found
bool PacketCap::find_device(const std::string& device_name) {
    if(set_device_list() == false) {
        return false;
    }

    while(devices && std::string(devices->name).find(device_name) == std::string::npos) {
        devices = devices->next;
    }

    if(!devices) {
        qDebug() << "Device not found: " << device_name;
        return false;
    }

    return true;
}

bool PacketCap::set_preferred_device() {
    update_adapter_list(true);
    std::string adapter_name = get_preferred_adapter().get_name();

    if(set_device(adapter_name)) {
        QString qt_adapter_name = QString::fromStdString(adapter_name);
        emit device_set(qt_adapter_name);
        return true;
    }
    return false;
}

bool PacketCap::set_device(const std::string& device_name) {
    if(find_device(device_name) == false) {
        return false;
    }

    strncpy(device, devices->name, DEFAULT_CHAR_BUFF);
    return true;
}

void PacketCap::set_filter(const int optind, const int argc, char** argv) {
    for (int i = optind; i < argc; i++) {
        strcat(filter, argv[i]);
        strcat(filter, " ");
    }
}

void PacketCap::update_adapter_list(bool emit_change) {
    network_adapters = get_network_adapters();

    if(emit_change) {
        std::vector<std::pair<std::string, std::string>> adapter_list; //<description, name> pair for each device
        for(auto& adapter: network_adapters) {
            auto desc_name = std::make_pair(adapter.get_desc(), adapter.get_name());
            adapter_list.push_back(desc_name);
        }
        emit new_devices_found(adapter_list);
    }
}

//Return a vector of relevant adapter info (adapted from https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersinfo)
std::vector<NetworkAdapter> PacketCap::get_network_adapters() {
    std::vector<NetworkAdapter> found_adapters;
    PIP_ADAPTER_INFO net_adapter_info;
    PIP_ADAPTER_INFO net_adapter = NULL;
    DWORD adapter_call_ret_val = 0;

    u_long out_buf_len = sizeof(IP_ADAPTER_INFO);
    net_adapter_info = (IP_ADAPTER_INFO *) MALLOC(sizeof(IP_ADAPTER_INFO));
    if (net_adapter_info == NULL) {
        qDebug() << "Error allocating memory needed to call GetAdaptersinfo";
        return found_adapters;
    }

    //Make an initial call to GetAdaptersInfo to get the necessary size into out_buf_len
    if (GetAdaptersInfo(net_adapter_info, &out_buf_len) == ERROR_BUFFER_OVERFLOW) {
        FREE(net_adapter_info);
        net_adapter_info = (IP_ADAPTER_INFO *) MALLOC(out_buf_len);
        if (net_adapter_info == NULL) {
            qDebug("Error allocating memory needed to call GetAdaptersinfo");
            return found_adapters;
        }
    }

    //Iterate over all found adapters, adding them to the return vector
    if ((adapter_call_ret_val = GetAdaptersInfo(net_adapter_info, &out_buf_len)) == NO_ERROR) {
        net_adapter = net_adapter_info;
        while (net_adapter) {
            found_adapters.emplace_back(net_adapter);
            net_adapter = net_adapter->Next;
        }
    } else {
        qDebug("GetAdaptersInfo failed with error: %d", adapter_call_ret_val);
    }

    if (net_adapter_info) {
        FREE(net_adapter_info);
    }

    return found_adapters;
}

//ToDo: there must be a simpler/more reliable way to figure out which adapter is connected to the internet
NetworkAdapter PacketCap::get_preferred_adapter() {
    //If the adapter's gateway address does not match the string in EMPTY_GATEWAY, it is our default/pref adapter
    for(auto& adapter : network_adapters) {
        if(adapter.get_gateway() != EMPTY_GATEWAY) {
            return adapter;
        }
    }
    return NetworkAdapter();
}

void PacketCap::print_network_adapter_info(const NetworkAdapter& adapter) {
    qDebug() << adapter.get_all_info();
}

void PacketCap::print_all_adapter_info() {
    for(auto& adapter : network_adapters) {
        print_network_adapter_info(adapter);
    }
}

void PacketCap::device_changed_by_user(const QString& device_name) {
    qDebug() << "In: PacketCap::device_changed_by_user()";
    if(set_device(device_name.toStdString())) {
        qDebug() << "Device switched to: " << device_name;
    } else {
        qDebug() << "Could not switch to device: " << device_name;
    }
}

void PacketCap::ui_closed() {
    ui_open = false;
    if(in_pcap_loop && handle != NULL) {
        pcap_breakloop(handle);
    }
    qDebug() << "ui_open changed to: false";
}

void PacketCap::change_capture_state(const bool& capture_enabled) {
    run_capture = capture_enabled;
    if(!run_capture && in_pcap_loop && handle != NULL) {
        pcap_breakloop(handle);
    }
    qDebug() << "run_capture changed to: " << (run_capture ? "true" : "false");
}
