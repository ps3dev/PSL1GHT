#ifndef __IN_H__
#define __IN_H__

#include <socket.h>
#include <ppu-types.h>

#define IPPROTO_IP           0x00
#define IPPROTO_ICMP         0x01
#define IPPROTO_TCP          0x06
#define IPPROTO_UDP          0x11
#define IPPROTO_ICMPV6       0x3A

#define IN_CLASSA(i)         (((u32)(i) & 0x80000000) == 0)
#define IN_CLASSA_NET        0xff000000
#define IN_CLASSA_NSHIFT     24
#define IN_CLASSA_HOST       0x00ffffff
#define IN_CLASSA_MAX        128

#define IN_CLASSB(i)         (((u32)(i) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET        0xffff0000
#define IN_CLASSB_NSHIFT     16
#define IN_CLASSB_HOST       0x0000ffff
#define IN_CLASSB_MAX        65536

#define IN_CLASSC(i)         (((u32)(i) & 0xe0000000) == 0xc0000000)
#define IN_CLASSC_NET        0xffffff00
#define IN_CLASSC_NSHIFT     8
#define IN_CLASSC_HOST       0x000000ff

#define IN_CLASSD(i)         (((u32)(i) & 0xf0000000) == 0xe0000000)
#define IN_MULTICAST(i)      IN_CLASSD(i)

#define INADDR_ANY           0x00000000
#define INADDR_LOOPBACK      0x7F000001
#define INADDR_BROADCAST     0xFFFFFFFF
#define INADDR_NONE          0xFFFFFFFF

#define IN_LOOPBACKNET       127

#define INET_ADDRSTRLEN      16

#define IP_OPTIONS           1 
#define IP_HDRINCL           2
#define IP_TOS               3
#define IP_TTL               4
#define IP_RECVOPTS          5
#define IP_RECVRETOPTS       6
#define IP_RECVDSTADDR       7
#define IP_RETOPTS           8
#define IP_MULTICAST_IF      9
#define IP_MULTICAST_TTL     10
#define IP_MULTICAST_LOOP    11
#define IP_ADD_MEMBERSHIP    12
#define IP_DROP_MEMBERSHIP   13


typedef u16 in_port_t;
typedef u32 in_addr_t;

struct in_addr
{
        in_addr_t s_addr;
};

struct sockaddr_in
{
        u8 sin_len;
        sa_family_t sin_family;
        in_port_t sin_port;
        struct in_addr sin_addr;
        u8 sin_zero[0x08];
};

struct sockaddr_in_p2p
{
        u8 sin_len;
        sa_family_t sin_family;
        in_port_t sin_port;
        struct in_addr sin_addr;
        in_port_t sin_vport;
        u8 sin_zero[6];
};

struct ip_mreq
{
        struct in_addr imr_multiaddr;
        struct in_addr imr_interface;
};

#endif
