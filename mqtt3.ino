#include <WiFi.h>
#include <PubSubClient.h>

#define led 18

//WiFi공유기의 아이디 비밀번호
const char* ssid = "iptime302";
const char* password = "dbsdbs1448?!";
//브로커의 주소
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

unsigned long mytime = 0;
long limit = 0;

//인터넷 공유기와 접속해라
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//데이터 수신 처리부
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  //payload에 들어오는 숫자를 문자열로 수신할 것
  String mypayload = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mypayload += (char)payload[i];
  }
  Serial.println();
  //payload안에 들어있는 문자열로된 숫자를 사칙연산이 가능한 숫자로 바꿀것
  limit = mypayload.toInt(); //대략 32초정도 입력이 가능하겠군!
  //LED가 켜진 시간 측정하기!
  mytime = millis();
  //LED를 켠다
  digitalWrite(led,HIGH);
}

//나의 ESP32가 MQTT서버와 접속하는 부분!
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //ESP32가 서버와 연결을 완전이 확정하는 부분
      Serial.println("connected");
      //서버와 연결되었으니 구독신청을 하겠다!
      client.subscribe("test/esp1/input");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  setup_wifi(); //인터넷 공유기와 접속할것!
  client.setServer(mqtt_server, 1883);//MQTT서버의 설정!
  client.setCallback(callback); //데이터수신함수 등록
}

void loop() {
  //ESP32와 MQTT서버가 연결을 유지하기 위한 부분(삭제불가)
  if (!client.connected()) {
    //서버와 ESP32간에 연결이 되어있지 않다면 연결하겠다!
    reconnect();
  }
  client.loop();

  //만약에 LED1번이 켜져있는 상태라면 limit경과후 끄겠다!
  if(digitalRead(led) == HIGH){
    if(millis() - mytime > limit){
      //led가 켜진다음 limit 밀리초만큼 지났다!
      digitalWrite(led,LOW);
    }
  }

  //2초간격으로 test메시지를 발행하는 예제
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    //TOPIC하고 PAYLOAD를 발행하겠다!
    client.publish("test/esp32/output", msg);
    
  }
}