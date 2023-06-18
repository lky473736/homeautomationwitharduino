// 16조 
// 작품명 : 아두이노로 스마트홈 구현하기

// 모드 
// 0 : 커튼 내리기
// 1 : 커튼 올리기
// 2 : 전등 on/off
// 3 : 타이머
// 4 : 아침모드
// 5 : 밤모드
// 6 : 자동 밝기조정 모드
// 7 : 문 close / open

// 리모콘, IR -> 시리얼 모니터
// 5V 모터 -> 서보모터 1 (커튼 역할 함)

#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int room1 = 10;
int speaker = 11;
int led1 = 9;
int sensor = A1;
int dong = 12; // 초인종

Servo servo1; // 현관문
int angle1 = 0;

Servo servo2; // 커튼
int angle2 = 0;

void setup(){
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("SmartHome");
  lcd.setCursor(0, 1);
  lcd.print("TEAM 16");
  delay(1000);
  
  pinMode(room1, OUTPUT);
  digitalWrite(room1, LOW);
  
  pinMode(speaker, OUTPUT);
  digitalWrite(speaker, LOW);
  
  pinMode(led1, OUTPUT);
  
  servo1.attach(13); // 현관문
  servo2.attach(8); // 커튼
  
  pinMode(dong, INPUT_PULLUP);
  
  Serial.println("HELLO! This is Smart Home System.");
  Serial.println("Write the mode you want.");
}

void loop() {
  if (digitalRead(dong) == LOW) {
    digitalWrite(speaker, HIGH);
  } else {
    digitalWrite(speaker, LOW);
  }
  
  if (Serial.available()) {
    char input = Serial.read();
    Serial.print("Chosen Mode : ");
    Serial.println(input);

    switch (input) {
      case '0':
        lcd.clear();
        lcd.print("Blind Down");
        angle2 = 90;
        servo2.write(angle2);
        delay(5000);
        break;

      case '1':
        lcd.clear();
        lcd.print("Blind Up");
        angle2 = 0;
        servo2.write(angle2);
        delay(5000);
        break;

      case '2':
        lcd.clear();
        if (digitalRead(room1) == LOW) {
          lcd.print("Room1 Light On");
          digitalWrite(room1, HIGH);
        } else if (digitalRead(room1) == HIGH) {
          lcd.print("Room1 Light Off");
          digitalWrite(room1, LOW);
        }
        break;

      case '3':
        lcd.clear(); 
        lcd.print("Timer");
        Serial.println("What time? (s) : ");
      
        while (!Serial.available()) {
         // 사용자 입력이 없는 동안 대기
        }
      
      { int k = Serial.parseInt();
        Serial.print(k);
        Serial.println(" second");
        delay(k * 1000);
        
        digitalWrite(speaker, HIGH);
        delay(2000);
        digitalWrite(speaker, LOW);
        break;} // 변수 생성을 했기 때문에 중괄호 처리 (C++ 종특)

      case '4':
        lcd.clear();
        lcd.print("Morning Mode");
        digitalWrite(room1, HIGH);
        angle2 = 0;
        servo2.write(angle2);
        for (int i = 0; i < 5; i++) {
          digitalWrite(speaker, HIGH);
          delay(500);
          digitalWrite(speaker, LOW);
          delay(500);
        }
        break;

      case '5':
        lcd.clear();
        lcd.print("Night Mode");
        digitalWrite(room1, LOW);
        angle2 = 90;
        servo2.write(angle2);
        delay(5000);
        break;

      case '6':
      {
        lcd.clear();
        lcd.print("LA"); // Light Automation
        
        while(!Serial.available()){
          int lightLevel = analogRead(sensor);
          int brightness = map(lightLevel, 0, 1023, 0, 255);
          if(brightness>150){
            digitalWrite(led1, HIGH);}
          else{
            digitalWrite(led1, LOW);
          }
          
        }
      }

      case '7':
        lcd.clear();
        if (angle1 == 90) {
          lcd.print("Door Open");
          angle1 = 0;
          servo1.write(angle1);
        } else if (angle1 == 0) {
          lcd.print("Door Close");
          angle1 = 90;
          servo1.write(angle1);
        }
        break;
    }
  }
}
