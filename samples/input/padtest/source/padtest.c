/*
  Original code copyrighted as follows:
  
  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

*/

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <io/pad.h> 
#include <io/mouse.h> 

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
	padInfo padinfo;
	padData paddata;
	
	fprintf(stdout, "Starting Pad Test.\n");
    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
		port = strtol(argv[1], &endptr, 0);
		if ( *endptr ) {
			fprintf(stderr, "ECHOSERV: Invalid port number.\n");
			exit(EXIT_FAILURE);
		}
    }else if ( argc < 2 ) {
		port = ECHO_PORT;
    }else{
		fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
		exit(EXIT_FAILURE);
    }

	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
    }

    char* message = "Welcome to PadTest\r\n";
    
	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling accept()\n");
		exit(EXIT_FAILURE);
	}

	Writeline(conn_s, message, strlen(message));
	
	sprintf(buffer, "Calling ioPadInit(%d) returned %d\r\n", 7, ioPadInit(7));
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "Calling ioPadGetInfo() returned %d\r\n", ioPadGetInfo(&padinfo));
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "PadInfo:\r\nMax Pads: %u\r\nConnected Pads: %u\r\nInfo Field: %08x\r\n", padinfo.max, padinfo.connected, padinfo.info);
	Writeline(conn_s, buffer, strlen(buffer));
	int i;
	for(i=0; i<MAX_PADS; i++){
		if(padinfo.status[i]){
			sprintf(buffer, "Controller %u:\r\nVendor ID: %hx\r\nProduct ID: %hx\r\nStatus: %hhu\r\n", i, padinfo.vendor_id[i], padinfo.product_id[i], padinfo.status[i]);
			Writeline(conn_s, buffer, strlen(buffer));
			
			sprintf(buffer, "Calling ioPadGetData(%d) returned %d\r\n", i, ioPadGetData(i, &paddata));
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "Pad Data:\r\nLength: %d\r\n", paddata.len);
			Writeline(conn_s, buffer, strlen(buffer));
			
			sprintf(buffer, "X..... %d\r\n", paddata.BTN_CROSS);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "O..... %d\r\n", paddata.BTN_CIRCLE);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "/\\.... %d\r\n", paddata.BTN_TRIANGLE);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "[].... %d\r\n", paddata.BTN_SQUARE);
			Writeline(conn_s, buffer, strlen(buffer));
			
			sprintf(buffer, "LEFT.. %d\r\n", paddata.BTN_LEFT);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "RIGHT. %d\r\n", paddata.BTN_RIGHT);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "DOWN.. %d\r\n", paddata.BTN_DOWN);
			Writeline(conn_s, buffer, strlen(buffer));
			sprintf(buffer, "UP.... %d\r\n", paddata.BTN_UP);
			Writeline(conn_s, buffer, strlen(buffer));
			
			padActParam actparam;
			actparam.small_motor = 1;
			actparam.large_motor = 0;
			
			sprintf(buffer, "Vibrating small motor... ");
			Writeline(conn_s, buffer, strlen(buffer));
			
			ioPadSetActDirect(i, &actparam);
			usleep(2000000);
			
			sprintf(buffer, "Finished.\r\n");
			Writeline(conn_s, buffer, strlen(buffer));

			actparam.small_motor = 0;
			actparam.large_motor = 255;
			
			sprintf(buffer, "Vibrating large motor... ");
			Writeline(conn_s, buffer, strlen(buffer));
			
			ioPadSetActDirect(i, &actparam);
			usleep(2000000);
			
			sprintf(buffer, "Finished.\r\n");
			Writeline(conn_s, buffer, strlen(buffer));
			
		}
		
	}
	
	
	
	
	sprintf(buffer, "Finished.\r\n");
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "Calling ioPadEnd() returned %d\r\n", ioPadEnd());
	Writeline(conn_s, buffer, strlen(buffer));

	/*  Close the connected socket  */

	if ( close(conn_s) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling close()\n");
		exit(EXIT_FAILURE);
	}
}

