#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include <time.h>
#include "socketPi.h"

// Define here the server IP address
#define SERVER_IP "192.168.1.11"
// Define here the server port
#define SERVER_PORT 16000
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
	SocketPiHandler_t hServerSocket = SocketPi(AF_INET, SOCK_DGRAM, 0);
	if(hServerSocket == NULL)
	{
		puts("Error creating socket");
		return -1;
	}
	puts("Server");
	if(SocketPi_Bind(hServerSocket,SERVER_IP,SERVER_PORT) != 0)
	{
		puts("Error Bind Address");
		SocketPi_Close(hServerSocket);
		return -1;
	}
	
	// *****************************
	// Main program
	// *****************************
	// Server sends time when client requests
	time_t tick;
	char str[100];
	char ipClient[30];
	int portClient;
	while(1)
	{
		char rxmsg[1024];
		int rxlen = 0;
		if((rxlen = SocketPi_ReceiveFrom(hServerSocket, rxmsg,1024,ipClient,&portClient,PISOCKET_NO_TIMEOUT)) < 0)
		{
			puts("Receive failed");
			break;
		}
		rxmsg[rxlen] = '\0';
 		printf("Client<%s,%d>: %s\n", ipClient,portClient,rxmsg);
 		
 		if(strcmp(rxmsg,"time")==0)
 		{
			time(&tick);
			snprintf(str,100,"%s",ctime(&tick));
			size_t msglen = strlen(str);
			if(SocketPi_SendTo(hServerSocket,str,msglen,ipClient,portClient) < 0)
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

