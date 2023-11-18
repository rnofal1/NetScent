/* This file contains definitions for the Packet class and its derived classes
 * Ramsey Nofal, 08/2023
 */


//Local
#include "Packet.h"
#include "util.h"

//ToDo: IMPORTANT. MINIMIZE std::string/QString type conversions

Packet::Packet(const struct ip& ip_header, const int& num)
               : ip_header(ip_header),
                time_added(std::time(nullptr)),
                num(num){}

Packet::~Packet(){}

QString Packet::get_info() const {
    return get_ip_info();
}

PacketType Packet::get_type() const {
    return PacketType::IP;
}
QString Packet::get_type_name() const {
    return "IP";
}

QString Packet::get_packet_spec_info() const {
    return "";
}

QString Packet::get_src_ip() const {
    return QString::fromStdString(std::string(inet_ntoa(ip_header.ip_src)));
}

QString Packet::get_dst_ip() const {
    return QString::fromStdString(std::string(inet_ntoa(ip_header.ip_dst)));
}

QString Packet::get_time_added() const {
    return QString::fromStdString(std::asctime(std::localtime(&time_added)));
}

int Packet::get_num() const {
    return num;
}

//Return a string containing miscellaneous ip header information
QString Packet::get_ip_info() const {
    std::string ip_info =  "ID:";
    ip_info += std::to_string(ntohs(ip_header.ip_id));
    ip_info += " TOS:0x";
    ip_info += std::to_string(ip_header.ip_tos);
    ip_info += ", TTL:";
    ip_info += std::to_string(ip_header.ip_ttl);
    ip_info += " IpLen:";
    ip_info += std::to_string(4*ip_header.ip_hl);
    ip_info += " DgLen:";
    ip_info += std::to_string(ntohs(ip_header.ip_len));
    ip_info += " Time Added: ";
    ip_info += std::asctime(std::localtime(&time_added));
    ip_info += "\n";

    return QString::fromStdString(ip_info);
}


/* get_ip_geo_json_info() uses curl to make an HTTP GET request to the
 * appropriate ip geolocation API
 *
 * This request requires a valid ip geolocation API key and takes the desired
 * ip address as a parameter
 *
 * ToDo: implement local ip location caching (for recently-inspected locations)
 * and/or switch to a local ip location database
 */
nlohmann::json Packet::get_ip_geo_json_info(const std::string& ip_addr) const {
    std::string key = get_geoloc_api_key();
    std::string get_url = "https://api.ipgeolocation.io/ipgeo?apiKey=" + key + "&ip="+ip_addr;
    return curl_get_json(get_url);
}

// ToDo: this could probably be a bit simpler
IPCoords Packet::get_ip_coords(const std::string& ip_addr) const {
    nlohmann::json loc_json = get_ip_geo_json_info(ip_addr);
    bool known_location = loc_json.contains("latitude"); //We known that if latitude exists, so will longitude
    float lati = 0.0;
    float longi = 0.0;

    if(known_location) {
        lati = get_json_val_float(loc_json, "latitude");
        longi = get_json_val_float(loc_json, "longitude");
    }

    return {known_location, lati, longi};
}

IPCoords Packet::get_dst_ip_coords() const {
    return get_ip_coords(get_dst_ip().toStdString());
}
IPCoords Packet::get_src_ip_coords() const {
    return get_ip_coords(get_src_ip().toStdString());
}

std::string Packet::parse_geo_info_json(const nlohmann::json& json) const {
    std::string json_info = + "Country: " +         get_json_val_string(json, "country_name")
                            + "\nState/Province: " +  get_json_val_string(json, "state_prov")
                            + "\nCity: " +            get_json_val_string(json, "city")
                            + "\nOrganization: " +    get_json_val_string(json, "organization") + "\n\n";
    return json_info;
}

std::string Packet::get_dst_geo_info() const{
    return parse_geo_info_json(get_ip_geo_json_info(get_dst_ip().toStdString()));
}

std::string Packet::get_src_geo_info() const {
    return parse_geo_info_json(get_ip_geo_json_info(get_src_ip().toStdString()));
}

std::string Packet::get_geo_info() {
    if(geo_info == "") {
        std::string src_ip = get_src_ip().toStdString();
        std::string dst_ip = get_dst_ip().toStdString();
        nlohmann::json src_json = get_ip_geo_json_info(src_ip);
        nlohmann::json dst_json = get_ip_geo_json_info(dst_ip);

        std::string src_info =  "Source Geographical Info:\nIP: "
                                + src_ip + '\n'
                                + parse_geo_info_json(src_json);
        std::string dst_info =  "Destination Geographical Info:\nIP: "
                               + dst_ip + '\n'
                               + parse_geo_info_json(dst_json);
        geo_info = src_info + dst_info;
    }

    return geo_info;
}

QString Packet::get_src_port() const {
    return "";
}
QString Packet::get_dst_port() const {
    return "";
}

TCPPacket::TCPPacket(const struct ip& ip_header, const int& num, const struct tcphdr& tcp_header)
                    : Packet(ip_header, num),
                    tcp_header(tcp_header){}

//ToDo: change this function (and similar functions) to use a formatting function similar to sprintf?
//ToDo: probably some way to minimize type conversions to/from std::string
QString TCPPacket::get_info() const {
    std::string tcp_info =  "TCP ";
    tcp_info += std::string(inet_ntoa(ip_header.ip_src));

    tcp_info += ":";
    tcp_info += std::to_string(ntohs(tcp_header.th_sport));
    tcp_info += " -> ";
    tcp_info += std::string(inet_ntoa(ip_header.ip_dst));
    tcp_info += ":";
    tcp_info += std::to_string(ntohs(tcp_header.th_dport));
    tcp_info += "\n";

    tcp_info += get_ip_info().toStdString();
    tcp_info += get_packet_spec_info().toStdString();

    return QString::fromStdString(tcp_info);
}

PacketType TCPPacket::get_type() const {
    return PacketType::TCP;
}
QString TCPPacket::get_type_name() const {
    return "TCP";
}

QString TCPPacket::get_src_port() const {
    return QString::fromStdString(std::to_string(ntohs(tcp_header.th_sport)));
}
QString TCPPacket::get_dst_port() const {
    return QString::fromStdString(std::to_string(ntohs(tcp_header.th_dport)));
}

QString TCPPacket::get_packet_spec_info() const {
    std::string tcp_info =  "";
    tcp_info += (tcp_header.th_flags & TH_URG ? 'U' : '*');
    tcp_info +=  (tcp_header.th_flags & TH_ACK ? 'A' : '*');
    tcp_info +=  (tcp_header.th_flags & TH_PUSH ? 'P' : '*');
    tcp_info +=  (tcp_header.th_flags & TH_RST ? 'R' : '*');
    tcp_info +=  (tcp_header.th_flags & TH_SYN ? 'S' : '*');
    tcp_info +=  (tcp_header.th_flags & TH_SYN ? 'F' : '*');
    tcp_info +=  "Seq: 0x";
    tcp_info += std::to_string(ntohl(tcp_header.th_seq));
    tcp_info +=  " Ack: 0x";
    tcp_info += std::to_string(ntohl(tcp_header.th_ack));
    tcp_info +=  " Win: 0x";
    tcp_info += std::to_string(ntohs(tcp_header.th_win));
    tcp_info +=  " TcpLen: ";
    tcp_info += std::to_string(4*tcp_header.th_off);
    tcp_info += "\n";

    return QString::fromStdString(tcp_info);
}

UDPPacket::UDPPacket(const struct ip& ip_header, const int& num, const struct udphdr& udp_header)
                    : Packet(ip_header, num),
                    udp_header(udp_header){}

QString UDPPacket::get_info() const {
    std::string udp_info =  "UDP ";
    udp_info += std::string(inet_ntoa(ip_header.ip_src));
    udp_info += ":";
    udp_info += std::to_string(ntohs(udp_header.uh_sport));
    udp_info +=  " -> ";
    udp_info += std::string(inet_ntoa(ip_header.ip_dst));
    udp_info += ":";
    udp_info += std::to_string(ntohs(udp_header.uh_dport));
    udp_info += "\n";

    udp_info += get_ip_info().toStdString(); //ToDo: unnecessary conversions

    return QString::fromStdString(udp_info); //ToDo: unnecessary conversions
}

PacketType UDPPacket::get_type() const {
    return PacketType::UDP;
}
QString UDPPacket::get_type_name() const {
    return "UDP";
}

QString UDPPacket::get_packet_spec_info() const {
    return "";
}

QString UDPPacket::get_src_port() const {
    return QString::fromStdString(std::to_string(ntohs(udp_header.uh_sport)));
}
QString UDPPacket::get_dst_port() const {
    return QString::fromStdString(std::to_string(ntohs(udp_header.uh_dport)));
}


ICMPPacket::ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header)
                        : Packet(ip_header, num),
                        icmp_header(icmp_header){}

QString ICMPPacket::get_info() const {
    std::string icmp_info = "ICMP ";
    icmp_info += std::string(inet_ntoa(ip_header.ip_src));
    icmp_info += " -> ";
    icmp_info += std::string(inet_ntoa(ip_header.ip_dst));
    icmp_info += "\n";

    icmp_info += get_ip_info().toStdString();
    icmp_info += get_packet_spec_info().toStdString();

    return QString::fromStdString(icmp_info);
}

PacketType ICMPPacket::get_type() const {
    return PacketType::ICMP;
}
QString ICMPPacket::get_type_name() const {
    return "ICMP";
}

QString ICMPPacket::get_src_port() const {
    return "";
}
QString ICMPPacket::get_dst_port() const {
    return "";
}

QString ICMPPacket::get_packet_spec_info() const {
    std::string icmp_info = "Type:";
    icmp_info += std::to_string(icmp_header.icmp_type);
    icmp_info += "Code:";
    icmp_info += std::to_string(icmp_header.icmp_code);
    icmp_info += "ID:";
    icmp_info += std::to_string(ntohs(icmp_header.icmp_hun.ih_idseq.icd_id));
    icmp_info += "Seq:";
    icmp_info += std::to_string(ntohs(icmp_header.icmp_hun.ih_idseq.icd_seq));
    icmp_info += "\n";

    return QString::fromStdString(icmp_info);
}
