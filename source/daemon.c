#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/signal.h"
#include "linux/if_packet.h"
#include "net/ethernet.h"
#include "errno.h"
#include "time.h"
#include "netinet/ip.h"
#include "stdlib.h"
#include "string.h"
#include "inttypes.h"

#define BufferSize 64000

int main(int argc, char const *argv[])
{
    struct sockaddr socketAdress;
    int sniffSocket;
	unsigned char *buf;
    ssize_t data;
	socklen_t length = sizeof(socketAdress);
	char * ifname = "eno1";

	printf("Allocating %d bytes for buffer: ", BufferSize);

	buf = (unsigned char *) malloc(BufferSize);

	if (!buf)
    {
		printf("[FAIL]\n");
        return 1;
    }
	else
	{
		printf("[DONE]\n");
	}

	printf("Openning socket: ");
    sniffSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sniffSocket == -1)
    {
        printf("[FAIL]\n");
        return 1;
    }
	else
	{
		printf("[DONE]\n");
	}
 
	printf("Binding to interface %s: ", ifname);
	if (setsockopt(sniffSocket, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)) == 0)
	{
		printf("[DONE]\n");
	}
	else
	{
		printf("[FAIL]\n");
		return 1;
	}
/*
	signal(SIGTERM, sigcb);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	signal(SIGINT, sigcb);
	signal(SIGHUP, sigcb);
*/

	while (1) {

        data = recvfrom(sniffSocket, buf, BufferSize, 0,&socketAdress, &length);
		if (data >= 0)
        {
                struct iphdr * header = (struct iphdr*) (buf + sizeof(struct ethhdr));
                printf("packet from: %d.%d.%d.%d protocol: %d\n", (unsigned char) header->saddr, (unsigned char) (header->saddr >> 8), (unsigned char) (header->saddr >> 16), (unsigned char) (header->saddr >> 24), (unsigned char) header->protocol);
        }		
	}

	free(buf);
	shutdown(sniffSocket, SHUT_RDWR);
	printf("\nFinished.\n");

	return 0;
}
