/*
  Original code copyrighted as follows:
  
  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

  Modified by TitanMKD to do basic Keyboard API tests.
  
  Use socat to have debug output
  http://www.dest-unreach.org/socat/
  configuration is for example for PS3 IP=10.1.1.1
  socat - TCP:10.1.1.1:2002
  End of test is a while loop which echo keyboard keys and waits Pad "X" Button to exit.
*/

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <io/kb.h> 
#include <io/pad.h>

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*  Global constants  */
#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)

char buffer[MAX_LINE];	/* character buffer */
int conn_s;				/* connection socket */

static u32 receive_exit_request = 0;

int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char     *endptr;                /*  for strtol()              */
	CellKbInfo kbinfo;
	CellKbConfig kbconfig;
	CellKbData kbdata;
	int i;
	
	fprintf(stdout, "Starting Kb Test.\n");
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

    char* message = "Welcome to KbTest\r\n";
    
	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling accept()\n");
		exit(EXIT_FAILURE);
	}

	Writeline(conn_s, message, strlen(message));
	
	/* Keyboard cellKbInit()/ioKbGetInfo() Test */
	sprintf(buffer, "Calling cellKbInit(%d) returned %d\r\n", CELL_MAX_KB_PORT_NUM, cellKbInit(CELL_MAX_KB_PORT_NUM));
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "Calling cellKbGetInfo() returned %d\r\n", cellKbGetInfo(&kbinfo));
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "KbInfo:\r\nMax Kbs: %u\r\nConnected Kbs: %u\r\nInfo Field: %08x\r\n", kbinfo.max, kbinfo.connected, kbinfo.info);
	Writeline(conn_s, buffer, strlen(buffer));
	for(i=0; i<CELL_MAX_KEYBOARDS; i++)
	{
		if(kbinfo.status[i])
		{
			/* Keyboard cellKbSetLEDStatus() Test */
			{
				CellKbLed led_state;
				/* num_lock=ON, caps_lock=OFF, scroll_lock=OFF, compose=OFF, kana=OFF*/
				led_state.leds = 0;
				led_state.num_lock = 1;
				sprintf(buffer, "num_lock=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);
				/* num_lock=OFF, caps_lock=ON, scroll_lock=OFF, compose=OFF, kana=OFF*/
				led_state.leds = 0;
				led_state.caps_lock = 1;
				sprintf(buffer, "caps_lock=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);
				/* num_lock=OFF, caps_lock=OFF, scroll_lock=ON, compose=OFF, kana=OFF*/
				led_state.leds = 0;
				led_state.scroll_lock = 1;
				sprintf(buffer, "scroll_lock=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);		
				/* num_lock=OFF, caps_lock=OFF, scroll_lock=OFF, compose=ON, kana=OFF*/
				led_state.leds = 0;
				led_state.compose = 1;
				sprintf(buffer, "compose=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);
				/* num_lock=OFF, caps_lock=OFF, scroll_lock=OFF, compose=OFF, kana=ON*/
				led_state.leds = 0;
				led_state.kana = 1;
				sprintf(buffer, "kana=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);
				/* All Kb Leds=OFF */
				led_state.leds = 0;
				sprintf(buffer, "All Kb Leds=OFF Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);	
				/* All Kb Leds=ON */
				led_state.leds = 0xFFFFFFFF;
				led_state.reserved = 0;
				sprintf(buffer, "All Kb Leds=ON Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);
				/* All Kb Leds=OFF */
				led_state.leds = 0;
				sprintf(buffer, "All Kb Leds=OFF Calling cellKbSetLEDStatus(%d, 0x%08X) returned %d\r\n", i, led_state.leds, cellKbSetLEDStatus(i, led_state));
				Writeline(conn_s, buffer, strlen(buffer));
				usleep(500000);

				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
			}		
		
			/* Keyboard cellKbRead() Info Test */
			{
				sprintf(buffer, "Keyboard %u:\r\nStatus: %hhu\r\n", i, kbinfo.status[i]);
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Calling cellKbRead(%d) returned %d\r\n", i, cellKbRead(i, &kbdata));
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "Kb Data:\r\nnb_keycode: %d\r\n", kbdata.nb_keycode);
				Writeline(conn_s, buffer, strlen(buffer));
				for(int j=0; j<kbdata.nb_keycode;j++)
				{
					sprintf(buffer, "keycode[%d]..... %d\r\n", j, kbdata.keycode[j]);
					Writeline(conn_s, buffer, strlen(buffer));
				}
				
				sprintf(buffer, "led.num_lock..... %d\r\n", kbdata.led.num_lock);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "led.caps_lock..... %d\r\n", kbdata.led.caps_lock);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "led.scroll_lock..... %d\r\n", kbdata.led.scroll_lock);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "led.compose..... %d\r\n", kbdata.led.compose);
				Writeline(conn_s, buffer, strlen(buffer));			
				sprintf(buffer, "led.kana..... %d\r\n", kbdata.led.compose);
				Writeline(conn_s, buffer, strlen(buffer));			
				
				sprintf(buffer, "mkey.l_ctrl..... %d\r\n", kbdata.mkey.l_ctrl);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.l_shift..... %d\r\n", kbdata.mkey.l_shift);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.l_alt..... %d\r\n", kbdata.mkey.l_alt);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.l_win..... %d\r\n", kbdata.mkey.l_win);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.r_ctrl..... %d\r\n", kbdata.mkey.r_ctrl);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.r_shift..... %d\r\n", kbdata.mkey.r_shift);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.r_alt..... %d\r\n", kbdata.mkey.r_alt);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "mkey.r_win..... %d\r\n", kbdata.mkey.r_win);
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
			}
			
			/* Keyboard Configuration cellKbGetConfiguration() Test */
			{	
				sprintf(buffer, "Calling cellKbGetConfiguration(%d) returned %d\r\n", i, cellKbGetConfiguration(i, &kbconfig));
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "kbconfig.mapping..... %d\r\n", kbconfig.mapping);
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "kbconfig.rmode..... %d\r\n", kbconfig.rmode);
				Writeline(conn_s, buffer, strlen(buffer));				
				sprintf(buffer, "kbconfig.codetype..... %d\r\n", kbconfig.codetype);
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
			}

			/* Keyboard cellKbSetReadMode()/ioKbSetCodeType()/ioKbRead() Test */
			{		
				sprintf(buffer, "Calling cellKbSetReadMode(%d, CELL_KB_RMODE_INPUTCHAR) returned %d\r\n", i, cellKbSetReadMode(i, CELL_KB_RMODE_INPUTCHAR));
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Calling cellKbSetCodeType(%d, CELL_KB_CODETYPE_ASCII) returned %d\r\n", i, cellKbSetCodeType(i, CELL_KB_CODETYPE_ASCII));
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Calling cellKbRead(%d) returned %d\r\n", i, cellKbRead(i, &kbdata));
				Writeline(conn_s, buffer, strlen(buffer));
				sprintf(buffer, "Kb Data:\r\nnb_keycode: %d\r\n", kbdata.nb_keycode);
				Writeline(conn_s, buffer, strlen(buffer));
				for(int j=0; j<kbdata.nb_keycode;j++)
				{
					sprintf(buffer, "keycode[%d]..... %d\r\n", j, kbdata.keycode[j]);
					Writeline(conn_s, buffer, strlen(buffer));
				}
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));
			}
			
			/* Keyboard cellKbCnvRawCode() Test */
			{
				CellKbMapping mapping;
				CellKbMkey mkey;
				CellKbLed led;
				u16 rawcode;
				
				/* Use previous data */
				mapping = kbconfig.mapping;
				mkey.mkeys = kbdata.mkey.mkeys;
				led.leds = kbdata.led.leds;
				for(int j=0; j<kbdata.nb_keycode ;j++)
				{
					rawcode = kbdata.keycode[j];
					sprintf(buffer, "Calling cellKbCnvRawCode(%d, 0x%08X, 0x%08X, 0x%04X) returned 0x%04X\r\n", mapping, mkey.mkeys, led.leds, rawcode,
								cellKbCnvRawCode(mapping, mkey, led, rawcode));
					Writeline(conn_s, buffer, strlen(buffer));
				}
			}
			
			/* Keyboard cellKbClearBuf() Test */
			{
				sprintf(buffer, "Calling cellKbClearBuf(%d) returned %d\r\n", i, cellKbClearBuf(i));
				Writeline(conn_s, buffer, strlen(buffer));
				
				sprintf(buffer, "Finished.\r\n");
				Writeline(conn_s, buffer, strlen(buffer));				
			}
		}
	}

	/* Keyboard cellKbEnd() Test */
	sprintf(buffer, "Calling cellKbEnd() returned %d\r\n", cellKbEnd());
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "Finished.\r\n");
	Writeline(conn_s, buffer, strlen(buffer));

	sprintf(buffer, "Start infinite loop Keyboard key output, stop it with Pad \"X\" Button.\r\n");
	Writeline(conn_s, buffer, strlen(buffer));
	
	cellPadInit(CELL_MAX_PORT_NUM);
	cellKbInit(CELL_MAX_KB_PORT_NUM);
	
	sprintf(buffer, "Calling cellKbGetInfo() returned %d\r\n", cellKbGetInfo(&kbinfo));
	Writeline(conn_s, buffer, strlen(buffer));
	
	sprintf(buffer, "KbInfo:\r\nMax Kbs: %u\r\nConnected Kbs: %u\r\nInfo Field: %08x\r\n", kbinfo.max, kbinfo.connected, kbinfo.info);
	Writeline(conn_s, buffer, strlen(buffer));	

	CellPadInfo2 padinfo2;
	CellPadData paddata;
	s32 status;

	while(receive_exit_request == 0)
	{
		/* Check the pads. */
		if( cellPadGetInfo2(&padinfo2) == 0)
		{
			for(i=0; i<CELL_MAX_PORT_NUM; i++)
			{
				if(padinfo2.port_status[i])
				{
					cellPadGetData(i, &paddata);
					if(paddata.BTN_CROSS){
						sprintf(buffer, "PadButton \"X\" Exit.\r\n");
						Writeline(conn_s, buffer, strlen(buffer));
						receive_exit_request = 1;					
					}
				}		
			}
		}
		/* Check Keyboards and output keys */
		if( cellKbGetInfo(&kbinfo) == 0)
		{
			for(i=0; i<CELL_MAX_KEYBOARDS; i++)
			{
				if(kbinfo.status[i])
				{
					status = cellKbRead(i, &kbdata);
					if(status != 0)
					{
						sprintf(buffer, "Calling cellKbRead(%d) returned %d\r\n", i, cellKbRead(i, &kbdata));
						Writeline(conn_s, buffer, strlen(buffer));
					}
					for(int j=0; j<kbdata.nb_keycode;j++)
					{
						u16 keycode;
						CellKbMkey mkey;
						CellKbLed led;
						keycode = kbdata.keycode[j];
						mkey.mkeys = kbdata.mkey.mkeys;
						led.leds = kbdata.led.leds;
						if(keycode == 0)
						{
							sprintf(buffer, "keycode[%d]=%d/0x%04X mkeys=0x%08X leds=0x%08X\r\n",
									j, keycode, keycode, mkey.mkeys, led.leds);
							Writeline(conn_s, buffer, strlen(buffer));
						}else
						{
							sprintf(buffer, "keycode[%d]=%d/0x%04X=%c mkeys=0x%08X leds=0x%08X\r\n",
									j, keycode, keycode, (keycode&0x00FF), mkey.mkeys, led.leds);
							Writeline(conn_s, buffer, strlen(buffer));
						}
					}
				}
			}
		}
		usleep(20000);
	}

	sprintf(buffer, "Finished.\r\n");
	Writeline(conn_s, buffer, strlen(buffer));

	cellPadEnd();
	cellKbEnd();
	
	sprintf(buffer, "Exit.\r\n");
	Writeline(conn_s, buffer, strlen(buffer));	

	/*  Close the connected socket  */
	if ( close(conn_s) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling close()\n");
		exit(EXIT_FAILURE);
	}
}

