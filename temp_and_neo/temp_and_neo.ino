#include <dht11.h>
#include <Adafruit_NeoPixel.h>

dht11 DHT;

#define DHT11_PIN 4
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
/*
  strip1.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip1.show(); 
  strip2.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip2.show(); 
  strip3.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip3.show(); 
  strip4.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip4.show(); 
  */
  setStrip();
}

void loop() {
  // put your main code here, to run repeatedly:
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

  if (DHT.temperature <= 19) {  //19도 이하
    colorWipe(strip1.Color(255, 0, 0), 50, &strip1); //빨간색 출력
    strip1.clear();
    //rainbowCycle(20, &strip1);
  }
  else if (DHT.temperature <= 22) {   //22도 이하   20 21 22
    //rainbowCycle(20, &strip2);
    colorWipe(strip2.Color(255, 0, 0), 50, &strip2); //빨간색 출력
    strip2.clear();
  }
  else if(DHT.temperature <= 24) {    //24도 이하  23 24 25
    colorWipe(strip3.Color(255, 0, 0), 50, &strip3); //빨간색 출력
    strip3.clear();
    //rainbowCycle(20, &strip3);
  }
  else {
    colorWipe(strip4.Color(255, 0, 0), 50, &strip4); //빨간색 출력 
    strip4.clear();
    //rainbowCycle(20, &strip4);        // 26도 이하 26 27 28
  }
  delay(1000);
  
  setStrip();
}
//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait, Adafruit_NeoPixel *strip) {
  for(uint16_t i=0; i<(*strip).numPixels(); i++) {
      (*strip).setPixelColor(i, c);
      (*strip).show();
      delay(wait);
  }
}

//NeoPixel에 달린 LED를 각각 다른색으로 시작하여 다양한색으로 5번 반복한다
void rainbowCycle(uint8_t wait, Adafruit_NeoPixel *strip) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
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
  strip1.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip1.show(); 
  strip2.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip2.show(); 
  strip3.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip3.show(); 
  strip4.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip4.show(); 
}

