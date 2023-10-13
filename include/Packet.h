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


//3rd-party
#include <pcap/pcap.h> //libpcap library

//Local
#include "ProtocolHeaders.h"

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
    virtual std::string get_info();

    std::string get_src_ip();
    std::string get_dst_ip();
    std::string get_time_added();

    int get_num();

protected:
    struct ip ip_header;

    //Describes the time at which the Packet was sniffed
    std::time_t time_added;

    //Describes the order at which the Packet was sniffed, relative to other Packets
    int num;

    //Return a string containing miscellaneous ip header information
    std::string get_ip_info();
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

    //Return a string containing miscellaneous UDPPacket header information
    std::string get_info() override;

private:
    struct udphdr udp_header;
};


class ICMPPacket : public Packet {
public:
    ICMPPacket(const struct ip& ip_header, const int& num, const struct icmp& icmp_header);

    //Return a string containing miscellaneous ICMPPacket header information
    std::string get_info() override;

private:
    struct icmp icmp_header;
};


#endif // PACKETS_H
