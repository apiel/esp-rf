
#ifndef __RF_H__
#define __RF_H__

#include <espressif/esp_common.h>
#include <queue.h>

#include "config.h"
#include "rf_receiver.h"

#define RF_CODE_SIZE RF_RESULT_SIZE+2 // -0 where o is protocol

void rf_task(void *pvParameters);

class Rf //: public esp_open_rtos::thread::task_t
{
    private:
        char cmd[1024];
        char last_code[RF_CODE_SIZE];
        unsigned int last_sent;

    public:
        QueueHandle_t rf_queue;
        void init();
        void onReceived(char * result);
        void consumer(char * code);
};

extern Rf rf;

#endif
