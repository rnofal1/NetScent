#include "Packet.h"

Packet::Packet(const struct ip& ip_header)
    : time_added(std::time(nullptr)), ip_header(ip_header){}

std::string Packet::get_ip_info() {
    std::string ip_info =  "ID:" + std::to_string(ntohs(ip_header.ip_id))
                        + " TOS:0x" + std::to_string(ip_header.ip_tos)
                        + ", TTL:" + std::to_string(ip_header.ip_ttl)
                        + " IpLen:" + std::to_string(4*ip_header.ip_hl)
                        + " DgLen:" + std::to_string(ntohs(ip_header.ip_len))
                        + " Time Added: " + std::asctime(std::localtime(&time_added));
    return ip_info;
}


std::string Packet::get_info() {
    return get_ip_info();
}

TCPPacket::TCPPacket(const struct ip& ip_header, const struct tcphdr& tcp_header)
    : Packet(ip_header), tcp_header(tcp_header){}

//ToDo: change this function (and similar functions) to use a formatting function similar to sprintf
std::string TCPPacket::get_info() {
    std::string tcp_info =  "TCP " + std::string(inet_ntoa(ip_header.ip_src))
                            + ":" + std::to_string(ntohs(tcp_header.th_sport))
                            + " -> " + std::string(inet_ntoa(ip_header.ip_dst))
                            + std::to_string(ntohs(tcp_header.th_dport)) + "\n";

    tcp_info += get_ip_info() + "\n";

    tcp_info += (tcp_header.th_flags & TH_URG ? 'U' : '*')
                +  (tcp_header.th_flags & TH_ACK ? 'A' : '*')
                +  (tcp_header.th_flags & TH_PUSH ? 'P' : '*')
                +  (tcp_header.th_flags & TH_RST ? 'R' : '*')
                +  (tcp_header.th_flags & TH_SYN ? 'S' : '*')
                +  (tcp_header.th_flags & TH_SYN ? 'F' : '*')
                +  "Seq: 0x" + std::to_string(ntohl(tcp_header.th_seq))
                +  " Ack: 0x" + std::to_string(ntohl(tcp_header.th_ack))
                +  " Win: 0x" + std::to_string(ntohs(tcp_header.th_win))
                +  " TcpLen: " + std::to_string(4*tcp_header.th_off)
                + "\n";

    return tcp_info;
}


UDPPacket::UDPPacket(const struct ip& ip_header, const struct udphdr& udp_header)
    : Packet(ip_header), udp_header(udp_header){}

std::string UDPPacket::get_info() {
    std::string udp_info =  "UDP " + std::string(inet_ntoa(ip_header.ip_src))
                            + ":" + std::to_string(ntohs(udp_header.uh_sport))
                            +  " -> " + std::string(inet_ntoa(ip_header.ip_dst))
                            + ":" + std::to_string(ntohs(udp_header.uh_dport)) + "\n";

    udp_info += get_ip_info() + "\n";

    return udp_info;
}


ICMPPacket::ICMPPacket(const struct ip& ip_header, const struct icmp& icmp_header)
    : Packet(ip_header), icmp_header(icmp_header){}

std::string ICMPPacket::get_info() {
    std::string icmp_info = "ICMP " + std::string(inet_ntoa(ip_header.ip_src))
                            + " -> " + std::string(inet_ntoa(ip_header.ip_dst)) + "\n";

    icmp_info += get_ip_info();

    icmp_info +=    "Type:" + std::to_string(icmp_header.icmp_type)
                    + "Code:" + std::to_string(icmp_header.icmp_code)
                    + "ID:" + std::to_string(ntohs(icmp_header.icmp_hun.ih_idseq.icd_id))
                    + "Seq:" + std::to_string(ntohs(icmp_header.icmp_hun.ih_idseq.icd_seq)) + "\n";

    return icmp_info;
}
