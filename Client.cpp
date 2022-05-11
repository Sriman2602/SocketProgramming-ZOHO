#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
struct newAccount {
    string name;
    string pwd;
}n;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 50
#define DEFAULT_PORT "27015"

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
char sendbuf[DEFAULT_BUFLEN];
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
string sen;
char user[50];

void Send() {
    cout << "TO BE SENT: ";
    cin >> sen;
    strcpy_s(sendbuf, sen.c_str());
    cout << "SENDBUF: " << sendbuf;
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    cout << "Sent: " << sen << endl;
}

void Receive() {
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        cout << "Received: ";
        for (int i = 0; i < iResult; i++)
            cout << recvbuf[i];
        cout << endl;
    }
}

void ACC_creation() {
    string str;
    cout << "Enter Personal Details\n";
    cout << "Name: ";
    cin >> n.name;
    cout << "Password: ";
    cin >> n.pwd;
    str = "$ " + n.name + '_' + n.pwd;
    strcpy_s(user, str.c_str());
    send(ConnectSocket, user, strlen(user), 0);
    cout << "Sent: " << user <<endl;
    iResult = recv(ConnectSocket, recvbuf, recvbuflen,0);
    for (int i = 0; i < iResult; i++)
        cout << recvbuf[i];
    cout << endl;
}

int main(int argc, char** argv)
{

    // Validate the parameters
    if (argc != 2) {
        cout << "usage: " << argv[0] << "server - name\n";
        //return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }           

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    
    ACC_creation();
    while (TRUE) {
        Send();
        Receive();
    }
    return 0;
}
