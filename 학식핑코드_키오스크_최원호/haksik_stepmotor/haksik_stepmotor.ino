// Blynk 디버깅 메시지를 Serial Monitor에 출력
#define BLYNK_PRINT Serial

#include <BlynkSimpleShieldEsp8266.h> // Blynk와 ESP8266 연동을 위한 라이브러리
#include <ESP8266_Lib.h> // ESP8266 모듈 제어를 위한 라이브러리
#include <Stepper.h> // 스텝모터제어를 위한 라이브러리

// Blynk 템플릿 및 토큰 정보
#define BLYNK_TEMPLATE_ID "TMPL6ublFYHVI"
#define BLYNK_TEMPLATE_NAME "Menu Cover"
#define BLYNK_AUTH_TOKEN "N-5yKy_zSGH1VT0Dzx5El2ioFqkP2VGs"

// 와이파이 정보
char ssid[] = "Phone (57)";
char pass[] = "12345678";

// ESP8266 세팅
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // ESP8266의 RX, TX를 2,3번 핀에 연결
#define ESP8266_BAUD 9600 // ESP8266의 통신속도 설정
ESP8266 wifi(&EspSerial); // ESP8266 객체 생성

// 스텝 모터 초기화
#define STEPS_PER_REV 2048 // 28BYJ-48 기준
Stepper motor1(STEPS_PER_REV, 8, 10, 9, 11);  // 1번 모터 핀
Stepper motor2(STEPS_PER_REV, 4, 6, 5, 7);    // 2번 모터 핀
Stepper motor3(STEPS_PER_REV, 12, 13, A0, A1); // 3번 모터 핀

// 동작 설정
int stepsToMove = 512; // 한 번 동작할때 필요한 스텝 수 (1/4 회전)
int motorSpeed = 10;   // 초기 속도 (RPM)

// 모터 제어 함수
void handleMotorAction(Stepper &motor, int state, const char* motorName) 
{
  motor.setSpeed(motorSpeed); // 슬라이더로 설정된 속도 적용

  if (state == 1) 
  { // 상태 ON일때
    motor.step(stepsToMove); // 시계 방향 회전 (내리기)
    Serial.printf("%s: 메뉴 품절 (시계 방향)\n", motorName);    // 상태출력
  } else { // 상태 OFF일 때
    motor.step(-stepsToMove); // 반시계 방향 회전 (올리기)
    Serial.printf("%s: 메뉴 판매 (반시계 방향)\n", motorName);
  }
}

// 메뉴 1: 내리기(ON) 및 올리기(OFF)
BLYNK_WRITE(V1) {
  int state = param.asInt(); // 버튼 상태 (0 = OFF, 1 = ON)
  handleMotorAction(motor1, state, "뚝배기");
}

// 메뉴 2: 내리기(ON) 및 올리기(OFF)
BLYNK_WRITE(V2) {
  int state = param.asInt(); // 버튼 상태 
  handleMotorAction(motor2, state, "덮밥");
}

// 메뉴 3: 내리기(ON) 및 올리기(OFF)
BLYNK_WRITE(V3) {
  int state = param.asInt(); // 버튼 상태 (0 = OFF, 1 = ON)
  handleMotorAction(motor3, state, "양식");
}

// 슬라이더로 모터 속도 설정 (공통 속도 적용)
BLYNK_WRITE(V4) {
  motorSpeed = param.asInt(); // 슬라이더 값 가져오기
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);

  Serial.printf("모터 속도 변경: %d RPM\n", motorSpeed); // 변경된 속도출력
}

void setup() {
	//시리얼 통신 및 와이파이 통신 세팅
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

	// 블링크와 online연결
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  // 모터 초기화
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);

  Serial.println("System Ready");
}

void loop() {
  Blynk.run();  // 블링크앱과 실시간 연결 및 동작실행
}
