#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include "socketPi.h"


int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -----------------------------------------------*/
	// *****************************************************************
	// HAL Peripheral Initialization
	// *****************************************************************
	HAL_GPIO_Init();
	
	// *****************************************************************
	// HAL Peripheral Configuration
	// *****************************************************************
	
	// *****************************************************************
	// Networking Initialization
	// *****************************************************************
	/* Create a TCP client */
	SocketPiHandler_t hClientSocket = SocketPi(AF_INET, SOCK_DGRAM, 0);
	if(hClientSocket == NULL)
	{
		puts("Error creating socket");
		return -1;
	}
	puts("Client");
	
	// *****************************************************************
	// Main program
	// *****************************************************************
	// Server sends time when client requests
	// Client request only six times
	char ipServer[] = "192.168.1.11";
	int portServer = 16000;
	int cnt = 0;
	while(cnt++ < 6)
	{
		char *txmsg = "time";
		if(SocketPi_SendTo(hClientSocket,txmsg,4,ipServer,portServer) < 0)
		{
			puts("Request failed");
			break;
		}
		
		char rxmsg[1024];
		int rxlen = 0;
		if((rxlen = SocketPi_ReceiveFrom(hClientSocket, rxmsg, 1024,NULL,NULL,TIMEOUT_MS(10))) < 0)
		{
			puts("Reply failed");
			break;
		}
		rxmsg[rxlen] = '\0';
 		printf("Server.Time: %s\n", rxmsg);
 		delay_ms(1000);
 		
	}
	
	// Close the socket
	SocketPi_Close(hClientSocket);
	
	/* USER CODE END -------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

