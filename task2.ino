#include <Arduino.h>

#define GPIO_INPUT_IO_0     18
#define ECHO_TEST_TXD       4
#define ECHO_TEST_RXD       5

int trigger = 0;

char hardware_flag_2[39] = "You capture the flag !!";

void IRAM_ATTR gpio_isr_handler() {
    trigger++;
}

void hardware_task2(void *pvParameters) {
    trigger = 0;
    while (1) {
        Serial.printf("[+] hardware task II : trigger %d\n", trigger);
        if (trigger > 10000) {
            Serial.printf("[+] hardware task II : %s\n", hardware_flag_2);
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);

    pinMode(GPIO_INPUT_IO_0, INPUT);
    attachInterrupt(digitalPinToInterrupt(GPIO_INPUT_IO_0), gpio_isr_handler, RISING);

    // UART 설정
    Serial1.begin(115200, SERIAL_8N1, ECHO_TEST_RXD, ECHO_TEST_TXD);

    // FreeRTOS 태스크 생성
    xTaskCreate(hardware_task2, "hardware_task2", 2048, NULL, 1, NULL);
}

void loop() {
    // loop 함수는 비워둡니다. 모든 작업은 FreeRTOS 태스크에서 처리됩니다.
}
