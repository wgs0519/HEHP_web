#include <Arduino.h>

// 터치 핀 정의
const int touchPin = T0; // T0은 GPIO 4에 해당

bool flagCaptured = false;
unsigned long touchStartTime = 0;
const unsigned long requiredTouchTime = 5000; // 5초

void setup() {
  Serial.begin(115200);
}

void loop() {
  int touchValue = touchRead(T0);
  
  // 터치 센서 값이 특정 값 이하일 때 터치가 감지된 것으로 처리
  if (touchValue < 100) { // 100은 터치 감지 기준 값으로 조정 가능
    if (touchStartTime == 0) {
      touchStartTime = millis();
    } else if (millis() - touchStartTime >= requiredTouchTime) {
      flagCaptured = true;
      touchStartTime = 0;
    }
  } else {
    touchStartTime = 0;
  }

  if (flagCaptured) {
    Serial.println("flag");
    delay(5000); // 메시지 출력을 위한 대기 시간
    flagCaptured = false;
  }
}
