#include "log_engine.h"

static void scan_solid_task(void *unused) {
    DataSoil_t actual_data = {0,0};

    while(1) {
        actual_data.is_wet = read_soil_digital();
        actual_data.humidity_lvl = to_percent(read_soil_analog());
    }
}

static void logger_task(void *unused) {
    char log[BUFFER_SIZE] = {0};

    while(1) {
        memset(log, 0, BUFFER_SIZE);
        create_log(log, BUFFER_SIZE);
        print_log_queue(log);
        vTaskDelay(pdMS_TO_TICKS(LOG_DELAY_MS));
    }
}

static void start_app() {  
    xHumData = xQueueCreate(128, sizeof(DataSoil_t));
    
    xTaskCreate(scan_solid_task, "scan_solid", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(logger_task, "logger", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(usart_tx_task, "uart_tx", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    vTaskStartSheduler();
}

