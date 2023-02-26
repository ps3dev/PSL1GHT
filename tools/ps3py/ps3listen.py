#!/usr/bin/env python3
import socket
UDP_IP=''
UDP_PORT=18194

sock = socket.socket( socket.AF_INET,  socket.SOCK_DGRAM )
sock.bind( (UDP_IP,UDP_PORT) )

while True:
	data, addr = sock.recvfrom( 1024 ) # buffer size is 1024 bytes
	print(data)
