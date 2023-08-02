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

//3rd-party
#include <pcap/pcap.h> //libpcap library
#include <netinet/tcp.h> //TCP-specific header fields
#include <netinet/udp.h> //UDP-specific header fields
#include <netinet/ip_icmp.h> //ICMP-specific header fields

class Packet {
public:
    Packet(const struct ip& ip_header);
    virtual std::string get_info();
    //virtual void print();

protected:
    std::time_t time_added;
    std::string get_ip_info();
    struct ip ip_header;
};


class TCPPacket : public Packet {
public:
    TCPPacket(const struct ip& ip_header, const struct tcphdr& tcp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct tcphdr tcp_header;
};


class UDPPacket : public Packet {
public:
    UDPPacket(const struct ip& ip_header, const struct udphdr& udp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct udphdr udp_header;
};


class ICMPPacket : public Packet {
public:
    ICMPPacket(const struct ip& ip_header, const struct icmp& icmp_header);
    std::string get_info() override;
    //void print() override;

private:
    struct icmp icmp_header;
};


#endif // PACKETS_H
