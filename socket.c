#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr is deprecated and I don't care because fuck IPv6
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

typedef enum { RESPONSE_OK, RESPONSE_NO, RESPONSE_SOMETHING_WEIRD_HAPPENED } SERVER_MSG;

int main(int argc, char **argv)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in addr;
//	char *localhost = "127.0.0.1";
	char *localhost = "74.125.226.36";
	char *message, response[1024];
	SIZE_T recv_size;
	SERVER_MSG msg;

//WSAStartup()
	puts("Calling WSAStartup() ...");
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		printf(" ...failed. Error code %d", WSAGetLastError());
		return 1;
	}
	puts("WSAStartup() successful.");

//socket()
	if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Failed to create socket: %d", WSAGetLastError());
	}
	puts("Socket created.");

	addr.sin_addr.s_addr = inet_addr(localhost);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);

//connect()
	printf("Connecting to %s...\n", localhost);
	if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		puts("Connection failed.");
		return 1;
	}
	puts("Connected.");

//test send/recv on google
//send()
	message = "GET / HTTP/1.1\r\n\r\n";
	if(send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed.");
		return 1;
	}
	puts("Data sent.");

//recv()
	if((recv_size = recv(s, response, 1024, 0)) == SOCKET_ERROR)
	{
		puts("Receive failed.");
		return 1;
	}
	puts("Reply received:");

	response[recv_size-1] = '\0';
	fprintf(stdout, "%s", response);
	puts(response);

	closesocket(s);
	WSACleanup();

	return 0;
}

int 