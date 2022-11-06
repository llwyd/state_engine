#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "mqtt_client.h"
#include "state.h"

#define TICK_PERIOD_MS ( 1000U )

#define SIGNALS(SIG) \

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );

extern void Task_Tick( void * pvParameters )
{
    TickType_t xLastWaitTime = xTaskGetTickCount();
    QueueHandle_t * queue = (QueueHandle_t *)pvParameters;
    const event_t e = EVENT( Tick );
    while( 1 )
    {
   
        xQueueSend( *queue, ( void *)&e, (TickType_t) 0U ); 
        vTaskDelayUntil( &xLastWaitTime, TICK_PERIOD_MS / portTICK_PERIOD_MS );
    }
}

static state_ret_t State_Loop( state_t * this, event_t s )
{
    STATE_DEBUG( s );
    state_ret_t ret;

    switch( s )
    {
        case EVENT( Tick ):
            
            HANDLED();
            break;
        case EVENT( Enter ):
        case EVENT( Exit ):
            HANDLED();
            break;
        default:
            NO_PARENT();
            break;
    }

    return ret;
}

void app_main(void)
{
    state_t state_machine;
    event_t current_event = EVENT( None );
    QueueHandle_t xEventQueue = xQueueCreate( FIFO_BUFFER_SIZE, sizeof( event_t ) );    
    TaskHandle_t xTick = NULL;

    xTaskCreate( Task_Tick, "Tick", 5000, &xEventQueue, (tskIDLE_PRIORITY + 1 ), &xTick );

    STATEMACHINE_Init( &state_machine, NULL, STATE( Loop ) );
    while( 1 )
    {
        if( xQueueReceive( xEventQueue, &current_event, (TickType_t)10 ) == pdPASS )
        {
            STATEMACHINE_Dispatch( &state_machine, current_event );
        }
    }
}
