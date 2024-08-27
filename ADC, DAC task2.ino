const int adcPin = 34; // 가변 저항이 연결된 ADC 핀 번호
const int dacPin = 25; // DAC 출력 핀 번호
const float targetVoltage = 1.65; // 목표 전압 값 (1.65V)
const int dacResolution = 255; // DAC 최대 값
const float maxVoltage = 3.3; // DAC 최대 전압

void setup() {
  Serial.begin(115200); // UART 통신 초기화
  pinMode(adcPin, INPUT);
  pinMode(dacPin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(adcPin); // 가변 저항 값 읽기
  int dacValue = map(sensorValue, 0, 4095, 0, dacResolution); // ADC 값을 0-255로 매핑하여 DAC 출력 값으로 변환

  dacWrite(dacPin, dacValue); // DAC 출력을 통해 아날로그 신호 제어

  float outputVoltage = (float)dacValue / dacResolution * maxVoltage; // 출력 전압 계산

  // UART로 현재 상태 출력
  Serial.print("ADC 값: ");
  Serial.print(sensorValue);
  Serial.print(" -> DAC 값: ");
  Serial.print(dacValue);
  Serial.print(" -> 출력 전압: ");
  Serial.println(outputVoltage);

  // 목표 전압 값에 도달했는지 확인
  if (abs(outputVoltage - targetVoltage) < 0.01) {
    Serial.println("Flag 획득!");
    while (true) {} // Flag 획득 성공 후 루프 중단
  }

  delay(1000); // 1초마다 값 갱신
}