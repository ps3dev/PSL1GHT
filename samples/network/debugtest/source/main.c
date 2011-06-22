/* 
   Small demonstration of how to use netcat to get debug printing in psl1ght.
   This is for fw 3.55 where ethdebug is not available.

   Usage:
   1. Set the correct ip to your computer below (DEBUG_IP).
   2. Fire up this command in linux / cygwin or equal:
   nc -l -u 18194
   3. Start the app on ps3.

   Don't forget to rerun nc after each run.

   Btw code is 100% stolen from the gemtest sample made by bigboss.
*/
   
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <net/net.h>
#include <netinet/in.h>

static int SocketFD;
#define DEBUG_IP "192.168.40.201"
#define DEBUG_PORT 18194

void debugPrintf(const char* fmt, ...)
{
  char buffer[0x800];
  va_list arg;
  va_start(arg, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, arg);
  va_end(arg);
  netSend(SocketFD, buffer, strlen(buffer), 0);
}

void debugInit()
{
  struct sockaddr_in stSockAddr;
  SocketFD = netSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  memset(&stSockAddr, 0, sizeof stSockAddr);

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(DEBUG_PORT);
  inet_pton(AF_INET, DEBUG_IP, &stSockAddr.sin_addr);

  netConnect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr);
	
  debugPrintf("network debug module initialized\n") ;
  debugPrintf("ready to have a lot of fun\n") ;
}

int main(int argc,char *argv[])
{
  netInitialize();
  debugInit();

  debugPrintf("\nTEST!\nIf you see this you've set up netcat correctly.\n");

  return 0;
}
