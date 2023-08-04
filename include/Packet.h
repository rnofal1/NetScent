#ifndef PACKETS_H
#define PACKETS_H

//Standard
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h> //Posix OS API
#include <signal.h>
#include <cstddef>
#include <ctime>
#include <cstdlib>
#include <curl/curl.h>
#include <iostream>

//3rd-party
#include <pcap/pcap.h> //libpcap library
#include <netinet/tcp.h> //TCP-specific header fields
#include <netinet/udp.h> //UDP-specific header fields
#include <netinet/ip_icmp.h> //ICMP-specific header fields
#include <nlohmann/json.hpp>

class Packet {
public:
    Packet(const struct ip& ip_header, const int& num);
    virtual std::string get_info();
    std::string get_src_ip();
    std::string get_dst_ip();
    std::string get_time_added();
    std::string get_packet_geo_info();
    int get_num();
    //virtual void print();

protected:
    static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
    std::string get_geoloc_api_key();
    std::string parse_json(const nlohmann::json& json);
    nlohmann::json get_ip_geo_json_info(const std::string& ip_addr);
    std::time_t time_added;
    std::string get_ip_info();
    std::string get_json_val(const nlohmann::json& json, const std::string& key);
    struct ip ip_header;
    int num;
};


class TCPPacket : public Packet {
public:
    TCPPacket(const struct ip& ip_header, const int& num, const struct tcphdr& tcp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct tcphdr tcp_header;
};


class UDPPacket : public Packet {
public:
    UDPPacket(const struct ip& ip_header, const int& num, const struct udphdr& udp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct udphdr udp_header;
};


class ICMPPacket : public Packet {
public:
    ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct icmp icmp_header;
};


#endif // PACKETS_H
