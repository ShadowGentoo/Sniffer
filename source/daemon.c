#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/un.h"
#include "sys/signal.h"
#include "linux/if_packet.h"
#include "net/ethernet.h"
#include "netinet/ip.h"
#include "stdlib.h"
#include "string.h"
#include "inttypes.h"
#include "../include/stat.h"
#include "unistd.h"

/* max size 65535, but ethernet usually use MTU =1500 */
#define BufferSize 1500
int run = 1;

void interruptionHandler(int signalNumber)
{
	printf("Get signal #%d\n", signalNumber);
	run = 0;
}

void parseCommand(unsigned char * message, const struct interfaceStat * iStat)
{
	printf("Received from UNIX_socket: %s", message);
}

void parsePacket(unsigned char * packet, struct interfaceStat * iStat)
{

	struct iphdr * header = (struct iphdr*) (packet + sizeof(struct ethhdr));

	printf("packet from: %d.%d.%d.%d protocol: %d\n", (unsigned char) header->saddr, (unsigned char) (header->saddr >> 8), 
		(unsigned char) (header->saddr >> 16), (unsigned char) (header->saddr >> 24), (unsigned char) header->protocol);

	addToStat(iStat, header->saddr);
}

int main(int argc, char const *argv[])
{
    struct sockaddr socketAdress;
    int sniffSocket, cliSocket;
	unsigned char *buf;
    ssize_t data;
	socklen_t length = sizeof(socketAdress);
	char * ifname = NULL;

	if (argc == 2)
	{
		ifname = (char *) malloc(strlen(argv[1]));
		strcpy(ifname, argv[1]);
	}
	else
	{
		ifname = "eno1";
	}
	

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

	printf("Openning sockets: ");
    sniffSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	cliSocket = socket(AF_UNIX, SOCK_DGRAM, 0);

    if ((sniffSocket == -1) || (cliSocket == -1))
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


	struct sockaddr_un un_addr;
	memset(&un_addr, 0, sizeof(un_addr));
	un_addr.sun_family = AF_UNIX;
	strncpy(un_addr.sun_path, "/tmp/sniffd_socket", sizeof(un_addr.sun_path)-1);
	
	printf("Binding UNIX_socket to /tmp/sniffd: ");
	if (bind(cliSocket, (struct sockaddr*)&un_addr, sizeof(un_addr)) == -1)
	{
		printf("[FAIL]\n");
		return 1;
	}
	else
	{
		printf("[DONE]\n");
	}
	
	signal(SIGTERM, interruptionHandler);
	signal(SIGINT, interruptionHandler);

	struct interfaceStat iStat;
	iStat.ipCount = 0;
	iStat.statistics = NULL;

	char * filename = malloc(strlen(ifname) + 4);
	strcat(filename, "./");
	strcat(filename, ifname);
	readStat(&iStat, ifname);

	while (run) {

        data = recvfrom(sniffSocket, buf, BufferSize, MSG_DONTWAIT,&socketAdress, &length);
		if (data >= 0)
        {
			parsePacket(buf, &iStat);
        }

		data = recv(cliSocket, buf, BufferSize, MSG_DONTWAIT);
		if (data >= 0)
		{
			parseCommand(buf, &iStat);
		}
	}

	writeStat(&iStat, ifname);
	free(filename);
	free(buf);
	unlink("/tmp/sniffd_socket");
	shutdown(sniffSocket, SHUT_RDWR);
	printf("\nFinished.\n");

	return 0;
}
