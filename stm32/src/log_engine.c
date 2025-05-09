#include "app_log.h"
#include "stdio.h"

QueueHandle_t xLogData = NULL;
QueueHandle_t xHumData = NULL;

static void scan_solid_task(void *unused) {
    (void)unused;
     vTaskDelay(pdMS_TO_TICKS(1));
    DataSoil_t actual_data = {0,0};
    while(1) {
        actual_data.is_wet = read_soil_digital();
        actual_data.humidity_lvl = to_percent(read_soil_analog());
        vTaskDelay(100);
        gpio_toggle(GPIOD, GREEN_LED);
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
        if (xQueueReceive(xHumData, &received_data, portMAX_DELAY) == pdPASS) {
            memset(log, 0, BUFFER_SIZE);
            gpio_toggle(GPIOD, ORANGE_LED);
            create_log(log, BUFFER_SIZE, received_data);
            send_log_queue(log);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}

static void usart_tx_task(void *unused) {
    (void)unused;
    char c;
    while(1) {
        if(xQueueReceive(xLogData, &c, portMAX_DELAY) == pdPASS) {
            uint32_t timeout = 100000;
            while (!usart_get_flag(USART2, USART_SR_TXE) && -- timeout){
                vTaskDelay(1);
            }
            if (timeout)
                usart_send(USART2, c);
        }
    }
}
void start_app(void) {

    xHumData = xQueueCreate(32, sizeof(DataSoil_t));
    xLogData = xQueueCreate(128, sizeof(char));  
    
    if (xHumData == NULL) {
        while(1);
    }
    if (xLogData == NULL) {
        while(1);
    }

    xTaskCreate(scan_solid_task, "scan_solid", 512, NULL, tskIDLE_PRIORITY+2, NULL);
    xTaskCreate(logger_task, "logger", 512, NULL, tskIDLE_PRIORITY+2, NULL);
    xTaskCreate(usart_tx_task, "uart_tx", 512, NULL, tskIDLE_PRIORITY+1, NULL);
    
    gpio_clear(GPIOD, GREEN_LED | ORANGE_LED);

    vTaskStartScheduler();
}

