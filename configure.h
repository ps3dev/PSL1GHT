// Some compile-time options for PSL1GHT
#pragma once

// Redirect stdio to the network. This 
// overrides the default action of  writing 
// to GameOS's tty via a system call. This 
// does not require Kammy to be run in the PS3.
// Listen to the stdout on your PC with e.g.
//   nc -l -p 4000
//#define _STDIO_TO_NET
// If __STDIO_TO_NET is defined, this is the
// IP and port to which to send the packets
#define _STDIO_NET_IP "10.0.0.3"
#define _STDIO_NET_PORT 4000 

