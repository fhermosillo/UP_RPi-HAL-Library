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

#define TIMEOUT_MS(X) X*1000
#define NO_TIMEOUT		-1

typedef void* SocketPiHandler_t;

SocketPiHandler_t SocketPi(int domain, int type, int protocol);

int SocketPi_Bind(SocketPiHandler_t hSocket, char *ip, int port);

int SocketPi_Connect(SocketPiHandler_t hSocket, char *ip, int port);

void SocketPi_Listen(SocketPiHandler_t hSocket, int nconn);
int SocketPi_Accept(SocketPiHandler_t hSocket, char *ip, int *port);

int SocketPi_Receive(SocketPiHandler_t hSocket, char *data, size_t maxlen, int timeout);
int SocketPi_ReceiveFrom(SocketPiHandler_t hSocket, char*data, size_t maxlen, char *ip_src, int *port_src, int timeout);
int SocketPi_Send(SocketPiHandler_t hSocket, char *data, size_t len);
int SocketPi_SendTo(SocketPiHandler_t hSocket, char *data, size_t len, char *ip_dst, int port_dst);

void SocketPi_Close(SocketPiHandler_t hSocket);

int SocketPi_ip_from_hostname(const char *hostname, char *ip_addr);

#endif /* __SOCKET_PI_H */
