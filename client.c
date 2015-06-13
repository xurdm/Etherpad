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
	char* localhost = "127.0.0.1";
	char* message, response[1024];
	SIZE_T recv_size, c, clip_len, bytes_len;
	FILE* fstdout;
	WCHAR* clip;
	char* bytes;

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
	server.sin_addr.s_addr = inet_addr(localhost);
	server.sin_port = htons(3028);

	printf("Connecting to %s...\n", localhost);
	if(connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("Connection failed.");
		return 1;
	}
	puts("Connected.");

	if((recv_size = recv(s, response, 1024, 0)) == SOCKET_ERROR)
	{
		puts("Receive failed.");
		return 1;
	}
	//printf("Message from server: %.*s\n", recv_size, response);

	//convert unicode string to byte string and send to server
	clip = get_clipboard_data();
	clip_len = wcslen(clip);
	int srcBytes = WideCharToMultiByte(CP_UTF8, 0, clip, clip_len, NULL, 0, NULL, NULL);
	bytes = (char*)malloc(srcBytes);
	bytes_len = WideCharToMultiByte(CP_UTF8, 0, clip, clip_len, bytes, srcBytes, NULL, NULL);
	send(s, bytes, bytes_len, 0);

//	fclose(stdout);
	closesocket(s);
	WSACleanup();

	system("pause");

	return 0;
}w