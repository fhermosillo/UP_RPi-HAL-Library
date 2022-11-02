#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include "max7219.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#define LED_PIN		GPIO_PIN_5

/*
 * SCHEDULE FUNCTIONS:
 * 
 * int pthread_create(pthread_t * thread, 
                       const pthread_attr_t * attr,
                       void * (*start_routine)(void *), 
                       void *arg);
 * 
 * 
 * void pthread_exit(void *retval);
 * 
 * int pthread_cancel(pthread_t thread);
 * 
 * pthread_t pthread_self(void);
 * 
 * int pthread_setschedprio(pthread_t thread, int prio);
 * 
 * int sched_get_priority_max(int policy);
 * int sched_get_priority_min(int policy);
 * 
 * By default policy is SCHED_OTHER
 * 
 * SCHED_OTHER min/max priority    : 0/0
 * SCHED_FIFO min/max priority     : 1/99
 * SCHED_RR min/max priority       : 1/99
 * SCHED_BATCH min/max priority    : 0/0
 * SCHED_IDLE min/max priority     : 0/0
 */

void *max7219_task(void *ptr);
void *led_task(void *ptr);

// Task synchronization
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
SPI_t *SPI0 = NULL;

int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -------------------------------------------------------------------*/
	// *****************************
	// HAL Peripheral Initialization
	// *****************************
	HAL_GPIO_Init();
	
	// *****************************
	// Peripheral Configuration
	// *****************************
	MAX7219_Begin(NULL, SPI_CHIP_0);

		
	// *****************************
	// Main program
	// *****************************
	pthread_t thread1, thread2, thread3;
    char *message1 = "Thread 1";
    char *message2 = "Thread 3";
	
	/* Create independent threads each of which will execute function
	 * If pthread_create returns 0, it implies that the thread was 
	 * created, otherwise the thread was not created.
	 */
	pthread_create( &thread1, NULL, max7219_task, (void*) message1);
	pthread_create( &thread2, NULL, led_task, (void*) NULL);
	pthread_create( &thread3, NULL, max7219_task, (void*)message2);

	/* Wait till threads are complete before main continues. Unless we 
	 * wait we run the risk of executing an exit which will terminate
	 * the process and all threads before the threads have completed.
	 */
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 
	pthread_join( thread3, NULL); 
	
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}


void *max7219_task(void *ptr)
{
	char *msg = (char *)ptr;
	int slen = strlen(msg);
	
	// Configure hardware
	printf("Thread <Max7219> with Message: \"%s\"\n", msg);
	
	while(1)
	{
		// If mutex has been taken, this function locks thread until mutex is free
		//pthread_mutex_lock( &mutex1 );
		
		// Shared resource: Hardware SPI 
		MAX7219_AnimateText(msg,slen,40,MAX7219_DIR_RIGHT);
		
		// Unlock mutex (shared resource)
		//pthread_mutex_unlock( &mutex1 );
		
		// Add delay
		HAL_Thread_Delay_ms(100);
	}
}

void *led_task(void *ptr)
{
	// Set LED as an Output
	HAL_GPIO_Set_Mode(LED_PIN, GPIO_OUTPUT);
	
	// Set LED to zero
	HAL_GPIO_Write(LED_PIN, GPIO_LOW);
	
	while(1)
	{
		// LED blinking
		HAL_GPIO_Toggle(LED_PIN);
		
		// Suspend thread for 500ms
		HAL_Thread_Delay_ms(500);
	}
}
