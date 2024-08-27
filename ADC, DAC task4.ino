const int adcPin = 34; // 가변 저항이 연결된 ADC 핀 번호
const int ledPin = 2;  // 내장된 LED 핀 번호
const int range = 50; // 목표 ADC 값 범위의 반경
const int stayDuration = 1000; // ADC 값 범위 내에 머무는 시간 (밀리초)
const int numAttempts = 3; // 성공해야 하는 시도 횟수

int targetAdcValue; // 목표 ADC 값의 중심
int attempts = 0; // 성공한 시도 횟수
bool inRange = false; // 목표 범위 내에 있는지 여부
unsigned long startTime = 0; // 목표 범위 내에 진입한 시간

void setup() {
  Serial.begin(115200); // UART 통신 초기화
  pinMode(adcPin, INPUT);
  pinMode(ledPin, OUTPUT);
  randomSeed(analogRead(0)); // 랜덤 시드 초기화
  setNewTarget(); // 첫 번째 목표 설정
}

void loop() {
  if (attempts >= numAttempts) {
    return; // 3번의 시도를 모두 성공하면 루프 종료
  }

  int sensorValue = analogRead(adcPin); // 가변 저항 값 읽기

  // 목표 범위에 가까워질수록 LED 밝기가 밝아지도록 계산
  int brightness;
  if (sensorValue < targetAdcValue - range) {
    brightness = map(sensorValue, 0, targetAdcValue - range, 0, 127); // 목표 범위 아래일 때 밝기 조절
  } else if (sensorValue > targetAdcValue + range) {
    brightness = map(sensorValue, targetAdcValue + range, 4095, 127, 0); // 목표 범위 위일 때 밝기 조절
  } else {
    brightness = 255; // 목표 범위 내일 때 최대 밝기
  }

  analogWrite(ledPin, brightness); // 내장된 LED 밝기 조절

  Serial.print("가변 저항 값 (ADC): ");
  Serial.print(sensorValue);
  Serial.print(" -> LED 밝기: ");
  Serial.println(brightness);

  // 목표 범위 내에 1초 동안 머무를 때 체크
  if (sensorValue >= (targetAdcValue - range) && sensorValue <= (targetAdcValue + range)) {
    if (!inRange) {
      startTime = millis(); // 범위에 처음 진입한 시간 기록
      inRange = true;
    }
    if (millis() - startTime >= stayDuration) {
      // 범위 내에 1초 동안 머물렀을 때
      attempts++;
      Serial.println("Flag 획득 시도 성공!");
      blinkLed(5); // LED 5번 깜빡임

      if (attempts >= numAttempts) {
        Serial.println("Flag 획득!"); // 최종 Flag 획득 메시지 출력
        while (true) {} // 프로그램 종료
      } else {
        setNewTarget(); // 새로운 목표 설정
      }

      inRange = false; // 초기화
    }
  } else {
    inRange = false; // 범위를 벗어나면 초기화
  }

  delay(100); // 0.1초마다 값 갱신
}

void setNewTarget() {
  targetAdcValue = random(500, 3600); // 새로운 목표 ADC 값 설정 (500 ~ 3600 사이)
  Serial.print("새로운 목표 ADC 값 범위: ");
  Serial.print(targetAdcValue - range);
  Serial.print(" ~ ");
  Serial.println(targetAdcValue + range);
}

void blinkLed(int times) {
  for (int i = 0; i < times; i++) {
    analogWrite(ledPin, 255); // LED 최대 밝기
    delay(500); // 0.5초 켜짐
    analogWrite(ledPin, 0); // LED 끄기
    delay(500); // 0.5초 꺼짐
  }
}