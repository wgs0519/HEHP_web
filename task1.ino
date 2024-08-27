#include <Arduino.h>

#define GPIO_INPUT_IO_0     18

int trigger = 0;

char hardware_flag_1[39] = "You capture the flag !!";

void IRAM_ATTR gpio_isr_handler() {
    trigger++;
}

void setup() {
    Serial.begin(115200);

    pinMode(GPIO_INPUT_IO_0, INPUT);
    attachInterrupt(digitalPinToInterrupt(GPIO_INPUT_IO_0), gpio_isr_handler, RISING);

    // UART 설정
    Serial1.begin(115200, SERIAL_8N1, ECHO_TEST_RXD, ECHO_TEST_TXD);
}

void hardware_task1() {
    int hit = 0;
    while (1) {
        Serial.printf("[+] hardware task I : hit %d\n", hit);
        if (digitalRead(GPIO_INPUT_IO_0)) {
            hit++;
        } else {
            hit = 0;
        }
        if (hit > 3) {
            Serial.printf("[+] hardware task I : %s\n", hardware_flag_1);

            break;
        }
        delay(1000);
    }
}

void loop() {
    hardware_task1();
}
