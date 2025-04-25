#ifndef LOG_ENGINE_H
#define LOG_ENGINE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "periphery.h"
#include "soil.h"
#include "send_log.h"

#define BUFFER_SIZE 128
#define LOG_DELAY_MS 100

typedef struct {
    uint8_t is_wet;
    uint16_t humidity_lvl
} DataSoil_t;

static QueueHandle_t xHumData;
static QueueHandle_t xLogData;

static void scan_solid_task(void *unused);
static void logger_task(void *unused);
static void usart_tx_task(void *unused); 

#endif // LOG_ENGINE_H

