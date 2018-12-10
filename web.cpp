#include <lwip/api.h>
#include <string.h>
#include <espressif/esp_common.h>

#include "config.h"

// need a timeout

void post(char * payload, ip_addr_t ip, int port, const char * method, const char * path) {
    struct netconn *nc = netconn_new(NETCONN_TCP);
    err_t err;
    if (nc == NULL) {
        printf("Failed to allocate socket\n");
        return;
    }

    struct ip_info myIpInfo;
    (void) sdk_wifi_get_ip_info(STATION_IF, &myIpInfo);
    err = netconn_bind(nc, &myIpInfo.ip, 0);
    if (err != ERR_OK) {
        netconn_delete (nc);
        printf("Connection error\n");
        return;
    }

    // ip_addr_t ip;
    // HTTP_IP(&ip);
    // int port = HTTP_PORT;
    err = netconn_connect ( nc, &ip, port);
    if (err != ERR_OK) {
        netconn_delete (nc);
        printf("Connection error\n");
        return;
    }

    char data[512];
    sprintf(data,
            "%s %s HTTP/1.1\r\n"
            // "POST /rf HTTP/1.1\r\n"
            "User-Agent: esp-open-rtos/0.1 esp8266\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s", method, path, strlen(payload), payload);

    err = netconn_write(nc, data, strlen(data), NETCONN_COPY);
    if (err != ERR_OK) {
        printf("Connection writing\n");
    }

    netconn_close(nc);
    netconn_delete(nc);
}
