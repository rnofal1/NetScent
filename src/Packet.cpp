/* This file contains definitions for the Packet class and its derived classes
 * Ramsey Nofal, 08/2023
 */


//Local
#include "Packet.h"


Packet::Packet(const struct ip& ip_header, const int& num)
               : ip_header(ip_header),
                time_added(std::time(nullptr)),
                num(num){}

std::string Packet::get_info() {
    return get_ip_info();
}

std::string Packet::get_src_ip() {
    return std::string(inet_ntoa(ip_header.ip_src));
}

std::string Packet::get_dst_ip() {
    return std::string(inet_ntoa(ip_header.ip_dst));
}

std::string Packet::get_time_added() {
    return std::asctime(std::localtime(&time_added));
}

//ToDo: split this into two functions, one for src and one for dst
std::string Packet::get_packet_geo_info() {
    QJsonObject src_json = get_ip_geo_json_info(std::string(inet_ntoa(ip_header.ip_src)));
    QJsonObject dst_json = get_ip_geo_json_info(std::string(inet_ntoa(ip_header.ip_dst)));

    std::string src_info =  "Source Geographical Info:\n" + parse_json(src_json);
    std::string dst_info =  "Destination Geographical Info:\n" + parse_json(dst_json);

    return src_info + dst_info;
}

int Packet::get_num() {
    return num;
}

QJsonObject Packet::string_to_json_obj(const std::string& json_string) {
    QJsonObject json_obj;

    QJsonDocument json_doc = QJsonDocument::fromJson(QString::fromStdString(json_string).toUtf8());

    if(!json_doc.isNull()) {
        if(json_doc.isObject()) {
            json_obj = json_doc.object();
        } else {
            std::cout << "Json doc is not an object\n";
        }
    } else {
        std::cout << "Invalid json\n";
    }

    return json_obj;
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
QJsonObject Packet::get_ip_geo_json_info(const std::string& ip_addr) {
    auto curl = curl_easy_init();
    auto key = get_geoloc_api_key();
    QJsonObject json;

    if(curl && !key.empty()) {
        std::string response_string;
        std::string get_url = "https://api.ipgeolocation.io/ipgeo?apiKey=" + key + "&ip="+ip_addr;

        //Setup curl call
        curl_easy_setopt(curl, CURLOPT_URL, get_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Packet::writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        //Execute curl call
        curl_easy_perform(curl);

        json = string_to_json_obj(response_string);

        curl_easy_cleanup(curl);
        curl = NULL;
    } else if(!curl) {
        std::cout << "Could not initialize curl\n";
    }

    return json;
}

std::string Packet::get_geoloc_api_key() {
    std::ifstream api_key_file(API_KEY_FILE);
    std::string ret_string;

    if(api_key_file) {
        std::getline(api_key_file, ret_string);
    } else {
        std::cout << "Geolocation API key not found\n";
    }
    api_key_file.close();

    return ret_string;
}

//ToDo: consider having this function take in a vector of desired json keys
std::string Packet::parse_json(const QJsonObject& json) {
    std::string json_info = "IP: " +                  get_json_val(json, "ip")
                            + "\nCountry: " +         get_json_val(json, "country_name")
                            + "\nState/Province: " +  get_json_val(json, "state_prov")
                            + "\nCity: " +            get_json_val(json, "city")
                            + "\nOrganization: " +    get_json_val(json, "organization") + "\n\n";
    return json_info;
}

std::string Packet::get_ip_info() {
    std::string ip_info =  "ID:" + std::to_string(ntohs(ip_header.ip_id))
                        + " TOS:0x" + std::to_string(ip_header.ip_tos)
                        + ", TTL:" + std::to_string(ip_header.ip_ttl)
                        + " IpLen:" + std::to_string(4*ip_header.ip_hl)
                        + " DgLen:" + std::to_string(ntohs(ip_header.ip_len))
                        + " Time Added: " + std::asctime(std::localtime(&time_added));
    return ip_info;
}

std::string Packet::get_json_val(const QJsonObject& json, const std::string& key) {
    QString q_key = QString::fromStdString(key);
    if (json.contains(q_key)) {
        return json[q_key].toString().toStdString();
    } else {
        return "Unknown";
    }
}

size_t Packet::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}


TCPPacket::TCPPacket(const struct ip& ip_header, const int& num, const struct tcphdr& tcp_header)
                    : Packet(ip_header, num),
                    tcp_header(tcp_header){}

//ToDo: change this function (and similar functions) to use a formatting function similar to sprintf?
std::string TCPPacket::get_info() {
    std::string tcp_info =  "TCP " + std::string(inet_ntoa(ip_header.ip_src))
                            + ":" + std::to_string(ntohs(tcp_header.th_sport))
                            + " -> " + std::string(inet_ntoa(ip_header.ip_dst))
                            + std::to_string(ntohs(tcp_header.th_dport)) + "\n";

    tcp_info += get_ip_info() + "\n";

    tcp_info += std::string()
                + (tcp_header.th_flags & TH_URG ? 'U' : '*')
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


UDPPacket::UDPPacket(const struct ip& ip_header, const int& num, const struct udphdr& udp_header)
                    : Packet(ip_header, num),
                    udp_header(udp_header){}

std::string UDPPacket::get_info() {
    std::string udp_info =  "UDP " + std::string(inet_ntoa(ip_header.ip_src))
                            + ":" + std::to_string(ntohs(udp_header.uh_sport))
                            +  " -> " + std::string(inet_ntoa(ip_header.ip_dst))
                            + ":" + std::to_string(ntohs(udp_header.uh_dport)) + "\n";

    udp_info += get_ip_info() + "\n";

    return udp_info;
}


ICMPPacket::ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header)
                        : Packet(ip_header, num),
                        icmp_header(icmp_header){}

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
