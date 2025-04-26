#include "app_log.h"

static void scan_solid_task(void *unused) {
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
    char c;
    while(1) {
        if(xQueueReceive(xLogData, &c, portMAX_DELAY) == pdPASS) {
            while (!usart_get_flag(USART2, USART_SR_TXE));
            usart_send(USART2, ch);
        }
    }
}

static void start_app() {  
    xHumData = xQueueCreate(128, sizeof(DataSoil_t));
    xLogData = xQueueCreate(128, sizeof(DataSoil_t));
    
    xTaskCreate(scan_solid_task, "scan_solid", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(logger_task, "logger", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(usart_tx_task, "uart_tx", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    vTaskStartSheduler();
}

