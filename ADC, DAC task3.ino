const int adcPin = 34; // 가변 저항이 연결된 ADC 핀 번호
const int ledPin = 2;  // 내장된 LED 핀 번호
const int targetAdcValue = 2048; // 문제 출제자가 원하는 ADC 값의 중심
const int range = 50; // 목표 ADC 값 범위의 반경
const int stayDuration = 1000; // ADC 값 범위 내에 머무는 시간 (밀리초)

void setup() {
  Serial.begin(115200); // UART 통신 초기화
  pinMode(adcPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  static unsigned long startTime = 0;
  static bool inRange = false;
  
  int sensorValue = analogRead(adcPin); // 가변 저항 값 읽기
  int brightness = map(sensorValue, 0, 4095, 0, 255); // ADC 값을 0-255로 매핑

  analogWrite(ledPin, brightness); // 내장된 LED 밝기 조절

  Serial.print("가변 저항 값 (ADC): ");
  Serial.print(sensorValue);
  Serial.print(" -> LED 밝기: ");
  Serial.println(brightness);

  // 특정 ADC 값 범위에 도달했을 때 시간 체크 시작
  if (sensorValue >= (targetAdcValue - range) && sensorValue <= (targetAdcValue + range)) {
    if (!inRange) {
      startTime = millis(); // 범위에 처음 진입한 시간 기록
      inRange = true;
    }
    if (millis() - startTime >= stayDuration) {
      // 범위 내에 1초 동안 머물렀을 때
      Serial.println("Flag 획득!"); // Flag 획득 메시지 출력
      for (int i = 0; i < 5; i++) {
        analogWrite(ledPin, 255); // LED 최대 밝기
        delay(500); // 0.5초 켜짐
        analogWrite(ledPin, 0); // LED 끄기
        delay(500); // 0.5초 꺼짐
      }
      inRange = false; // 깜빡임 후 초기화
    }
  } else {
    inRange = false; // 범위를 벗어나면 초기화
  }

  delay(100); // 0.1초마다 값 갱신
}