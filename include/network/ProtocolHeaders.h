/* This file contains declarations for the network protocol headers
 * Goal: mirror netinet protocol structs
 *
 * See: https://npcap.com/guide/npcap-tutorial.html
 * See: https://sites.uclouvain.be/SystInfo/usr/include/netinet/ip.h.html
 * Ramsey Nofal, 08/2023
 */


#ifndef HEADERS_H
#define HEADERS_H


/* Standard C/C++ */
#include <stdint.h>
#include <winsock2.h>
#include <windows.h>


// IPv4 header
struct iphdr
{
    unsigned int ihl:4;
    unsigned int version:4;

    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

// IPv4 expanded header, from: https://www.binarytides.com/code-packet-sniffer-c-winpcap/
struct ip {
    unsigned char ip_hl:4; // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
    unsigned char ip_version :4; // 4-bit IPv4 version
    unsigned char ip_tos; // IP type of service
    unsigned short ip_len; // Total length
    unsigned short ip_id; // Unique identifier

    unsigned char ip_frag_offset :5; // Fragment offset field

    unsigned char ip_more_fragment :1;
    unsigned char ip_dont_fragment :1;
    unsigned char ip_reserved_zero :1;

    unsigned char ip_frag_offset1; //fragment offset

    unsigned char ip_ttl; // Time to live
    unsigned char ip_p; // Protocol(TCP,UDP etc)
    unsigned short ip_checksum; // IP checksum
    struct in_addr ip_src; // Source address
    struct in_addr ip_dst; // Source address
};

// UDP header (from netinet)
struct udphdr {
    uint16_t uh_sport;                // source port
    uint16_t uh_dport;                // destination port
    uint16_t uh_ulen;                // udp length
    uint16_t uh_sum;                // udp checksum
};

// TCP header (from netinet)
struct tcphdr
{
    uint16_t th_sport;                // source port
    uint16_t th_dport;                // destination port
    uint32_t  th_seq;                // sequence number
    uint32_t th_ack;                // acknowledgement number
    uint8_t th_x2:4;                // (unused)
    uint8_t th_off:4;                // data offset

    uint8_t th_flags;
#  define TH_FIN        0x01
#  define TH_SYN        0x02
#  define TH_RST        0x04
#  define TH_PUSH        0x08
#  define TH_ACK        0x10
#  define TH_URG        0x20
    uint16_t th_win;                // window
    uint16_t th_sum;                // checksum
    uint16_t th_urp;                // urgent pointer
};

// Internal of an ICMP Router Advertisement (from netinet.h)
struct icmp_ra_addr
{
    uint32_t ira_addr;
    uint32_t ira_preference;
};

// ICMP header (from netinet.h)
struct icmp
{
    uint8_t  icmp_type;        // type of message, see below
    uint8_t  icmp_code;        // type sub code
    uint16_t icmp_cksum;        // ones complement checksum of struct
    union
    {
        unsigned char ih_pptr;                // ICMP_PARAMPROB
        struct in_addr ih_gwaddr;        // gateway address
        struct ih_idseq                // echo datagram
        {
            uint16_t icd_id;
            uint16_t icd_seq;
        } ih_idseq;
        uint32_t ih_void;

        // ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191)
        struct ih_pmtu
        {
            uint16_t ipm_void;
            uint16_t ipm_nextmtu;
        } ih_pmtu;

        struct ih_rtradv
        {
            uint8_t irt_num_addrs;
            uint8_t irt_wpa;
            uint16_t irt_lifetime;
        } ih_rtradv;
    } icmp_hun;
#define        icmp_pptr        icmp_hun.ih_pptr
#define        icmp_gwaddr        icmp_hun.ih_gwaddr
#define        icmp_id                icmp_hun.ih_idseq.icd_id
#define        icmp_seq        icmp_hun.ih_idseq.icd_seq
#define        icmp_void        icmp_hun.ih_void
#define        icmp_pmvoid        icmp_hun.ih_pmtu.ipm_void
#define        icmp_nextmtu        icmp_hun.ih_pmtu.ipm_nextmtu
#define        icmp_num_addrs        icmp_hun.ih_rtradv.irt_num_addrs
#define        icmp_wpa        icmp_hun.ih_rtradv.irt_wpa
#define        icmp_lifetime        icmp_hun.ih_rtradv.irt_lifetime
    union
    {
        struct
        {
            uint32_t its_otime;
            uint32_t its_rtime;
            uint32_t its_ttime;
        } id_ts;
        struct
        {
            struct ip idi_ip;
            // options and then 64 bits of data
        } id_ip;
        struct icmp_ra_addr id_radv;
        uint32_t   id_mask;
        uint8_t    id_data[1];
    } icmp_dun;
#define        icmp_otime        icmp_dun.id_ts.its_otime
#define        icmp_rtime        icmp_dun.id_ts.its_rtime
#define        icmp_ttime        icmp_dun.id_ts.its_ttime
#define        icmp_ip                icmp_dun.id_ip.idi_ip
#define        icmp_radv        icmp_dun.id_radv
#define        icmp_mask        icmp_dun.id_mask
#define        icmp_data        icmp_dun.id_data
};

#endif // HEADERS_H
