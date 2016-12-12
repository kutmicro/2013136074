//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// The Unnamed Circuit
// 
// Made by Yegyeom Yu
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3020987-the-unnamed-circuit

#include <Adafruit_NeoPixel.h>

#define PIN 2

//네오픽셀을 사용하기 위해 객체 하나를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);

int sample_cloud = 1;		// 1(맑음)	 : 2번핀 켬
							// 2,3,4(구름) : 4번핀 켬
							// 5(비) 	  : 7번핀 켬
							// 6(눈/비) 	 : 7,8같이켬
							// 7(눈) 	  : 8번핀 켬
int temp = 40;		

int red = 255;
int green = 255;
int blue = 255;

void setup() {
  strip1.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip1.show(); 
}

void loop() {
  if (temp >= 40) {
    red = 255;
    green = 0;
    blue = 0;
  }
  else if (temp >= 25) {		//25~39
    red = 255;
    green = (40-temp) * 10;		// 10~150 , 39일 때 10, 25일때 150
    blue = (40-temp) * 10;
  }
  else if (temp >= 10) {		//10~24
    green = 255;
    red = (25-temp)*10;			// 10~150, 24일 때 10, 10일 때 150
    blue = (25-temp)*10;
  }
  else if (temp >= 0) {			//0~9
    green = 255;
    blue = 255;
    red = temp * 20;
  }
  else if (temp >= -20) {		//-1~-20
    blue = 255;
    green = (temp+21) * 10;		// 10~150 , -20일 때 10, -6일때 150
    red = (temp+21) * 10;
  }
  else {
    blue = 255;
    red = 0;
    green = 0;
  }
  switch(sample_cloud) {
    case 1 :
    colorWipe(strip1.Color(red, blue, green), 50); //빨간색 출력
    break;
  }
}

//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
      strip1.setPixelColor(i, c);
      strip1.show();
      delay(wait);
  }
}
//http://forum.falinux.com/zbxe/index.php?document_srl=867520&mid=lecture_tip
