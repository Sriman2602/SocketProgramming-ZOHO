#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 50
#define DEFAULT_PORT "27015"

WSADATA wsaData;
int iResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;
SOCKET ClientSocket1 = INVALID_SOCKET;

struct addrinfo* result = NULL;
struct addrinfo hints;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
char userpass[50];

void ServerRecSend(SOCKET S,SOCKET D, char recvbuf[50], int recvbuflen) {
	iResult = recv(S, recvbuf, recvbuflen, 0);
	cout << "RECEIVED: ";
	if (recvbuf[0] == '$') {//USER CREATION and Storing Details.
		cout << "FILE TO BE STORED:";
		for (int i = 0; i < iResult; i++)
			cout<<recvbuf[i];
		cout << endl;
		send(S, "USER CREATED", 13, 0);
		return;
	}
	if (iResult > 0) {
		for (int i = 0; i < iResult; i++)
			cout << recvbuf[i];
		cout << endl;
		iSendResult = send(D, recvbuf, iResult, 0);
		printf("Bytes sent: %d\n", iSendResult);
	}
}

int main()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	ClientSocket1 = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// No longer need server socket
	closesocket(ListenSocket);
	ServerRecSend(ClientSocket, ClientSocket1, recvbuf, recvbuflen);//USER ACCEPTING.
	do{
		ServerRecSend(ClientSocket, ClientSocket1, recvbuf, recvbuflen);
		ServerRecSend(ClientSocket1, ClientSocket, recvbuf, recvbuflen);
	} while (TRUE);
	return 0;
}
