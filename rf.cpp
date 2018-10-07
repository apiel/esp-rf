#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>
#include <task.hpp>

#include "rf.h"
#include "config.h"
#include "rf_receiver.h"
#include "web.h"

Rf rf = Rf();

void rf_task(void *pvParameters)
{
    char code[RF_CODE_SIZE];
    rf.init();
    while(1){
        while(xQueueReceive(rf.rf_queue, &code, 0) == pdTRUE){
            // printf("Got code from queue: %s\n", code);
            rf.consumer(code);
        }
        taskYIELD();
        vTaskDelay(10);
    }
}

void Rf::onReceived(char * result) {
    printf("Result::: %s\n", result);
    if (rf_queue && xQueueSend(rf_queue, result, 0) == pdFALSE) {
        printf("Rf queue overflow (no more place in queue).\r\n");
    }
}

void Rf::consumer(char * result) {
    // printf("Consume queue: %s\n", result);
    post(result);
}

void Rf::init()
{
    rf_queue = xQueueCreate(20, RF_CODE_SIZE);
    printf("Start rf receiver\n");
    rfReceiver.start(PIN_RF433_RECEIVER, [](char * result){
        rf.onReceived(result);
    });
}
