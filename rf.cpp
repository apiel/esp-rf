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
    if (strcmp(result, "cD;]d<]DEc<E=EEEE=[:[-1") == 0) {
        // printf("button 3 up\n");
        // ip_addr_t ip;
        // IP4_ADDR(&ip, 192, 168, 0, 122);
        // post((char *)"{\"on\": true}", ip, 8080, "POST", "/api/S6QJ3NqpQzsR6ZFzOBgxSRJPW58C061um8oP8uhf/lights/0xd0cf5efffe6f87e4/state");
        if (rf_queue && xQueueSend(rf_queue, result, 0) == pdFALSE) {
            printf("Rf queue overflow (no more place in queue).\r\n");
        }
    }
    // if (strcmp(result, "ZZVUVIZV-0") == 0) { // LIMIT what goes in the queue
    //     printf("Result::: %s\n", result);
    //     if (rf_queue && xQueueSend(rf_queue, result, 0) == pdFALSE) {
    //         printf("Rf queue overflow (no more place in queue).\r\n");
    //     }
    // }
}

void Rf::consumer(char * result) {
    // printf("Consume queue: %s\n", result);

// need to move this in another file
    ip_addr_t ip;
    if (strcmp(result, "cD;]d<]DEc<E=EEEE=[:[-1") == 0) {
        printf("send\n");
        // need a timer to dont send the request multiple time
        IP4_ADDR(&ip, 192, 168, 0, 122);
        post((char *)"{\"on\": true}", ip, 8080, "POST", "/api/S6QJ3NqpQzsR6ZFzOBgxSRJPW58C061um8oP8uhf/lights/0xd0cf5efffe6f87e4/state");
    } else {
        // HTTP_IP(&ip);
        // post(result, ip, HTTP_PORT, "POST", "/rf");
    }
}

void Rf::init()
{
// // need to find why it doesnt work
// ip_addr_t ip;
// IP4_ADDR(&ip, 192, 168, 0, 67);
// post((char *)"{\"on\": true}", ip, 80, "POST", "/api/S6QJ3NqpQzsR6ZFzOBgxSRJPW58C061um8oP8uhf/lights/state");

    rf_queue = xQueueCreate(20, RF_CODE_SIZE);
    printf("Start rf receiver\n");
    rfReceiver.start(PIN_RF433_RECEIVER, [](char * result){
        rf.onReceived(result);
    });
}
