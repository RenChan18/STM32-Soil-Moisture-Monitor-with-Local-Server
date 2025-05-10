#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "string.h"
#include "stdio.h"
#include "ctype.h"

#define UART_NUM        UART_NUM_1
#define UART_HW_TX        (GPIO_NUM_2)
#define UART_SW_RX      (GPIO_NUM_13)
#define UART_BAUD_RATE   9600
#define BIT_PERIOD_US   (1000000/UART_BAUD_RATE)
#define BUF_SIZE        128
#define MAX_LINE_LENGHT     64

static QueueHandle_t uart_txq, uart_rxq;
static QueueHandle_t uartQueue;
static const char *TRANSMIT = "TX_UART";
static const char *RECEIVE = "RX_UART";

void gpio_init(void) {
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << UART_SW_RX),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_config);
}

void uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}



void uart_rx_task(void * pvParameters) {
    uint8_t rx_byte;
    char str_buf[BUF_SIZE];
    int idx = 0;

    memset(str_buf, 0, BUF_SIZE);
    ESP_LOGI(RECEIVE, ">>> SOFTWARE RX");
    while(1) {
        esp_task_wdt_reset();
        if(gpio_get_level(UART_SW_RX) == 0) {
            ets_delay_us(BIT_PERIOD_US / 2);
            rx_byte = 0;
            for (int i = 0; i < 8; i++) {
                ets_delay_us(BIT_PERIOD_US);
                int bit = gpio_get_level(UART_SW_RX);
                rx_byte |= (bit << i);
                esp_task_wdt_reset();
            }
            ets_delay_us(BIT_PERIOD_US);
            if (rx_byte == '\n' || rx_byte == '\r') {
                ESP_LOGI(RECEIVE, ">>> READ_DATA");
                if (idx > 0) {
                    str_buf[idx] = '\0';
                    ESP_LOGI(RECEIVE, "RECEIVED LINE: %s", str_buf);
                    if (xQueueSend(uartQueue, str_buf, portMAX_DELAY) != pdPASS) {
                        ESP_LOGE(RECEIVE, "Error queue owerflow!!!");
                    }
                    idx = 0;
                    memset(str_buf, 0, BUF_SIZE);
                }
            } else {
                if (idx < BUF_SIZE - 1) {
                    str_buf[idx++] = rx_byte;
                } else {
                    str_buf[idx] = '\0';
                    ESP_LOGI(RECEIVE, "RECEIVED OVERFLOW LINE: %s", str_buf);
                    idx = 0;
                    memset(str_buf, 0, BUF_SIZE);
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

void uart_tx_task(void * pvParameters) {
    char tx_buf[BUF_SIZE];
    ESP_LOGI(TRANSMIT, ">>> SOFTWARE TX");
    while(1) {
        if (xQueueReceive(uartQueue, tx_buf, portMAX_DELAY) == pdPASS) {
            size_t len = strlen(tx_buf);
            int sent = uart_write_bytes(UART_NUM, tx_buf, len);
            uart_write_bytes(UART_NUM, "\r\n", 2);
            if (sent == len) {
                ESP_LOGI(TRANSMIT, "Data sent successfully: %s", tx_buf);
            } else {
                ESP_LOGE(TRANSMIT, "FAILED TO SEND DATA");
            }
        }
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void) {
    gpio_init();
    uart_init();

    uartQueue = xQueueCreate(1, BUF_SIZE);
    if (uartQueue == NULL) {
        ESP_LOGE("QUEUE", "Couldn't create queue!");
        return;
    }
    xTaskCreate(uart_rx_task, "read_uart", 4096, NULL, 7, NULL);
    xTaskCreate(uart_tx_task, "write_uart", 4096, NULL, 5, NULL);
}




