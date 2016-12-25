#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIN1 22
#define NEOPIN2 23
#define NEOPIN3 24
#define NEOPIN4 25

#define RepeatTime 5    //5초마다 날씨 제어

String dataFromBluetooth;
AlarmId threeHourAlarmId;

int preLED = 0;    //이전 loop에서 켰던 LED
int nowLED = 0;
int setWait = 20;

//네오픽셀을 사용하기 위해 객체를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, NEOPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(5, NEOPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(5, NEOPIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(5, NEOPIN4, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  printTime();
  setStrip();
}

void loop() {
  char ch;
   if (Serial.available()) {
    ch = Serial.read();
  }
  switch(ch) {
    //기본값
    case '1' :
    break;

    //블루투스 읽기
    case '2' :
    Serial.println("Data 2 recieved");
    dataFromBluetooth = "(2016-12-23 15:00:00 Clear -4)(2016-12-23 18:00:00 Clouds -6)(2016-12-23 21:00:00 Clear -9)(2016-12-24 00:00:00 Clouds -7)(2016-12-24 03:00:00 Clear 0)(2016-12-24 06:00:00 Clear 2)(2016-12-24 09:00:00 Clouds -3)(2016-12-24 12:00:00 Clouds -7)(2016-12-24 15:00:00 Clear -9)(2016-12-24 18:00:00 Clouds -10)(2016-12-24 21:00:00 Clear -10)(2016-12-25 00:00:00 Clouds -7)(2016-12-25 03:00:00 Clouds 3)(2016-12-25 06:00:00 Clouds 5)(2016-12-25 09:00:00 Clouds 1)(2016-12-25 12:00:00 Clouds -2)(2016-12-25 15:00:00 Clouds -2)(2016-12-25 18:00:00 Clouds -3)(2016-12-25 21:00:00 Clouds -3)(2016-12-26 00:00:00 Clouds -1)END";
    dataFromBluetooth = "(2016-12-24 18:00:00 Clouds -10)(2016-12-24 21:00:00 Clear -10)(2016-12-25 00:00:00 Rain -7)(2016-12-25 00:00:00 Snow -7)END";
    //dataFromBluetooth = "(Clouds -2)(Clouds -3)(Clouds -3)(Clouds -1)END";
    Repeats();      //Repeat 한번 호출
    threeHourAlarmId =  Alarm.timerRepeat(RepeatTime, Repeats);  //RepeatTime초마다 Repeat호출
    ch = 3; //LED 제어로 상태 바꿈
    break;
    
    //LED 제어
    case '3' :
    break;
  }//switch 끝
  
  delay(1000);
  Alarm.delay(1000);
}

void  Repeats(){
  String nowData;
  String weather;
  String temp;
  Serial.println("------------Repeats() Function-----------");
  printTime();
  Serial.print(RepeatTime);
  Serial.println(" second timer");
  
  //받아온 데이터의 첫번째 부분만 꺼내기
  nowData = dataFromBluetooth.substring(dataFromBluetooth.indexOf("("),dataFromBluetooth.indexOf(")")+1);
  Serial.println("nowData : " + nowData);  
  //첫번째 부분 제거
  dataFromBluetooth = dataFromBluetooth.substring(dataFromBluetooth.indexOf(nowData) + nowData.length());
  Serial.println("dataFromBluetooth : " + dataFromBluetooth);

  //첫번째 부분에서 날짜 제거
  nowData = nowData.substring(nowData.indexOf(" ") + 1);
  //첫번째 부분에서 시간 제거
  nowData = nowData.substring(nowData.indexOf(" ") + 1, nowData.indexOf(")"));
  //첫번째 부분의 날씨
  //weather = nowData.substring(nowData.indexOf("(") + 1,nowData.indexOf(" "));
  weather = nowData.substring(0,nowData.indexOf(" "));
  //첫번째 부분의 기온
  //temp = nowData.substring(nowData.indexOf(" ") + 1);
  temp = nowData.substring(nowData.indexOf(" ") + 1);
  //마지막 데이터일 경우가 아닐 경우만  
  //첫번째 부분으로 LED 제어(마지막 데이터
  if(dataFromBluetooth.startsWith("END")) {
    Serial.println("Last Data");
    Alarm.free(threeHourAlarmId);
  }
  controlLED(weather, temp.toInt());
  Serial.print("nowData : ");
  Serial.println(nowData);
  Serial.print("weather : ");
  Serial.println(weather);
  Serial.print("temp : ");
  Serial.println(temp.toInt());
  
  //마지막 데이터일 경우(데이터가END로 시작함)
  if(dataFromBluetooth.startsWith("END")) {
    Serial.println("Last Data");
    Alarm.free(threeHourAlarmId);
  }
  Serial.println("-----------------------------------------");
}
void controlLED(String weather, int temp) {
  if (weather.equals("Clear")) {  //맑으면 1번 LED켜기
    nowLED = 1;
    clearLED();
    //rainbow(setWait, &strip1);
    //rainbowCycle(setWait, &strip1, 1);
  }
  else if (weather.equals("Clouds")) {   //흐리면 2번 LED 켜기
    nowLED = 2;
    clearLED();
    //rainbow(setWait, &strip2);
    rainbowCycle(setWait, &strip2, 1);
  }
  else if(weather.equals("Rain")) {    //비오면 3번 LED 켜기
    nowLED = 3;
    clearLED();
    rainbowCycle(setWait, &strip3, 1);
  }
  else if(weather.equals("Snow")) {    //눈오면 4번 LED켜기
    nowLED = 4;
    clearLED();
    rainbowCycle(setWait, &strip4, 1);
  }
  else {
    nowLED = 1;
    clearLED();
    rainbowCycle(setWait, &strip1, 1); 
  }
}
void printTime() {
  Serial.print("Cureent Time : ");
  Serial.print(year());
  Serial.print("-");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());
}
void setStrip() {
  strip1.begin();
  strip1.show(); 
  strip2.begin();
  strip2.show(); 
  strip3.begin();
  strip3.show(); 
  strip4.begin();
  strip4.show(); 
}
void clearLED() {
  Serial.print("NOWLED : ");
  Serial.print(nowLED);
  Serial.print(", preLED : ");
  Serial.println(preLED);
  if (nowLED != preLED) {
    Serial.println("Different");
    switch (preLED) {
      case 1:
        strip1.clear();
        break;
      case 2:
        strip2.clear();
        break;
      case 3:
        strip3.clear();
        break;
      case 4:
        strip4.clear();
        break;
    }
    setStrip();
  }
  preLED = nowLED;
}


//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait, Adafruit_NeoPixel *strip) {
  for(uint16_t i=0; i<(*strip).numPixels(); i++) {
      (*strip).setPixelColor(i, c);
      (*strip).show();
      delay(wait);
  }
}
//모든 LED를 출력가능한 모든색으로 한번씩 보여주는 동작을 한번하는 함수
void rainbow(uint8_t wait, Adafruit_NeoPixel *strip) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    //j += 1;
    for(i=0; i<(*strip).numPixels(); i++) {
      (*strip).setPixelColor(i, Wheel((i+j) & 255, strip));
    }
    (*strip).show();
    delay(wait);
  }
}

//NeoPixel에 달린 LED를 각각 다른색으로 시작하여 다양한색으로 n번 반복한다
void rainbowCycle(uint8_t wait, Adafruit_NeoPixel *strip, int n) {
  uint16_t i, j;

  for(j=0; j<256*n; j++) {
    j += 1;
    for(i=0; i< (*strip).numPixels(); i++) {
      (*strip).setPixelColor(i, Wheel(((i * 256 / (*strip).numPixels()) + j) & 255, strip));
    }
    (*strip).show();
    delay(wait);
  }
}

//255가지의 색을 나타내는 함수
uint32_t Wheel(byte WheelPos ,Adafruit_NeoPixel *strip) {
  if(WheelPos < 85) {
   return  (*strip).Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return  (*strip).Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return  (*strip).Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

