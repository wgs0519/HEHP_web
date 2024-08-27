#include <Arduino.h>
#include "driver/uart.h"

#define GPIO_INPUT_IO_0     18
#define ECHO_TEST_TXD       4
#define ECHO_TEST_RXD       5

int trigger = 0;

char hardware_flag_3[38] = "flag{this_is_hardware_flag3}";

void hardware_task3(void *pvParameters) {
    Serial.printf("[+] hardware task III : find the third flag in another UART\n");
    while (1) {
        uart_write_bytes(UART_NUM_1, hardware_flag_3, strlen(hardware_flag_3));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);

    pinMode(GPIO_INPUT_IO_0, INPUT);

    // UART 설정
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART
