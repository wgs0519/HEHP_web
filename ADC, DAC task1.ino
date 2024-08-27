const int dacPin = 25; // DAC 출력 핀 번호

void setup() {
  Serial.begin(115200); // UART 통신 초기화
}

void loop() {
  for (int i = 0; i < 256 / 8; i++) { // 0V ~ 3.3V
    dacWrite(dacPin, i * 8); // DAC 값 증가
    Serial.print("\n");
    Serial.print(i);
    delay(10); // 0.01s 지연
  }
  for (int i = 255 / 8; i >= 0; i--) { // 3.3V ~ 0V
    dacWrite(dacPin, i * 8); // DAC 값 감소
    Serial.print("\n");
    Serial.print(i);
    delay(10); // 0.01s 지연
  }
}