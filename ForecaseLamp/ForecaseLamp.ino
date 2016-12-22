#include <Adafruit_NeoPixel.h>

#define HC06 Serial3
#define NEOPIN1 23
#define NEOPIN2 24
#define NEOPIN3 25
#define NEOPIN4 26

//네오픽셀을 사용하기 위해 객체를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, NEOPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(5, NEOPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(5, NEOPIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(5, NEOPIN4, NEO_GRB + NEO_KHZ800);

String tempStr = "";
String strArray[35];
int count = 0;
int statusFlag = 0;     // 0 : default, 1 : recieving data, 2 : control LED

int preLED = 0;    //이전 loop에서 켰던 LED
int nowLED = 0;
int setWait = 20;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  HC06.begin(9600);
  setStrip();
  
  Serial.write("\nStartt\n");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(HC06.available())
  {
    statusFlag = 1;
  }
  
  switch(statusFlag) {
    case 0:
      Serial.println("default status");
      delay(1000);
      break;
    case 1:
      while(HC06.available())
      {
        char data = HC06.read();
        tempStr += data;
        delay(10);
      }
      if( !tempStr.equals("") )  //str 값이 있다면
      {
        strArray[count++] = tempStr;
        Serial.println("Data from Bluetooth : " + tempStr); //시리얼모니터에 str 출력
        Serial.println(count);
        tempStr="";  //tempStr 변수값 초기화
      }
    
      if(count == 35) 
      {
        for(int i = 0; i < 35; i++) {
          Serial.println(strArray[i]);
        }
        Serial.println("End of Recieving");
        count = 0;
        statusFlag = 2;
      }
      delay(500);
      break;
    case 2:
      Serial.println("Control LED status");
      //for(int i = 0; i < 35; i++) {
      //    Serial.println(strArray[i]);
       // }
      delay(1000);
      break;
  }//end of switch
  HC06.print("Connected");
}

/*
//온도에 따라 NeoPixel 제어하는 함수 
void controlLED() {
   if (DHT.temperature <= 26) {  //19도 이하
    nowLED = 1;
    clearLED();
    rainbowCycle(setWait, &strip1, 1);
  }
  else if (DHT.temperature <= 27) {   //22도 이하   20 21 22
    nowLED = 2;
    clearLED();
    rainbowCycle(setWait, &strip2, 1);
  }
  else if(DHT.temperature <= 28) {    //24도 이하  23 24 
    nowLED = 3;
    clearLED();
    rainbowCycle(setWait, &strip3, 1);
  }
  else {
    nowLED = 4;
    clearLED();
    rainbowCycle(setWait, &strip4, 1);        // 25도 이상
  }
}
*/
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
    j += 2;
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
