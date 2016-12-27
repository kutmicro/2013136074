#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Adafruit_NeoPixel.h>

#define HC06 Serial3
#define NEOPIN1 22
#define NEOPIN2 23
#define NEOPIN3 24
#define NEOPIN4 25

//네오픽셀을 사용하기 위해 객체를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(4, NEOPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(4, NEOPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(4, NEOPIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(4, NEOPIN4, NEO_GRB + NEO_KHZ800);

int RepeatTime = 10800;    //세시간마다 날씨 제어

String tempStr = "";
String dataFromBluetooth = "";
String nowWeather = "";
int nowTemp = 0;

//LED 동기화를 위한 변수(같은 LED를 켤 때 깜빡거리지 않게하기 위해)
int preLED = 0;    //이전 loop에서 켰던 LED
int nowLED = 0;    //현재 loop에서 켠 LED
int setWait = 20;

int LEDtype = 0;    // 0 : 디폴트(RainbowCycle), 1 : Rainbow, 2: 사용자 지정 RGB
String RGB = "";
String TEMP = "";

AlarmId threeHourAlarmId;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  HC06.begin(9600);
  
  printTime();
  setStrip();
}
void loop()
{
  if(HC06.available()) {
    while(HC06.available()){
      char data = HC06.read();
      tempStr += data;
      delay(10);
    }
    //데이터의 끝 수신
    if(tempStr.indexOf("END") != -1) {
        Serial.println("Data from Bluetooth : " + tempStr); //시리얼모니터에 str 출력
        Serial.println("End of Recieving");
        dataFromBluetooth = tempStr;
        tempStr="";  //tempStr 변수값 초기화
        //알람 해제
        Alarm.free(threeHourAlarmId);
        
        RepeatTime = 10800; //  시간 간격을 3시간으로 초기화
        Repeats();      //Repeat 한번 호출
        threeHourAlarmId =  Alarm.timerRepeat(RepeatTime, Repeats);  //RepeatTime초마다 Repeat호출
    }
    //테스트 수신시 알람 시간을 5초로 변경
    if(tempStr.indexOf("TEST") != -1) {
        Serial.println("Data from Bluetooth : " + tempStr); //시리얼모니터에 str 출력
        tempStr="";  //tempStr 변수값 초기화
        //세시간 알람 해제
        Alarm.free(threeHourAlarmId);
        
        RepeatTime = 5;
        Repeats();      //Repeat 한번 호출
        threeHourAlarmId =  Alarm.timerRepeat(RepeatTime, Repeats);  //RepeatTime초마다 Repeat호출
    }
    //LED 제어요청 수신
    if(tempStr.indexOf("LED") != -1) {
      Serial.println("Data from Bluetooth : " + tempStr);
      LEDParsing(tempStr);
      controlLED(nowWeather,nowTemp);
      tempStr="";  //tempStr 변수값 초기화        
    }
  }
  delay(100);
  Alarm.delay(100);
}

///////////////////////일정 시간마다 반복 호출되어 데이터를 controlLED로 보내는 함수///////////////////////
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

  HC06.print(nowData);
  
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
  nowWeather = weather;
  nowTemp = temp.toInt();
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

//////////////////////////////////// LED 제어하는 함수//////////////////////////////////////////
void controlLED(String weather, int temp) {
  if (weather.equals("Snow")) {  //눈오면 1번 LED켜기
    nowLED = 1;
    clearLED();
    switch(LEDtype) {
      case 0 :
      rainbowCycle(setWait, &strip1, 1);
      break;
      case 1 :
      rainbow(setWait, &strip1);
      break;
      case 2:
      //첫번째 RGB
      String tmpStr = RGB.substring(RGB.indexOf("(") + 1,RGB.indexOf(")"));
      int r = tmpStr.substring(0,tmpStr.indexOf(" ")).toInt();
      int g = tmpStr.substring(tmpStr.indexOf(" ") + 1,tmpStr.lastIndexOf(" ")).toInt();
      int b = tmpStr.substring(tmpStr.lastIndexOf(" ")).toInt();
      colorWipe(strip1.Color(r,g,b), setWait, &strip1);
      Serial.print(tempStr);
      break;
    }
  }
  else if (weather.equals("Rain")) {   //비오면 2번 LED 켜기
    nowLED = 2;
    clearLED();
    switch(LEDtype) {
      case 0 :
      rainbowCycle(setWait, &strip2, 1);
      break;
      case 1 :
      rainbow(setWait, &strip2);
      break;
      case 2:
      //두번째 RGB
      String tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf(")"));
      int r = tmpStr.substring(0,tmpStr.indexOf(" ")).toInt();
      int g = tmpStr.substring(tmpStr.indexOf(" ") + 1,tmpStr.lastIndexOf(" ")).toInt();
      int b = tmpStr.substring(tmpStr.lastIndexOf(" ")).toInt();
      colorWipe(strip2.Color(r,g,b), setWait, &strip2);
      
      Serial.print(tempStr);
      break;
    }
    
  }
  else if(weather.equals("Clouds")) {    //흐리면 3번 LED 켜기
    nowLED = 3;
    clearLED();
    switch(LEDtype) {
      case 0 :
      rainbowCycle(setWait, &strip3, 1);
      break;
      case 1 :
      rainbow(setWait, &strip3);
      break;
      case 2:
      //세번째 RGB
      String tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf(")"));
      Serial.print(tempStr);
      int r = tmpStr.substring(0,tmpStr.indexOf(" ")).toInt();
      int g = tmpStr.substring(tmpStr.indexOf(" ") + 1,tmpStr.lastIndexOf(" ")).toInt();
      int b = tmpStr.substring(tmpStr.lastIndexOf(" ")).toInt();
      colorWipe(strip3.Color(r,g,b), setWait, &strip3);
      break;
    }
  }
  else if(weather.equals("Clear")) {    //맑으면 4번 LED켜기
    nowLED = 4;
    clearLED();
    switch(LEDtype) {
      case 0 :
      rainbowCycle(setWait, &strip4, 1);
      break;
      case 1 :
      rainbow(setWait, &strip4);
      break;
      case 2:
      //네번째 RGB
      String tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf("(") + 1);
      tmpStr = RGB.substring(RGB.indexOf(")"));
      int r = tmpStr.substring(0,tmpStr.indexOf(" ")).toInt();
      int g = tmpStr.substring(tmpStr.indexOf(" ") + 1,tmpStr.lastIndexOf(" ")).toInt();
      int b = tmpStr.substring(tmpStr.lastIndexOf(" ")).toInt();
      colorWipe(strip4.Color(r,g,b), setWait, &strip4);
      
      Serial.print(tempStr);
      break;
    }
  }
  else {
    Serial.println("NO WEATHER");
  }
}
////////////////////////////////////////////LED 색 변경 함수///////////////////////////////////////////
void LEDParsing(String data) {
  data = data.substring(4, data.lastIndexOf(")"));     // "LED(" 제거, 마지막 ")"제거
  if(data.startsWith("RAINBOW") ) {
    if(data.indexOf("CYCLE") == -1) {
      LEDtype = 1;
    }
    else LEDtype = 0;
  }
  else if(data.startsWith("RGB")) {
    RGB = data.substring(data.indexOf("(") + 1, data.lastIndexOf(")"));    //"RGB"와 필요없는 괄호 제거
    LEDtype = 2;
  }
}

//////////////////////////////////// Strip 초기화 관련 함수////////////////////////////////////////////
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

//////////////////////////////////// LED 색 채우는 함수////////////////////////////////////////////
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
    //j += 1;
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

///////////////////////////////////현재 시간을 시리얼에 출력하는 함수////////////////////////////////////
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
