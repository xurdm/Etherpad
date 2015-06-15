#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr is deprecated and I don't care because fuck IPv6
#include <winsock2.h>
#include <ws2tcpip.h>
#include "etherpad.h"
#pragma comment(lib, "ws2_32.lib")

typedef enum { RESPONSE_OK, RESPONSE_NO, RESPONSE_SOMETHING_WEIRD_HAPPENED } SERVER_MSG;

int main(int argc, char **argv)
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	char* message;
	char response[1024];
	SIZE_T recv_size;
	SERVER_MSG msg;
	SIZE_T c;
	FILE *fstdout;
	WCHAR* current_clip;

//	freopen_s(&fstdout, "console.log", "w", stdout);	//redirect stdout to log file. TODO: error check

	puts("Calling WSAStartup() ...");
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		printf(" ...failed. Error code %d", WSAGetLastError());
		return 1;
	}
	puts("WSAStartup() successful.");

	if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Failed to create socket: %d", WSAGetLastError());
	}
	puts("Socket created.");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(3028);

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
		
		do
		{
			recv_size = recv(new_socket, response, 1024, 0);
			printf("data size: %d\n", recv_size);

			if(recv_size > 0)
			{
				int srcBytes, clip_len;
				char* bytes = response;
				int bytes_len = recv_size;
				WCHAR* clip;

				srcBytes = MultiByteToWideChar(CP_UTF8, 0, bytes, bytes_len, NULL, 0);
				clip = (WCHAR*)calloc(srcBytes, sizeof(WCHAR)); //check ret
				clip_len = MultiByteToWideChar(CP_UTF8, 0, bytes, bytes_len, clip, srcBytes);
				clip[clip_len-1] = 0;

				for(int i = 0; i < clip_len; ++i)
					wprintf(L"%lc", clip[i]);

				wprintf("Bytes received: %d\nLast error: %d\n", recv_size, WSAGetLastError());

				free(clip);
			}
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

//	fclose(stdout);
	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}
