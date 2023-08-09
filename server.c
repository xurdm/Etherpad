#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <winsock2.h>
#include <ws2tcpip.h>
#include "etherpad.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv)
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	char response[1024];
	SIZE_T recv_size;
	SIZE_T c;
//	FILE *fstdout;
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

			if(recv_size > 0)
			{
				int srcBytes, clip_len;
				char* bytes = response;
				int bytes_len = recv_size;
				WCHAR* clip;

				//interpret byte string as unicode string
				srcBytes = MultiByteToWideChar(CP_UTF8, 0, bytes, bytes_len, NULL, 0);
				clip = (WCHAR*)calloc(srcBytes, sizeof(WCHAR)); //check ret
				clip_len = MultiByteToWideChar(CP_UTF8, 0, bytes, bytes_len, clip, srcBytes);

				//print client clipboard data
				printf("Bytes received: %d\nLast error: %d\nClipboard data: ", recv_size, WSAGetLastError());
				for(int i = 0; i < clip_len; ++i)
					wprintf(L"%lc", clip[i]);
				puts("");

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
