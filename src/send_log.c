#include "app_log.h"

void create_log(char *buf, size_t buf_size, DataSoil_t curr_humidity) {
    const char prefix[] = "[START_INFO] 2025-04-26 14:30:34 | Humidity is:";
    const char postfix[] = "\%[END_INFO]\r\n";

    size_t prefix_len = strlen(prefix);
    size_t postfix_len = strlen(postfix);

    memcpy(buf, prefix, prefix_len);
    buf+= prefix_len;
    buf_size -= prefix_len;
    
    int num = curr_humidity.humidity_lvl;
    if (num == 0) {
        *buf++ = '0';
    } else {
        char tmp[10];
        unsigned int i = 0;
        while (num > 0 && sizeof(tmp)-1) {
            tmp[i++] = '0' + (num % 10);
            num /= 10;
        }
        while (i-- > 0 && i < sizeof(tmp)-1) {
            *buf++ = tmp[i];
            buf_size--;
        }
    }
    memcpy(buf, postfix, postfix_len);
    buf+= postfix_len;
    buf_size -= postfix_len;
    *buf = '\0';
}

void send_log_queue(const char *log) {
    for (; *log; ++log) {
        while (xQueueSend(xLogData, log, pdMS_TO_TICKS(100)) != pdPASS) {
            vTaskDelay(1);
        }
    }

}







