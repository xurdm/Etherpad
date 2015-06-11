#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr is deprecated and I don't care
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in addr;
//	char *localhost = "127.0.0.1";
	char *localhost = "74.125.226.36";
	char *message, server_reply[2000];
	int recv_size;

//WSAStartup()
	puts("Calling WSAStartup() ...");
	if(WSAStartup(MAKEWORD(2,2), &wsa) != NULL)
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
	printf("Connecting to %s...", localhost);
	if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		puts("Connection failed.");
		return 1;
	}
	puts("Connected.");

//send()
	message = "GET / HTTP/1.1\r\n\r\n";
	if(send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed.");
		return 1;
	}
	puts("Data sent.");

//recv()
	if((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR)
	{
		puts("Receive failed.");
		return 1;
	}
	puts("Reply received:");
	server_reply[recv_size] = '\0';
	puts(server_reply);

	closesocket(s);
	WSACleanup();

	return 0;
}