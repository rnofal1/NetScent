/* This file contains declarations for the Packet class and its derived classes
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETS_H
#define PACKETS_H


//Standard C/C++
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h> //Posix OS API
#include <signal.h>
#include <cstddef>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <filesystem>

//3rd-party
#include <pcap/pcap.h> //libpcap library
#include <netinet/tcp.h> //TCP-specific header fields
#include <netinet/udp.h> //UDP-specific header fields
#include <netinet/ip_icmp.h> //ICMP-specific header fields
#include <nlohmann/json.hpp>
#include <curl/curl.h>


//Defines
#define API_KEY_FILE "api_key.txt"


/* The Packet class stores ip info and provides public methods to access
 * this info
 *
 * Derived classes TCPPacket, UDPPacket, and ICMPPacket store further info
 * specific to TCP, UDP, and ICMP packets, respectively
 */
class Packet {
public:
    Packet(const struct ip& ip_header, const int& num);

    //Return a string containing miscellaneous Packet header information
    virtual std::string get_info();

    std::string get_src_ip();
    std::string get_dst_ip();
    std::string get_time_added();

    //Return a string describing geographical information regarding the sniffed Packet
    std::string get_packet_geo_info();

    int get_num();

protected:
    struct ip ip_header;

    //Describes the time at which the Packet was sniffed
    std::time_t time_added;

    //Describes the order at which the Packet was sniffed, relative to other Packets
    int num;

    nlohmann::json get_ip_geo_json_info(const std::string& ip_addr);

    //Grabs the API key for the external ip geolocation service
    std::string get_geoloc_api_key();

    //Return json info in a formatted string
    std::string parse_json(const nlohmann::json& json);

    //Return a string containing miscellaneous ip header information
    std::string get_ip_info();

    std::string get_json_val(const nlohmann::json& json, const std::string& key);

    //Callback function used during HTTP GET request (curl)
    static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
};


class TCPPacket : public Packet {
public:
    TCPPacket(const struct ip& ip_header, const int& num, const struct tcphdr& tcp_header);

    //Return a string containing miscellaneous TCPPacket header information
    std::string get_info() override;

private:
    struct tcphdr tcp_header;
};


class UDPPacket : public Packet {
public:
    UDPPacket(const struct ip& ip_header, const int& num, const struct udphdr& udp_header);

    //Return a string containing miscellaneous TCPPacket header information
    std::string get_info() override;

private:
    struct udphdr udp_header;
};


class ICMPPacket : public Packet {
public:
    ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header);

    //Return a string containing miscellaneous TCPPacket header information
    std::string get_info() override;

private:
    struct icmp icmp_header;
};


#endif // PACKETS_H
