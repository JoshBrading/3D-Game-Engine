#ifndef __NETWORK_H__
#define __NETWORK_H__

/**
 * @brief Attempts to connect to the provided ip
 * @param IP Address to connect to
 * @return False on error, True on success
 */
int establish_connection( char* ipAddr );

void send_update( );

void network_receive();

void close_connection();

#endif
