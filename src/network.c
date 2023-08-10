// Client network code modified from https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code

#define WIN32_LEAN_AND_MEAN

#include "network.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <simple_logger.h>
#include <entity.h>
#include "enemy.h"
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

    Entity* ent;
    //slog( "Send Update" );
   // Uint8 canSend = false;
    char data[ 1024 ];
    int count = entity_get_manager()->entity_count;
    int i;
    for( i = 0; i < entity_get_manager()->entity_count; i++ )
    {
        ent = &entity_get_manager()->entity_list[ i ];

        if( ent->tag != NULL && ent->tag == "player")
        {
           // canSend = true;
          //  slog( "Last Input: %s", ent->lastInput );
            //slog( "Position ( %f, %f, %f )", ent->position.x, ent->position.y, ent->position.z );
            sprintf( data, "%s(%f,%f,%f)", ent->lastInput, ent->targetPos.x, ent->targetPos.y, ent->targetPos.z);
            ent->lastInput = ""; // Reset the input so it does not send multiple times
            const char* sendbuf = data;
            iResult = send( ConnectSocket, sendbuf, ( int )strlen( sendbuf ), 0 );
            if( iResult == SOCKET_ERROR )
            {
                slog( "send failed with error: %d", WSAGetLastError() );
                closesocket( ConnectSocket );
                WSACleanup();
            }

           // slog( "Bytes Sent: %i", iResult );
            network_receive();
        }
    }
}

void network_receive()
{
    // Receive until the peer closes the connection
    //iResult = recv( ConnectSocket, recvbuf, recvbuflen, 0 );
    //if( iResult > 0 )
    //{
    //    //printf( "Bytes received: %d\n", cResult );
    //
    //    char cleanText[ 512 ];
    //    char charArray[ 4 ][ 32 ];
    //    strncpy( cleanText, recvbuf, iResult );
    //
    //    if( strstr( cleanText, "SDLK" ) != NULL )
    //    {
    //        //printf( "Data...\n%s \n", cleanText );
    //
    //        char delim[] = "(,)";
    //
    //        char* ptr = strtok( recvbuf, delim );
    //
    //        Vector3D remotePos = vector3d( 0.0f, 0.0f, 0.0f );
    //        int i = 0;
    //        while( ptr != NULL )
    //        {
    //            printf( "'%s'\n", ptr );
    //            ptr = strtok( NULL, delim );
    //
    //            if( i == 0 ) remotePos.x = atof( ptr ); // Relies on data being sent in proper order
    //            if( i == 1 ) remotePos.y = atof( ptr );
    //
    //            i++;
    //        }
    //
    //        printf( "Pos( %f, %f )", remotePos.x, remotePos.y );
    //
    //        if( strstr( cleanText, "SDLK_1" ) != NULL )
    //        {
    //            Entity* tower = tower_rifle_new( remotePos );
    //        }
    //        else if( strstr( cleanText, "SDLK_2" ) != NULL )
    //        {
    //            Entity* tower = tower_grenadier_new( remotePos );
    //        }
    //        else if( strstr( cleanText, "SDLK_3" ) != NULL )
    //        {
    //            Entity* tower = tower_blocker_new( remotePos );
    //        }
    //        else if( strstr( cleanText, "SDLK_4" ) != NULL )
    //        {
    //            Entity* tower = tower_mechanic_new( remotePos );
    //        }
    //        else if( strstr( cleanText, "SDLK_5" ) != NULL )
    //        {
    //            Entity* tower = tower_mechanic_new( remotePos );
    //            tower->tag = "t_support";
    //            tower->model = gf3d_model_load( "t_support", 1 ); // Wow! Talk about bad code.
    //        }
    //        else if( strstr( cleanText, "SDLK_Q" ) != NULL )
    //        {
    //            enemy_new( "enemy", remotePos );
    //        }
    //    }
    //}
    //else if( iResult == 0 )
    //    slog( "Connection closed" );
    //else
    //    slog( "recv failed with error: %d", WSAGetLastError() );
    return;
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