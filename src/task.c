#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static void start_app() {  
    xTaskCreate(scan_solid_task, "scan_solid", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(logger_task, "logger", 256, NULL, tskIDLE_PRIORITY+1, NULL);
}
