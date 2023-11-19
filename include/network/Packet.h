/* This file contains declarations for the Packet class and its derived classes
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETS_H
#define PACKETS_H


//Standard C/C++
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <signal.h>
#include <cstddef>
#include <ctime>
#include <cstdlib>
#include <iostream>

//Standard Qt
#include <QString>

//3rd-party
#include <pcap/pcap.h> //libpcap library
#include <nlohmann/json.hpp>

//Local
#include "ProtocolHeaders.h"


//ip_coords struct used in get_ip_coords() and associated functions
struct IPCoords {
    bool known = false;
    float latitude;
    float longitude;
};

enum PacketType {IP, TCP, UDP, ICMP};

/* The Packet class stores ip info and provides public methods to access
 * this info
 *
 * Derived classes TCPPacket, UDPPacket, and ICMPPacket store further info
 * specific to TCP, UDP, and ICMP packets, respectively
 */
class Packet {
public:
    Packet(const struct ip& ip_header, const int& num);
    virtual ~Packet();

    //Return a string containing miscellaneous Packet header information
    virtual QString get_info() const;

    virtual PacketType get_type() const;
    virtual QString get_type_name() const;
    virtual QString get_packet_spec_info() const; //Packet specific info


    //Return a string containing miscellaneous ip header information
    QString get_ip_info() const;

    QString get_src_ip() const;
    virtual QString get_src_port() const;

    QString get_dst_ip() const;
    virtual QString get_dst_port() const;

    QString get_time_added() const;

    //Return a string describing geographical information regarding the sniffed Packet
    std::string get_geo_info();

    std::string get_dst_geo_info() const;
    std::string get_src_geo_info() const;

    IPCoords get_dst_ip_coords() const;
    IPCoords get_src_ip_coords() const;

    int get_num() const;

protected:
    struct ip ip_header;

    //Describes the time at which the Packet was sniffed
    std::time_t time_added;

    //Storting geo_info minimizes API calls
    std::string geo_info;

    //Describes the order at which the Packet was sniffed, relative to other Packets
    int num;

    nlohmann::json get_ip_geo_json_info(const std::string& ip_addr) const;

    IPCoords get_ip_coords(const std::string& ip_addr) const;

    std::string parse_geo_info_json(const nlohmann::json& json) const;
};


class TCPPacket : public Packet {
public:
    TCPPacket(const struct ip& ip_header, const int& num, const struct tcphdr& tcp_header);

    //Return a string containing miscellaneous TCPPacket header information
    QString get_info() const override;

    PacketType get_type() const override;
    QString get_type_name() const override;
    QString get_packet_spec_info() const override; //Packet specific info
    QString get_src_port() const override;
    QString get_dst_port() const override;

private:
    struct tcphdr tcp_header;
};


class UDPPacket : public Packet {
public:
    UDPPacket(const struct ip& ip_header, const int& num, const struct udphdr& udp_header);

    //Return a string containing miscellaneous UDPPacket header information
    QString get_info() const override;

    PacketType get_type() const override;
    QString get_type_name() const override;
    QString get_packet_spec_info() const override; //Packet specific info
    QString get_src_port() const override;
    QString get_dst_port() const override;

private:
    struct udphdr udp_header;
};


class ICMPPacket : public Packet {
public:
    ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header);

    //Return a string containing miscellaneous ICMPPacket header information
    QString get_info() const override;

    PacketType get_type() const override;
    QString get_type_name() const override;
    QString get_packet_spec_info() const override; //Packet specific info
    QString get_src_port() const override;
    QString get_dst_port() const override;

private:
    struct icmp icmp_header;
};

#endif // PACKETS_H
