#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
// 블링크 템플릿, 키, 토큰 정보 입력
#define BLYNK_TEMPLATE_ID "TMPL6ublFYHVI" 
#define BLYNK_TEMPLATE_NAME "test" 
#define BLYNK_AUTH_TOKEN "N-5yKy_zSGH1VT0Dzx5El2ioFqkP2VGs"

#include <ESP8266_Lib.h> //ESP8266 제어를 위한 라이브러리
#include <BlynkSimpleShieldEsp8266.h> // Blynk와 ESP8266 연동 라이브러리
#include <Servo.h>  // 서보 모터 라이브러리

//와이파이 정보
char ssid[] = "iPhone (57)";
char pass[] = "12345678";

// Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // ESP의 RX, TX를 2,3번 핀에 연결

#define ESP8266_BAUD 9600 // ESP8266의 통신속도 설정

ESP8266 wifi(&EspSerial); // ESP8266 객체 생성

// 초기 각도 설정
int default_Angle = 144;  // 기본 각도_off 각도
int drop_Angle = 60;      // 가림막 내린 각도_on 각도

// 상태 플래그 (각도 전환 여부)
bool flag1 = false;  // servo1 & servo2
bool flag2 = false;  // servo3 & servo4
bool flag3 = false;  // servo5 & servo6

// 서보 모터 객체 생성
Servo servo1, servo2, servo3, servo4, servo5, servo6;

// 서보 모터 제어 함수
void handleServoAction(Servo &servoA, Servo &servoB, bool &flag, int pin)
{
  flag = !flag;  // 버튼을 누를 때마다, 상태 전환
  int angle = flag ? drop_Angle : default_Angle; // 삼항연산자로 조건에 따른 각도 입력
  servoA.write(angle); // drop_Angle이 입력된다.
  servoB.write(180 - angle);  // 반대 방향
  
  // 간단한 상태 출력
  Serial.printf("Servo %d, %d Angle: %d\n",pin,pin+1,angle);

}

// 버튼으로 servo1 & servo2 제어
BLYNK_WRITE(V6) {
  handleServoAction(servo1, servo2, flag1, 9); // 핀 9
}

// 버튼으로 servo3 & servo4 제어
BLYNK_WRITE(V4) {
  handleServoAction(servo3, servo4, flag2, 11); // 핀 11
}

// 버튼으로 servo5 & servo6 제어
BLYNK_WRITE(V7) {
  handleServoAction(servo5, servo6, flag3, 5); // 핀5
}

// 한번 실행되는 setup()함수
void setup() {
// Debug console
Serial.begin(9600); // 시리얼 통신 시작
EspSerial.begin(ESP8266_BAUD); // ESP8266 통신 시작
delay(10);

Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass); // Blynk 서버 연결

// 서보 모터를 Arduino 핀에 연결
servo1.attach(9);
servo2.attach(10);
servo3.attach(11);
servo4.attach(6);
servo5.attach(5);
servo6.attach(3);

// 초기 각도 설정(가림막이 올라간 상태)
// 마주보는 서보모터끼리 같은 방향 움직이게 설정
servo1.write(default_Angle);
servo2.write(180 - default_Angle);
servo3.write(default_Angle);
servo4.write(180 - default_Angle);
servo5.write(default_Angle);
servo6.write(180 - default_Angle);
}

void loop() {
Blynk.run(); // 블링크앱과 실시간 연결 및 동작실행
}