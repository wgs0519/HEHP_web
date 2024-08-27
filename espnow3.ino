//ESP32전용 ESPNOW라이브러리
#include <esp_now.h>
#include <WiFi.h>

#define btn1 18
#define btn2 19

//일정한 간격으로 메시지를 전송하기 위한 타이머값
unsigned long t = 0;

//내가 데이터를 보낼 녀석의 MAC주소
//보드A : 08:D1:F9:E0:2E:AC
//보드B : 08:D1:F9:E0:C0:A4

byte boardA[] = {0x08,0xD1,0xF9,0xE0,0x2E,0xAC};
byte boardB[] = {0x08,0xD1,0xF9,0xE0,0xC0,0xA4};
esp_now_peer_info_t dest;

void setup() {
  Serial.begin(115200);
  pinMode(btn1,INPUT_PULLUP); 
  pinMode(btn2,INPUT_PULLUP);
  //제일 처음 할일! 와이파이모드를 스테이션 모드로!
  WiFi.mode(WIFI_STA); 

  //ESPNOW의 기능을 활성화한다!
  if (esp_now_init() != 0) {
    //ESP NOW 시작 실패!
    Serial.println("ESPNOW 실패!");
    return;
  }else{
     //ESP NOW 시작 성공!
     Serial.println("ESPNOW 성공!");
  }

  //상대방 보드 주소를 대입한다!
  for(int i = 0;i<6;i++){
    dest.peer_addr[i] = boardB[i];
  }
  //상대방의 채널을 설정한다!
  dest.channel = 1;  
  dest.encrypt = false;

  //나의 peer를 등록한다!
  esp_now_add_peer(&dest);

  //송신완료 콜백함수 등록
  esp_now_register_send_cb(OnDataSent);

  //수신완료 콜백 함수 등록
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(btn1) == LOW){
    //버튼이 눌려졌음!
    byte data = '0';
    esp_now_send(dest.peer_addr, &data, sizeof(data));
    //버튼을 1회 누르면 1회 메시지를 전송하겠다!
    delay(300); //이방식은 권장하는 방식은 아님!
  }
  if(digitalRead(btn2) == LOW){
    //버튼이 눌려졌음!
    byte data = '1';
    esp_now_send(dest.peer_addr, &data, sizeof(data));
    //버튼을 1회 누르면 1회 메시지를 전송하겠다!
    delay(300); //이방식은 권장하는 방식은 아님!
  }
}

//송신콜백함수 원형:
//void OnDataSent(uint8_t *mac, uint8_t status) {
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
   //status가 0일때 송신 완료!
   if(status == 0){
      Serial.println("성공적으로 송신했음!");
   }else{
      Serial.println("송신 실패!!");
   }
}


//수신콜백함수 원형:
//void OnDataRecv(uint8_t * mac, uint8_t * data, uint8_t len) {
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  //data 수신 데이터
  //len 수신한 데이터의 길이
}
----------------------------------------------여기까지 board A, 다음부터 board B

//ESP32전용 ESPNOW라이브러리
#include <esp_now.h>
#include <WiFi.h>

#define led 15


//일정한 간격으로 메시지를 전송하기 위한 타이머값
unsigned long t = 0;

//내가 보낼 메시지
char data[] = "나는 boardB보드이다!!";

//내가 데이터를 보낼 녀석의 MAC주소
//보드A : 08:D1:F9:E0:2E:AC
//보드B : 08:D1:F9:E0:C0:A4

byte boardA[] = {0x08,0xD1,0xF9,0xE0,0x2E,0xAC};
byte boardB[] = {0x08,0xD1,0xF9,0xE0,0xC0,0xA4};
esp_now_peer_info_t dest;

void setup() {
  Serial.begin(115200);
  
  //제일 처음 할일! 와이파이모드를 스테이션 모드로!
  WiFi.mode(WIFI_STA); 

  //ESPNOW의 기능을 활성화한다!
  if (esp_now_init() != 0) {
    //ESP NOW 시작 실패!
    Serial.println("ESPNOW 실패!");
    return;
  }else{
     //ESP NOW 시작 성공!
     Serial.println("ESPNOW 성공!");
  }

  //상대방 보드 주소를 대입한다!
  for(int i = 0;i<6;i++){
    dest.peer_addr[i] = boardA[i];
  }
  //상대방의 채널을 설정한다!
  dest.channel = 1;  
  dest.encrypt = false;

  //나의 peer를 등록한다!
  esp_now_add_peer(&dest);

  //송신완료 콜백함수 등록
  esp_now_register_send_cb(OnDataSent);

  //수신완료 콜백 함수 등록
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(millis() - t > 2000){
    t = millis();
    //2초마다 한번씩 하겠다!
    esp_now_send(dest.peer_addr, (uint8_t *) &data, sizeof(data));
  }
  
}

//송신콜백함수 원형:
//void OnDataSent(uint8_t *mac, uint8_t status) {
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
   //status가 0일때 송신 완료!
   if(status == 0){
      Serial.println("성공적으로 송신했음!");
   }else{
      Serial.println("송신 실패!!");
   }
}


//수신콜백함수 원형:
//void OnDataRecv(uint8_t * mac, uint8_t * data, uint8_t len) {
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  if(data[0] == '0'){
    //LED OFF
    digitalWrite(led,LOW);
  }else if(data[0] == '1'){
    //LED ON
    digitalWrite(led,HIGH);
  }
}