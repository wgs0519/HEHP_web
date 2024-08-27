#include <Arduino.h>

// 터치 핀 정의
const int touchPin = T0; // T0은 GPIO 4에 해당
unsigned long lastTouchTime = 0;
const unsigned long doubleTapTime = 3000; // 더블 탭 시간 간격

void setup() {
  Serial.begin(115200);
}

void loop() {
  int touchValue = touchRead(T0);

  if (touchValue < 100) { // 터치 감지 기준 값
    unsigned long currentTime = millis();

    if (currentTime - lastTouchTime < doubleTapTime) {
      Serial.println("flag!");
      delay(2000);
      lastTouchTime = 0; // 더블 탭 감지 후 초기화
    } else {
      lastTouchTime = currentTime;
    }
  }

}
