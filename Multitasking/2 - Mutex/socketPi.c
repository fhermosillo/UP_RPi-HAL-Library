#include "socketPi.h"


typedef struct
{
	int id_socket0;
	int id_socket1;
	int rol;
	
	int domain;
	int type;
	int protocol;
	
	struct sockaddr_in socket0_addr, socket1_addr;
	
}SocketPi_t;




/* Main functions */
SocketPiHandler_t SocketPi(int domain, int type, int protocol)
{
	int socket_desc = socket(domain, type , protocol);
	if (socket_desc == -1)
	{
		return NULL;
	}
	
	SocketPi_t *pSocket = (SocketPi_t *)malloc(sizeof(SocketPi_t));
	pSocket->id_socket0 = -1;
	pSocket->id_socket1 = -1;
	pSocket->rol = -1;
	
	if(pSocket != NULL)
	{
		pSocket->id_socket0 = socket_desc;
		pSocket->domain = domain;
		pSocket->type = type;
		pSocket->protocol = protocol;
		
		memset(&pSocket->socket0_addr, 0, sizeof(pSocket->socket0_addr)); 
		memset(&pSocket->socket1_addr, 0, sizeof(pSocket->socket1_addr)); 
    
	} else
	{
		close(socket_desc);
	}
    
	return (SocketPiHandler_t)pSocket;
}

int SocketPi_Bind(SocketPiHandler_t hSocket, char *ip, int port)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	pSocket->socket0_addr.sin_family = AF_INET;
	if(ip == NULL)
		pSocket->socket0_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		pSocket->socket0_addr.sin_addr.s_addr = inet_addr(ip);
	pSocket->socket0_addr.sin_port = htons(port);
	
	if( bind(pSocket->id_socket0,(struct sockaddr *)&pSocket->socket0_addr, sizeof(pSocket->socket0_addr)) < 0)
	{
		return -1;
	}
	pSocket->rol = 0; // Server
	
	return 0;
}

int SocketPi_Connect(SocketPiHandler_t hSocket, char *ip, int port)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	pSocket->socket0_addr.sin_family = AF_INET;
	if(ip == NULL)
		pSocket->socket0_addr.sin_addr.s_addr = INADDR_ANY;
	else
		pSocket->socket0_addr.sin_addr.s_addr = inet_addr(ip);
	pSocket->socket0_addr.sin_port = htons(port);
	
	
	if (connect(pSocket->id_socket0 , (struct sockaddr *)&pSocket->socket0_addr , sizeof(pSocket->socket0_addr)) < 0)
	{
		return -1;
	}
	pSocket->rol = 1; // Client
	
	return 0;
}

void SocketPi_Listen(SocketPiHandler_t hSocket, int nconn)
{
	if(hSocket == NULL || nconn < 1)
	{
		return;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	listen(pSocket->id_socket0, nconn);
}

int SocketPi_Accept(SocketPiHandler_t hSocket, char *ip, int *port)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	
	ip = NULL;
	*port = -1;
	
	int c = sizeof(struct sockaddr_in);
	pSocket->id_socket1 = accept(pSocket->id_socket0, (struct sockaddr *)&pSocket->socket1_addr, (socklen_t*)&c);
	if (pSocket->id_socket1 < 0)
	{
		perror("accept failed");
		return -1;
	}
	
	ip = inet_ntoa(pSocket->socket1_addr.sin_addr);
	*port = ntohs(pSocket->socket1_addr.sin_port);
	
	return 0;
}

int SocketPi_Receive(SocketPiHandler_t hSocket, char *data, size_t maxlen, int timeout)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	// Read socket
	int sock_desc = pSocket->rol == 0 ? pSocket->id_socket1 : pSocket->id_socket0;
	int read_sz;
	
	if(timeout>0)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout;
		
		if (setsockopt(sock_desc, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
			perror("Error");
		}
	}
	
	if( (read_sz = recv(sock_desc , data , 1024 , 0)) > 0)
	{
		return read_sz;
	} else
	{
		close(sock_desc);
		return -1;
	}
}

int SocketPi_ReceiveFrom(SocketPiHandler_t hSocket, char*data, size_t maxlen, char *ip_dst, int *port_dst, int timeout)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	if(ip_dst!=NULL)
		*ip_dst = '\0';
	if(port_dst!=NULL)
		*port_dst = -1;
	
	int read_sz;
	int c = sizeof(struct sockaddr_in);
	
	if(timeout>0)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout;
		
		if (setsockopt(pSocket->id_socket0, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
			perror("Error");
		}
	}
	
	if( (read_sz = recvfrom(pSocket->id_socket0, data, maxlen, 0, (struct sockaddr *)&pSocket->socket1_addr, (socklen_t *)&c)) > 0)
	{
		if(ip_dst!=NULL)
			inet_ntop(pSocket->domain, &(pSocket->socket1_addr.sin_addr), ip_dst, INET_ADDRSTRLEN);
		if(port_dst!=NULL)
			*port_dst = ntohs(pSocket->socket1_addr.sin_port);
		
		return read_sz;
	} else
	{
		close(pSocket->id_socket0);
		return -1;
	}
}

int SocketPi_Send(SocketPiHandler_t hSocket, char *data, size_t len)
{
	if(hSocket == NULL)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	int sock_desc = pSocket->rol == 0 ? pSocket->id_socket1 : pSocket->id_socket0;
	if( send(sock_desc , data, len, 0) < 0 )
	{
		close(sock_desc);
		return -1;
	}
	
	return 0;
}


int SocketPi_SendTo(SocketPiHandler_t hSocket, char *data, size_t len, char *ip_dst, int port_dst)
{
	if(hSocket == NULL || ip_dst == NULL || port_dst < 0)
	{
		return -1;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	// Setup address
	pSocket->socket1_addr.sin_family = pSocket->domain;
	pSocket->socket1_addr.sin_addr.s_addr = inet_addr(ip_dst);
	pSocket->socket1_addr.sin_port = htons(port_dst);
	
	int c = sizeof(struct sockaddr_in);
	if( sendto(pSocket->id_socket0, data, len, 0, (struct sockaddr *)&pSocket->socket1_addr, (socklen_t)c) < 0 )
	{
		close(pSocket->id_socket0);
		return -1;
	}
	
	return 0;
}


void SocketPi_Close(SocketPiHandler_t hSocket)
{
	if(hSocket == NULL)
	{
		return;
	}
	SocketPi_t *pSocket = (SocketPi_t *)hSocket;
	
	
	close(pSocket->id_socket0);
	close(pSocket->id_socket1);
	
}

/* Auxiliar Functions */
int SocketPi_ip_from_hostname(const char *hostname, char *ip_addr)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		//gethostbyname failed
		herror("gethostbyname");
		return 1;
	}
	
	//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip_addr , inet_ntoa(*addr_list[i]) );
	}
	
	//printf("%s resolved to : %s\n" , hostname , ip_addr);
	
	return 0;
}
