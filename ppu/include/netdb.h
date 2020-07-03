#ifndef __NETDB_H__
#define __NETDB_H__

#include <socket.h>

#define NETDB_INTERNAL		-1
#define NETDB_SUCCESS		0x00
#define HOST_NOT_FOUND		0x01
#define TRY_AGAIN			0x02
#define NO_RECOVERY			0x03
#define NO_DATA				0x04
#define NO_ADDRESS			NO_DATA

#define NI_MAXHOST      1025
#define NI_MAXSERV      32

#define NI_NUMERICHOST 1       /* Don't try to look up hostname.  */
#define NI_NUMERICSERV 2       /* Don't convert port number to name.  */
#define NI_NOFQDN      4       /* Only return nodename portion.  */
#define NI_NAMEREQD    8       /* Don't return numeric addresses.  */
#define NI_DGRAM       16      /* Look up UDP service rather than TCP.  */
#define NI_IDN         32      /* Convert name from IDN format.  */
#define NI_IDN_ALLOW_UNASSIGNED 64 /* Don't reject unassigned Unicode
                                        code points.  */
#define NI_IDN_USE_STD3_ASCII_RULES 128 /* Validate strings according to
                                             STD3 rules.  */
/* Possible values for `ai_flags' field in `addrinfo' structure.  */
#define AI_PASSIVE     0x0001  /* Socket address is intended for `bind'.  */
#define AI_CANONNAME   0x0002  /* Request for canonical name.  */
#define AI_NUMERICHOST 0x0004  /* Don't use name resolution.  */
#define AI_V4MAPPED    0x0008  /* IPv4 mapped addresses are acceptable.  */
#define AI_ALL         0x0010  /* Return IPv4 mapped and IPv6 addresses.  */
#define AI_ADDRCONFIG  0x0020  /* Use configuration of this host to choose
                                   returned address type..  */
#define AI_IDN        0x0040  /* IDN encode input (assuming it is encoded
                                   in the current locale's character set)
                                   before looking it up. */
#define AI_CANONIDN   0x0080  /* Translate canonical name from IDN format. */
#define AI_IDN_ALLOW_UNASSIGNED 0x0100 /* Don't reject unassigned Unicode
                                            code points.  */
#define AI_IDN_USE_STD3_ASCII_RULES 0x0200 /* Validate strings according to
                                                STD3 rules.  */

extern int h_errno;

struct hostent
{
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
	#define h_addr h_addr_list[0]
};


struct servent
{
  char *s_name;                 /* Official service name.  */
  char **s_aliases;             /* Alias list.  */
  int s_port;                   /* Port number.  */
  char *s_proto;                /* Protocol to use.  */
};

/* Structure to contain information about address of a service provider.  */
struct addrinfo
{
  int ai_flags;                 /* Input flags.  */
  int ai_family;                /* Protocol family for socket.  */
  int ai_socktype;              /* Socket type.  */
  int ai_protocol;              /* Protocol for socket.  */
  socklen_t ai_addrlen;         /* Length of socket address.  */
  struct sockaddr *ai_addr;     /* Socket address for socket.  */
  char *ai_canonname;           /* Canonical name for service location.  */
  struct addrinfo *ai_next;     /* Pointer to next in list.  */
};

#ifdef __cplusplus
extern "C" {
#endif

struct hostent* gethostbyaddr(const char *addr,socklen_t len,int tpye);
struct hostent* gethostbyname(const char *name);
struct servent *getservbyport(int port, const char *proto);

#ifdef __cplusplus
	}
#endif

#endif
