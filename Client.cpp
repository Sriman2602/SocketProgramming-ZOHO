#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
int i;
WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
string sen;
char sendbuf[DEFAULT_BUFLEN];

class newAccount {
private:
    string name;
    string pwd;
public:
    string sen;
    void newAcc() {
        cout << "Enter Basic Details\n";
        cout << "NAME: ";
        cin >> name;
        cout << "PASSWORD: ";
        cin >> pwd;
        sen = "$ "+ name + " " + pwd;
        cout << sen;
        strcpy_s(sendbuf, sen.c_str());
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        cout << "\nSENT TO SERVER\n";
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        for (i = 0; i < iResult; i++) {
            cout << recvbuf[i];
        }
    }

    void alrAcc() {
        int j, flag = 0;
        string uid, pass;
        cout << "Enter Username and Password\n";
        cout << "USERNAME: ";
        cin >> uid;
        cout << "PASSWORD: ";
        cin >> pass;
        if (uid == name) {
            if (pass == pwd) {
                cout << "WELCOME " << uid;
                flag = 1;
            }
        }
        if (flag == 0) {
            cout << "Wrong Username and Password,Please try again. ";
        }
    }
};

void acc() {
    newAccount n;
    int choice; char ch;
    i = 0;
    do {
        system("cls");
        cout << "\t\t************************************************************\t\t";
        cout << "\n\n\t\t\t\tDOTS AND BOXES\n\n";
        cout << "\t\t************************************************************\t\t";
        cout << "\n\n\t 1. NEW USER";
        cout << "\n\n\t 2. ALREADY HAVING?";
        cout << "\n\n\n\t ENTER YOUR CHOICE: ";
        cin >> choice;
        if (choice == 1) {
            n.newAcc();
        }
        else if (choice == 2) {
            n.alrAcc();
        }
        else
            cout << "Wrong Choice";
        cin >> ch;
    } while (ch!='q');
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

    // Send an initial buffer
    acc();
    system("cls");
    while (TRUE) {
        sendbuf[0] = '\0';
        cout << sendbuf;
        cout << "Sending: ";
        cin >> sen;
        strcpy_s(sendbuf, sen.c_str());
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        cout << "Sent: " << sendbuf;
        
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            cout << "Received: ";
            for (int i = 0; i < iResult; i++)
                cout << recvbuf[i];
            cout << endl;
        }       
    }
    return 0;
}
