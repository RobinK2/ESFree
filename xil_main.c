
/*
 * This is a demonstration example of ESFree on Digilent ZedBoard platform.
 */

/* Xilinx includes. */
#include "xparameters.h"

#include "scheduler.h"



TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;
TaskHandle_t xHandle4 = NULL;


static void testFuncA1( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i=0; i < 6000000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestA1 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncA2( void *pvParameters)
{
char* c = pvParameters;
int i;
	for( i=0; i < 1000000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestA2 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncA3( void *pvParameters)
{
char* c = pvParameters;
int i;
	for( i=0; i < 60000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestA3 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncA4( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i=0; i < 1000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestA4 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncS1( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i=0; i < 1000000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestS1 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncS2( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i=0; i < 600000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestS2 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncS3( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i = 0; i < 8000000; i++ )
	{
	int a = i * i * i * i;
	}
	printf("TestS3 %c %d\r\n", *c, xTaskGetTickCount());
}

static void testFuncS4( void *pvParameters )
{
char* c = pvParameters;
int i;
	for( i = 0; i < 4000000; i++ )
	{
	int a = i*i*i*i;
	}
	printf("TestS4 %c %d\r\n", *c, xTaskGetTickCount());
}



static void testFunc1( void *pvParameters ){
char* c = pvParameters;
	printf("Test1 %c\r\n", *c);

int i, a;
	for( i = 0; i < 1000000; i++ )
	{
		a = 1 + i*i*i*i;
	}
}


int stopc = 0;
static void testFunc2( void *pvParameters )
{
char* c = pvParameters;
int i, a;
	for( i = 0; i < 1000000; i++ )
	{
		a = 1 + i * i * i * i;
	}
	printf("Test2 %c\r\n", *c);

	if( stopc == 3 )
	{
		vSchedulerAperiodicJobCreate( testFuncA1, "A1", "A1-2", pdMS_TO_TICKS( 100 ) );
	}
	if( stopc == 5 )
	{
		if( pdFALSE == xSchedulerSporadicJobCreate( testFuncS1, "S1", "S1-2", pdMS_TO_TICKS( 100 ), pdMS_TO_TICKS( 1000 ) ) )
		{
			printf("Sporadic job S1 not accepted tick %d\r\n", xTaskGetTickCount() );
		}
	}
	stopc++;
}

static void testFunc3( void *pvParameters ){
	char* c = pvParameters;
	printf("Test3 %c\r\n", *c);
	int i, a;
	for(i = 0; i < 1000000; i++ )
	{
		a = 1 + a * a * i;
	}
}

static void testFunc4(void *pvParameters)
{
	char* c = pvParameters;
	int i, a;
	for(i = 0; i < 2000000; i++)
	{
		a = 1 + i * i * i * i;
	}
	printf("Test4 %c\r\n", *c);
}

int main( void )
{
	printf( "Hello from Freertos\r\n" );

	char c1 = 'a';
	char c2 = 'b';
	char c3 = 'c';
	char c4 = 'e';


	vSchedulerInit();

	vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(200), pdMS_TO_TICKS(100), pdMS_TO_TICKS(500));
	vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(50), pdMS_TO_TICKS(100), pdMS_TO_TICKS(100), pdMS_TO_TICKS(100));
	vSchedulerPeriodicTaskCreate(testFunc3, "t3", configMINIMAL_STACK_SIZE, &c3, 3, &xHandle3, pdMS_TO_TICKS(0), pdMS_TO_TICKS(300), pdMS_TO_TICKS(100), pdMS_TO_TICKS(200));
	vSchedulerPeriodicTaskCreate(testFunc4, "t4", configMINIMAL_STACK_SIZE, &c4, 4, &xHandle4, pdMS_TO_TICKS(0), pdMS_TO_TICKS(400), pdMS_TO_TICKS(100), pdMS_TO_TICKS(100));

	vSchedulerAperiodicJobCreate( testFuncA1, "A1", "A1-1", pdMS_TO_TICKS( 100 ) );
	vSchedulerAperiodicJobCreate( testFuncA2, "A2", "A2-1", pdMS_TO_TICKS( 50 ) );
	vSchedulerAperiodicJobCreate( testFuncA3, "A3", "A3-1", pdMS_TO_TICKS( 50 ) );
	vSchedulerAperiodicJobCreate( testFuncA4, "A4", "A4-1", pdMS_TO_TICKS( 50 ) );


	BaseType_t xReturnValue = xSchedulerSporadicJobCreate( testFuncS1, "S1", "S1-1", pdMS_TO_TICKS( 100 ), pdMS_TO_TICKS( 300 ) );
	if( pdFALSE == xReturnValue )
	{
		printf("Sporadic job S1 not accepted\r\n");
	}

	xReturnValue = xSchedulerSporadicJobCreate( testFuncS2, "S2", "S2-1", pdMS_TO_TICKS( 20 ), pdMS_TO_TICKS( 50 ) );
	if( pdFALSE == xReturnValue )
	{
		printf("Sporadic job S2 not accepted\r\n");
	}

	xReturnValue = xSchedulerSporadicJobCreate( testFuncS3, "S3", "S3-1", pdMS_TO_TICKS( 50 ), pdMS_TO_TICKS( 1400 ) );
	if( pdFALSE == xReturnValue )
	{
		printf("Sporadic job S3 not accepted\r\n");
	}
	xReturnValue = xSchedulerSporadicJobCreate( testFuncS4, "S4", "S4-1", pdMS_TO_TICKS( 200 ), pdMS_TO_TICKS( 20000 ) );
	if( pdFALSE == xReturnValue )
	{
		printf("Sporadic job S4 not accepted\r\n");
	}


	vSchedulerStart();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/


