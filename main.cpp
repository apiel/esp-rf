#include "task.hpp"
#include "espressif/esp_common.h"
#include "esp/uart.h"

#include <string.h>

#include <ssid_config.h>

#include "config.h"
#include "wifi.h"
#include "version.h"
#include "rf.h"
#include "led.h"

static void  main_task(void *pvParameters)
{
    wifi_wait_connection();
    xTaskCreate(&rf_task, "rf_task", 1024, NULL, 4, NULL);

    while(1) { // keep task running else program crash, we could also use xSemaphore
        task_led_blink(2, 10, 20);
        taskYIELD();
        vTaskDelay(200);
        taskYIELD();
        wifi_wait_connection(); // here we could check for wifi connection and reboot if disconnect
    }
}

extern "C" void user_init(void)
{
    uart_set_baud(0, 115200);

    printf("SDK version: %s\n", sdk_system_get_sdk_version());
    printf("MyHomeBridge sonoff compile version: %s\n", VERSION);

    wifi_new_connection((char *)WIFI_SSID, (char *)WIFI_PASS);

    xTaskCreate(&main_task, "main_task", 1024, NULL, 9, NULL);
}
