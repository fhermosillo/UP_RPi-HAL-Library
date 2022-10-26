#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include <time.h>
#include "socketPi.h"


int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -----------------------------------------------*/
	// *****************************
	// HAL Peripheral Initialization
	// *****************************
	HAL_GPIO_Init();
	
	// *****************************
	// Peripheral Configuration
	// *****************************
	
	// *****************************
	// Networking Initialization
	// *****************************
	/* Create a TCP server */
	SocketPiHandler_t hServerSocket = SocketPi(AF_INET, SOCK_STREAM, 0);
	if(hServerSocket == NULL)
	{
		puts("Error creating socket");
		return -1;
	}
	
	if(SocketPi_Bind(hServerSocket,NULL,16000) != 0)
	{
		puts("Error Bind Address");
		SocketPi_Close(hServerSocket);
		return -1;
	}
	SocketPi_Listen(hServerSocket,10);
	
	char ip_client[30];
	int port_client;
	if(SocketPi_Accept(hServerSocket, ip_client, &port_client) != 0)
	{
		puts("Error Accepting Client");
		SocketPi_Close(hServerSocket);
		return -1;
	}
	
	// *****************************
	// Main program
	// *****************************
	// Server sends time when client requests
	time_t tick;
	char str[100];
	while(1)
	{
		char rxmsg[1024];
		int rxlen = 0;
		if((rxlen = SocketPi_Receive(hServerSocket, rxmsg,1024)) < 0)
		{
			puts("Receive failed");
			break;
		}
		rxmsg[rxlen] = '\0';
 		printf("Client: %s\n", rxmsg);
 		
 		if(strcmp(rxmsg,"time")==0)
 		{
			time(&tick);
			snprintf(str,100,"%s",ctime(&tick));
			size_t slen = strlen(str);
			if(SocketPi_Send(hServerSocket,str,slen) < 0)
			{
				puts("Send failed");
				break;
			}
		}
	}
	
	// Close the socket
	SocketPi_Close(hServerSocket);
	
	/* USER CODE END -------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

