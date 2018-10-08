#include <lwip/api.h>
#include <string.h>
#include <espressif/esp_common.h>

#include "config.h"

void post(char * code) {
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

    ip_addr_t ip;
    HTTP_IP(&ip);
    err = netconn_connect ( nc, &ip, HTTP_PORT );
    if (err != ERR_OK) {
        netconn_delete (nc);
        printf("Connection error\n");
        return;
    }

    char data[512];
    sprintf(data,
            "POST /rf HTTP/1.1\r\n"
            "User-Agent: esp-open-rtos/0.1 esp8266\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s", strlen(code), code);

    err = netconn_write(nc, data, strlen(data), NETCONN_COPY);
    if (err != ERR_OK) {
        printf("Connection writing\n");
    }

    netconn_close(nc);
    netconn_delete(nc);
}
