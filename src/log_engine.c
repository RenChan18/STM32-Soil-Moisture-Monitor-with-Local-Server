#include "app_log.h"
#include "stdio.h"

QueueHandle_t xLogData = NULL;
QueueHandle_t xHumData = NULL;

static void scan_solid_task(void *unused) {
    (void)unused;
    DataSoil_t actual_data = {0,0};

    while(1) {
        actual_data.is_wet = read_soil_digital();
        actual_data.humidity_lvl = to_percent(read_soil_analog());
        while (xQueueSend(xHumData, &actual_data, pdMS_TO_TICKS(100)) != pdPASS) {
            vTaskDelay(1);
        }
    }
}

static void logger_task(void *unused) {
    (void)unused;
    char log[BUFFER_SIZE] = {0};
    DataSoil_t received_data;

    while(1) {
        if (xQueueReceive(xHumData, &received_data, pdMS_TO_TICKS(100)) == pdPASS) {
            memset(log, 0, BUFFER_SIZE);
            create_log(log, BUFFER_SIZE, received_data);
            send_log_queue(log);
            vTaskDelay(pdMS_TO_TICKS(LOG_DELAY_MS));
        }
    }
}

static void usart_tx_task(void *unused) {
    (void)unused;
    char c;
    while(1) {
        if(xQueueReceive(xLogData, &c, portMAX_DELAY) == pdPASS) {
            while (!usart_get_flag(USART2, USART_SR_TXE));
            usart_send(USART2, c);
        }
    }
}

void start_app(void) {
    // Создаем очереди с проверкой
    xHumData = xQueueCreate(32, sizeof(DataSoil_t));
    xLogData = xQueueCreate(128, sizeof(char));  // Для USART достаточно 128 символов
    
    /*if (xHumData == NULL) {
    printf("ERROR: xHumData NULL\r\n");
    while(1);
    }
    if (xLogData == NULL) {
    printf("ERROR: xLogData NULL\r\n");
    while(1);
    }*/
    // Увеличиваем размер стека
    xTaskCreate(scan_solid_task, "scan_solid", 128, NULL, tskIDLE_PRIORITY+4, NULL);
    xTaskCreate(logger_task, "logger", 128, NULL, tskIDLE_PRIORITY+2, NULL);
    xTaskCreate(usart_tx_task, "uart_tx", 128, NULL, tskIDLE_PRIORITY+1, NULL);
    
    vTaskStartScheduler();
}

