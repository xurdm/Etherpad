#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr is deprecated and I don't care because fuck IPv6
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

typedef enum { RESPONSE_OK, RESPONSE_NO, RESPONSE_SOMETHING_WEIRD_HAPPENED } SERVER_MSG;

int _not_main(int argc, char **argv)
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
//	char *localhost = "127.0.0.1";
	char *localhost = "74.125.226.36";
	char *message, response[1024];
	SIZE_T recv_size;
	SERVER_MSG msg;
	SIZE_T c;
	FILE *fstdout;

//	freopen_s(&fstdout, "console.log", "w", stdout);	//redirect stdout to log file. TODO: error check

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

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(3028);

	/*** server ***/
	if(bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed. Error code: %d", WSAGetLastError());
		return 1;
	}
	puts("Bind done.");

	listen(s, 3);
	puts("Waiting for incoming connections...");
	
	c = sizeof(struct sockaddr_in);

	if((new_socket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET)
	{
		puts("Connection accepted");
		message = "poop\n";
		send(new_socket, message, strlen(message), 0);

		do
		{
			recv_size = recv(s, response, 1024, 0);
			if(recv_size > 0)
				printf("Bytes received: %d\nResponse: %s\n", recv_size, response);
			else if(recv_size == 0)
				puts("Connection closed.");
			else
				printf("recv failed: %d\n", WSAGetLastError());

		} while(recv_size > 0);
	}

	if(new_socket == INVALID_SOCKET)
	{
		printf("Accept failed with error code: %d", WSAGetLastError());
		return 1;
	}
	/*** /server ***/
	
	system("pause");

//	fclose(stdout);
	closesocket(s);
	WSACleanup();

	return 0;
}
