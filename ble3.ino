#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  //Serial : ESP32와 USB로 연결된 PC사이의 통신
  //SerialBT : ESP32와 내장블루투스 모듈사이의 통신
  Serial.begin(115200);
  pinMode(15,OUTPUT);
  //블루투스 슬레이브 장비의 이름은 뭔가?
  SerialBT.begin("EXAMPLE");
  Serial.println("블루투스 통신 시작!");
}

void loop() {
  //PC에서 ESP32쪽으로 뭔가 데이터를 전송했다!
  if (Serial.available() > 0) {
    //PC로부터 ESP32가 수신한 1byte를 블루투스통신으로 전송하겠다!
    SerialBT.write(Serial.read());
  }
  //블루투스통신으로 뭔가가 ESP32로 입력이 되었다!
  if (SerialBT.available() > 0) {
    char cmd = SerialBT.read();
    if(cmd == '0'){
      //LED를 끈다
      digitalWrite(15,LOW);
    }else if(cmd == '1'){
      //LED를 켠다
      digitalWrite(15,HIGH);
    }

  }
  delay(20);
}