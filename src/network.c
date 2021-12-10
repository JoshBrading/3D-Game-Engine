// Client network code modified from https://docs.microsoft.com/en-us/windows/win32/winsock/sending-and-receiving-data-on-the-client

#define WIN32_LEAN_AND_MEAN

#include "network.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <simple_logger.h>
#include <entity.h>
#include <static_entity.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;

int establish_connection( char* ipAddr )
{


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        slog("WSAStartup failed with error: %d", iResult);
        return 0;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ipAddr, DEFAULT_PORT, &hints, &result );
    if ( iResult != 0 ) {
        slog("getaddrinfo failed with error: %d", iResult);
        WSACleanup();
        return 0;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            slog("socket failed with error: %ld", WSAGetLastError());
            WSACleanup();
            return 0;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        slog("Unable to connect to server!");
        WSACleanup();
        return 0;
    }

}

void send_update()
{
    const char* sendbuf = "this is a test";

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        slog("send failed with error: %d", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
    }

    slog("Bytes Sent: %i", iResult);



    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            slog("Bytes received: %i", iResult);
        else if (iResult == 0)
            slog("Connection closed");
        else
            slog("recv failed with error: %d", WSAGetLastError());

    } while (iResult > 0);
}

void close_connection()
{
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        slog("shutdown failed with error: %d", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
    }


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    slog("Network connection Closed.");
}

char build_json()
{
    
}