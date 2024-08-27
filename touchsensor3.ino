#include <Arduino.h>

// 핀 정의
const int touchPin = T0; // 터치 센서 핀 (T0은 GPIO 4에 해당)
const int ledPin = 2;   // LED 핀 (GPIO 2)
int touchValue = 0;     // 터치 센서 값
int ledBrightness = 0;  // LED 밝기 값 (0-255)

// PWM 설정
const int freq = 50; // PWM 주파수 설정 (50 Hz)
const int ledChannel = 0; // PWM 채널
const int resolution = 8; // PWM 해상도 (8비트, 0-255)

void setup() {
  // 시리얼 통신 시작
  Serial.begin(115200);

  // LED 핀을 PWM 출력을 위해 설정
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // 터치 센서 값을 읽어오기
  touchValue = touchRead(touchPin);

  // 터치 센서 값을 LED 밝기로 매핑 (0에서 최대 터치 값을 0에서 255까지의 LED 밝기로 매핑)
  // 50은 임의의 최대 터치 값, 필요에 따라 조정 가능
  ledBrightness = map(touchValue, 0, 107, 0, 255);

  // LED 밝기 설정 (constrain 함수로 값 제한)
  ledBrightness = constrain(ledBrightness, 0, 255);
  ledcWrite(ledChannel, ledBrightness);

  
  // 시리얼 모니터에 터치 값과 LED 밝기 값 출력
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  Serial.print("  LED Brightness: ");
  Serial.println(ledBrightness);

  // LED 밝기가 최대 값인 경우 "flag!" 출력
  if (ledBrightness == 255) {
    Serial.println("flag!");
  }

  delay(1000);
}

