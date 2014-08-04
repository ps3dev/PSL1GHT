/* This sample application will connect to a specified IP using the PS3's
 * network and send a small message. It will try sending it with both
 * TCP and UDP.
 */

#include <psl1ght/lv2/net.h>

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Configure these (especially the IP) to your own setup.
 * Use netcat to receive the results on your PC:
 * TCP: nc -l -p 4000
 * UDP: nc -u -l -p 4000
 * For some versions of netcat the -p option may need to be removed.
 */

#define TESTSTRING	"y halo thar\n"
#define TESTIP		"192.168.1.13"
#define TESTPORT	4000

void tcp_test()
{
	printf("Beginning TCP socket test...\n");

	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) {
		printf("Unable to create a socket: %d\n", errno);
		return;
	}
	printf("Socket created: %d\n", s);

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	inet_pton(AF_INET, TESTIP, &server.sin_addr);
	server.sin_port = htons(TESTPORT);

	int ret = connect(s, (struct sockaddr*)&server, sizeof(server));
	if (ret) {
		printf("Unable to connect to server: %d\n", errno);
		return;
	}
	printf("Connected to server successfully!\n");
	ret = write(s, TESTSTRING, strlen(TESTSTRING));
	if (ret < 0)
		printf("Unable to send to server: %d\n", errno);
	else
		printf("Sent %d bytes to server!\n", ret);

	ret = shutdown(s, SHUT_RDWR);
	if (ret < 0)
		printf("Unable to shutdown socket: %d\n", errno);
	else
		printf("Socket shutdown successfully!\n");

	ret = close(s);
	if (ret < 0)
		printf("Unable to close socket: %d\n", ret);
	else
		printf("TCP test successful!\n");
}

void udp_test()
{
	printf("Beginning UDP socket test...\n");

	int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0) {
		printf("Unable to create a socket: %d\n", errno);
		return;
	}
	printf("Socket created: %d\n", s);

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	inet_pton(AF_INET, TESTIP, &server.sin_addr);
	server.sin_port = htons(TESTPORT);

	int ret = sendto(s, TESTSTRING, strlen(TESTSTRING), 0, (struct sockaddr*)&server, sizeof(server));
	if (ret < 0)
		printf("Unable to send to server: %d\n", errno);
	else
		printf("Sent %d bytes to server!\n", ret);

	ret = close(s);
	if (ret < 0)
		printf("Unable to close socket: %d\n", ret);
	else
		printf("UDP test successful!\n");

}

int main(int argc, const char* argv[])
{
	printf("Initializing network... %d\n", netInitialize());
	tcp_test();
	udp_test();
	netDeinitialize();
	return 0;
}
