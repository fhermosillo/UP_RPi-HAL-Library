#ifndef __SOCKET_PI_H
#define __SOCKET_PI_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <netdb.h>	//hostent
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/time.h>

#define SOCKETPI_TIMEOUT(MS) X*1000
#define SOCKETPI_NO_TIMEOUT		-1

#define SOCKETPI_TCP	SOCK_STREAM
#define SOCKETPI_UDP	SOCK_DGRAM

#define SOCKETPI_PROT_DEFAULT	0

typedef void* SocketPiHandler_t;

/*
 * Create a socket
 *
 * domain: @https://man7.org/linux/man-pages/man2/socket.2.html
 *	AIF_NET (IPv4)
 *	AF_INET6 (IPv6)
 *	AF_BLUETOOTH (Bluetooth low-level socket protocol)
 *	AF_UNIX (Local communication)
 *
 * type: @https://man7.org/linux/man-pages/man2/socket.2.html
 *	SOCK_STREAM (Connection-based byte strems or TCP)
 *	SOCK_DGRAM (Datagrams)
 *	SOCK_RAW (Raw network protocol access)
 *
 * protocol: Must be set to 0, to use the protocol according to the socket type
 */
SocketPiHandler_t SocketPi(int domain, int type, int protocol);

/*
 * Bind an IP address and a Port
 */
int SocketPi_Bind(SocketPiHandler_t hSocket, char *ip, int port);

/*
 * Connect to a server
 */
int SocketPi_Connect(SocketPiHandler_t hSocket, char *ip, int port);

/*
 * Listen to a maximum number of devices
 */
void SocketPi_Listen(SocketPiHandler_t hSocket, int nconn);

/*
 * Accept a connection-based client
 */
int SocketPi_Accept(SocketPiHandler_t hSocket, char *ip, int *port);

/*
 * Receives data from a connected client/server
 * Timeout: Timeout to receive data in microseconds
 */
int SocketPi_Receive(SocketPiHandler_t hSocket, char *data, size_t maxlen, int timeout);

/*
 * Receives data from a client/server at ip address and port
 * Timeout: Timeout to receive data in microseconds
 */
int SocketPi_ReceiveFrom(SocketPiHandler_t hSocket, char*data, size_t maxlen, char *ip_src, int *port_src, int timeout);

/*
 * Sends data to a connected client/server
 */
int SocketPi_Send(SocketPiHandler_t hSocket, char *data, size_t len);

/*
 * Sends data to a client/server at ip address and port
 */
int SocketPi_SendTo(SocketPiHandler_t hSocket, char *data, size_t len, char *ip_dst, int port_dst);

/*
 * Close socket
 */
void SocketPi_Close(SocketPiHandler_t hSocket);

/*
 * Get ip from a hostname (e.g, a webpage, etc)
 */
int SocketPi_ip_from_hostname(const char *hostname, char *ip_addr);

#endif /* __SOCKET_PI_H */
