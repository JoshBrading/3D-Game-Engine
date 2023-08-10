// Server network code modified from https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include "simple_logger.h"
#include "gfc_types.h"
#include "entity.h"
#include "world.h"
#include "enemy.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
int cResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;


struct addrinfo hints;

int network_server_start()
{

    struct addrinfo* result = NULL;

    // Initialize Winsock
    cResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if( cResult != 0 )
    {
        printf( "WSAStartup failed with error: %d\n", cResult );
        return 1;
    }

    ZeroMemory( &hints, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    cResult = getaddrinfo( NULL, DEFAULT_PORT, &hints, &result );
    if( cResult != 0 )
    {
        printf( "getaddrinfo failed with error: %d\n", cResult );
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if( ListenSocket == INVALID_SOCKET )
    {
        printf( "socket failed with error: %ld\n", WSAGetLastError() );
        freeaddrinfo( result );
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    cResult = bind( ListenSocket, result->ai_addr, ( int )result->ai_addrlen );
    if( cResult == SOCKET_ERROR )
    {
        printf( "bind failed with error: %d\n", WSAGetLastError() );
        freeaddrinfo( result );
        closesocket( ListenSocket );
        WSACleanup();
        return 1;
    }

    freeaddrinfo( result );

    cResult = listen( ListenSocket, SOMAXCONN );
    if( cResult == SOCKET_ERROR )
    {
        printf( "listen failed with error: %d\n", WSAGetLastError() );
        closesocket( ListenSocket );
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept( ListenSocket, NULL, NULL );
    if( ClientSocket == INVALID_SOCKET )
    {
        printf( "accept failed with error: %d\n", WSAGetLastError() );
        closesocket( ListenSocket );
        WSACleanup();
        return 1;
    }
   
    // No longer need server socket
    closesocket( ListenSocket );


    return 0;
}


void network_server_receive()
{
    char recvbuf[ 1024 ];
    int recvbuflen = 1024;
    int iSendResult;
    // Receive until the peer shuts down the connection

    //cResult = recv( ClientSocket, recvbuf, recvbuflen, 0 );
    //if( cResult > 0 )
    //{
    //    //printf( "Bytes received: %d\n", cResult );
    //
    //    char cleanText[ 512 ];
    //    char charArray[ 4 ][ 32 ];
    //    strncpy( cleanText, recvbuf, cResult );
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
    //            if( i == 0) remotePos.x = atof( ptr ); // Relies on data being sent in proper order
    //            if( i == 1) remotePos.y = atof( ptr );
    //
    //            i++;
    //        }
    //
    //        printf( "Pos( %f, %f )", remotePos.x, remotePos.y );
    //
    //        if( strstr( cleanText, "SDLK_1" ) != NULL )
    //        {
    //            Entity* tower = tower_rifle_new( remotePos );
    //        }else if( strstr( cleanText, "SDLK_2" ) != NULL )
    //        {
    //            Entity* tower = tower_grenadier_new( remotePos );
    //        }else if( strstr( cleanText, "SDLK_3" ) != NULL )
    //        {
    //            Entity* tower = tower_blocker_new( remotePos );
    //        }else if( strstr( cleanText, "SDLK_4" ) != NULL )
    //        {
    //            Entity* tower = tower_mechanic_new( remotePos );
    //        }else if( strstr( cleanText, "SDLK_5" ) != NULL )
    //        {
    //            Entity* tower = tower_mechanic_new( remotePos );
    //            tower->tag = "t_support";
    //            tower->model = gf3d_model_load( "t_support", 1 ); // Wow! Talk about bad code.
    //        }
    //
    //        //return 0;
    //
    //    }
    //    // Echo the buffer back to the sender
    //
    //
    //    Entity* ent;
    //    //slog( "Send Update" );
    //   // Uint8 canSend = false;
    //    char data[ 1024 ];
    //    int count = entity_get_manager()->entity_count;
    //    int i;
    //    for( i = 0; i < entity_get_manager()->entity_count; i++ )
    //    {
    //        ent = &entity_get_manager()->entity_list[ i ];
    //
    //        if( ent->tag != NULL && ent->tag == "player" )
    //        {
    //            // canSend = true;
    //           //  slog( "Last Input: %s", ent->lastInput );
    //             //slog( "Position ( %f, %f, %f )", ent->position.x, ent->position.y, ent->position.z );
    //            sprintf( data, "%s(%f,%f,%f)", ent->lastInput, ent->targetPos.x, ent->targetPos.y, ent->targetPos.z );
    //            ent->lastInput = ""; // Reset the input so it does not send multiple times
    //            const char* sendbuf = data;
    //            iSendResult = send( ClientSocket, sendbuf, ( int )strlen( sendbuf ), 0 );
    //            
    //            int rdm = rand() % ( 100 - 0 + 1 );
    //            //slog( "Rand: %i", rdm );
    //            if( 1 == rdm )
    //            {
    //                for( int row = 1; row < world_get()->maxRows; row++ )
    //                {
    //                    if( 1 == rand() % ( 25 - 0 + 1 ) )
    //                    {
    //                        Entity* enemy = enemy_new( "enemy", vector3d( world_get()->maxCols * 2 + 1, row * 2, 0 ) );
    //                        sprintf( data, "SDLK_Q(%f,%f,%f)", enemy->position.x, enemy->position.y, 0 );
    //                        sendbuf = data;
    //                        iSendResult = send( ClientSocket, sendbuf, ( int )strlen( sendbuf ), 0 );
    //                    }
    //
    //                }
    //            }
    //            
    //            
    //            
    //            if( iSendResult == SOCKET_ERROR )
    //            {
    //                slog( "send failed with error: %d", WSAGetLastError() );
    //                closesocket( ClientSocket );
    //                WSACleanup();
    //            }
    //        }
    //    }
    //
    //    //iSendResult = send( ClientSocket, recvbuf, cResult, 0 );
    //    //if( iSendResult == SOCKET_ERROR )
    //    //{
    //    //    printf( "send failed with error: %d\n", WSAGetLastError() );
    //    //    closesocket( ClientSocket );
    //    //    WSACleanup();
    //    //    return;
    //    //}
    //    ////printf( "Bytes sent: %d\n", iSendResult );
    //}
    //else if( cResult == 0 )
    //    printf( "Connection closing...\n" );
    //else
    //{
    //    printf( "recv failed with error: %d\n", WSAGetLastError() );
    //    closesocket( ClientSocket );
    //    WSACleanup();
    //    return;
    //}

    return;



}

void network_server_close()
{
    // shutdown the connection since we're done
    cResult = shutdown( ClientSocket, SD_SEND );
    if( cResult == SOCKET_ERROR )
    {
        printf( "shutdown failed with error: %d\n", WSAGetLastError() );
        closesocket( ClientSocket );
        WSACleanup();
        return;
    }

    // cleanup
    closesocket( ClientSocket );
    WSACleanup();
}