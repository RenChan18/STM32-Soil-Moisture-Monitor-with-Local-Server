#ifndef APP_LOG_H
#define APP_LOG_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "periphery.h"
#include "soil.h"

#include "string.h"

#define BUFFER_SIZE 128
#define LOG_DELAY_MS 100

typedef struct {
    uint8_t is_wet;
    uint8_t humidity_lvl;
} DataSoil_t;

extern QueueHandle_t xHumData;
extern QueueHandle_t xLogData;

void start_app(void);

void create_log(char *buf, size_t buf_size, DataSoil_t curr_humidity);
void send_log_queue(const char *log);
#endif // APP_LOG_H

