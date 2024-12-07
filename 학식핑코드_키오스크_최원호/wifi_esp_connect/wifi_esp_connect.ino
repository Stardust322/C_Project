#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX 연결

void setup() {
  Serial.begin(9600); // 시리얼 통신시작
  mySerial.begin(9600); // 시리얼 통신속도 설정
}


// 시리얼 모니터의 입력한 AT명령어입력
// 1. AT - OK확인
// 2. AT+UART_DEF=9600,8,1,0,0_통신속도 맞춰주기
// 3. AT+CWLAP_주변와이파이 검색
// 4. AT+CWJAP="SSID","비밀번호"_와이파이 접속
void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}