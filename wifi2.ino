#include <WiFi.h> //가해자 보드의 코드
#include <WebServer.h>
#include <HardwareSerial.h>

const char* ap_ssid = "ESP32_Attack";
const char* ap_password = "12345678";

WebServer server(80);
HardwareSerial MySerial(1);

int currentGuess[4] = {0, 0, 0, 0};

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 Password Attack</h1>";
  html += "<form action=\"/try\" method=\"GET\">";
  html += "Password: <input type=\"text\" name=\"password\" maxlength=\"4\"><br>";
  html += "<input type=\"submit\" value=\"Try\">";
  html += "</form>";
  html += "<p>Current Guess: ";
  for (int i = 0; i < 4; i++) {
    html += String(currentGuess[i]);
  }
  html += "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleTry() {
  if (server.hasArg("password")) {
    String password = server.arg("password");
    if (password.length() == 4) {
      for (int i = 0; i < 4; i++) {
        currentGuess[i] = password[i] - '0';
      }

      char guessStr[4];
      for (int i = 0; i < 4; i++) {
        guessStr[i] = currentGuess[i] + '0';
      }

      MySerial.write(guessStr, 4);

      delay(2000); // 2초 대기

      if (MySerial.available()) {
        int correctDigits = MySerial.read();
        String responseHtml = "<html><body><p>Correct Digits: " + String(correctDigits) + "</p>";
        responseHtml += "<a href=\"/\">Try Again</a></body></html>";
        server.send(200, "text/html", responseHtml);
        Serial.print("Current Guess: ");
        for (int i = 0; i < 4; i++) {
          Serial.print(currentGuess[i]);
        }
        Serial.print(" - Correct Digits: ");
        Serial.println(correctDigits);

        if (correctDigits == 4) {
          Serial.print("Password found: ");
          for (int i = 0; i < 4; i++) {
            Serial.print(currentGuess[i]);
          }
          Serial.println();
        }
      } else {
        server.send(200, "text/html", "<html><body><p>No response from victim device</p><a href=\"/\">Try Again</a></body></html>");
      }
    } else {
      server.send(200, "text/html", "<html><body><p>Invalid password length. Must be 4 digits.</p><a href=\"/\">Try Again</a></body></html>");
    }
  } else {
    server.send(200, "text/html", "<html><body><p>No password provided</p><a href=\"/\">Try Again</a></body></html>");
  }
}

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // RX1=16, TX1=17

  WiFi.softAP(ap_ssid, ap_password);

  server.on("/", handleRoot);
  server.on("/try", handleTry);
  server.begin();

  Serial.print("Created Hotspot, IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
}

----------------------------------------------------------------------------다음부터 피해자 보드의 코드

#include <HardwareSerial.h> //피해자 보드의 코드
#include <stdlib.h> // 랜덤 함수 사용을 위해 추가

HardwareSerial MySerial(1);

const int ledPin = 2;
int password[4]; // 비밀번호 배열

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200); // 시리얼 모니터 시작
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // RX1=16, TX1=17
  
  // 랜덤 시드 설정 (아날로그 핀 노이즈 활용)
  randomSeed(analogRead(0));

  // 비밀번호를 랜덤으로 생성
  for (int i = 0; i < 4; i++) {
    password[i] = random(0, 10); // 0에서 9 사이의 숫자
  }

  // 생성된 비밀번호 출력 (디버깅용)
  Serial.print("Generated Password: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(password[i]);
  }
  Serial.println();
}

void loop() {
  if (MySerial.available()) {
    char guess[4];
    int correctDigits = 0;

    MySerial.readBytes(guess, 4);

    for (int i = 0; i < 4; i++) {
      if (guess[i] - '0' == password[i]) {
        correctDigits++;
      }
    }

    for (int i = 0; i < correctDigits; i++) {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
    }

    if (correctDigits == 4) {
      Serial.println("Password Compromised!");
    }

    MySerial.write(correctDigits);
  }
}