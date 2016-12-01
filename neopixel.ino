#include <Adafruit_NeoPixel.h>

#define PIN 23

//네오픽셀을 사용하기 위해 객체 하나를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip1.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip1.show(); 
}

void loop() {
  //아래의 순서대로 NeoPixel을 반복한다.

  //화려하게 다양한 색 출력
  rainbowCycle(20, &strip1);
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
