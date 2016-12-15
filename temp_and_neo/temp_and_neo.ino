#include <dht11.h>
#include <Adafruit_NeoPixel.h>

dht11 DHT;

#define DHT11_PIN 27
#define PIN1 23
#define PIN2 24
#define PIN3 25
#define PIN4 26

//네오픽셀을 사용하기 위해 객체 하나를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(5, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(5, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(5, PIN4, NEO_GRB + NEO_KHZ800);

int preLED = 0;    //이전 loop에서 켰던 LED
int nowLED = 0;
int setWait = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  setStrip();
}

void loop() {
  // put your main code here, to run repeatedly:
  controlSensor();
  controlLED();
  delay(100);
}
//온습도센서 제어하는 함수
void controlSensor() {
  int chk;
  Serial.print("DHT11, \t");
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);

}
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
