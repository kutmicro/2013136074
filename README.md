# 2013136074

##Forecast Lamp

### 1. 개요
#### 1.1 제작 동기
날씨는 사람의 생활에 많은 영향을 미친다. 집을 나서기 전에는 항상 날씨를 확인하고 우산을 챙길지 안 챙길지, 무슨 옷을 입어야 할지 결정하게 된다. 매일 핸드폰을 통해 날씨를 확인하지만 도구를 사용할 필요 없이 바로 날씨를 눈으로 볼 수 있으면 편리할 것 같아 구상하게 되었다.
#### 1.2 정의
기상청에서 실시간으로 날씨 정보를 받아와 날씨를 시각적으로 보여주는 아크릴 램프. 

![정의](https://github.com/kutmicro/2013136074/blob/master/res_md/lamp_func.png)

### 2. 기능
#### 2.1 날씨
구름 낀 날씨에는 구름 모양의 아크릴 판에 불빛이 들어오고, 밝은 날에는 태양 모양의 아크릴 판에 불빛이 들어와 현재 날씨를 눈으로 확인할 수 있다.
#### 2.2 기온
기온에 따라 불빛이 색을 달리하여 현재 기온을 알 수 있다.

### 3. 구현
#### 3.1 기능 구현
기상청의 RSS를 통해 온도, 구름상태, 강수형태, 강수확률의 정보를 받아온다. 받아온 정보를 종합하여 LED의 빛을 조절한다.
#### 3.2 외관
아크릴판을 각각 해, 구름, 비, 눈, 해+구름 모양으로 레이저 커팅기를 이용해 만든다. 하단에는 3D프린터를 이용해 아크릴판을 세우고 LED와 아두이노를 보호하는 케이스를 만든다.

![외관](https://github.com/kutmicro/2013136074/blob/master/res_md/lamp_smaple1.png)

![샘플_그냥예뻐서](https://github.com/kutmicro/2013136074/blob/master/res_md/lamp_smaple2.png)

### 4. 핀맵
| Arduino Pin | NEOPIXEL | Description |
| :---: | :---: | :---: |
| GND | GND | NEOPIXEL의 GND에 연결 |
| +5V | +5V | NEOPIXEL에 전원 공급 |
| D23 | Din | NEOPIXEL1 제어 |
| D24 | Din | NEOPIXEL2 제어 |
| D25 | Din | NEOPIXEL3 제어 |
| D26 | Din | NEOPIXEL4 제어 |
### 5. 구성도   
![구성도](https://github.com/kutmicro/2013136074/blob/master/res_md/%EA%B5%AC%EC%84%B1%EB%8F%84.png)
### 6. 재료
#### 6.1 부품
- Arduino Mega 1개
- 와이파이쉴드 or esp8266 1개
- Neopixel LED 5*4개
- 점퍼케이블
- 아크릴판 4개
- 브래드보드

#### 6.2 장비
- 3D 프린터
- 레이저커팅기

### 7. 참고 사이트
- Arduino Pitout Diagram "http://samyeon.tistory.com/64"
- Neopixel LED "http://kocoafab.cc/tutorial/view/228"
- Weather Parsing "http://kocoafab.cc/tutorial/view/595"
- DIY Acrylic Lamp "http://kocoafab.cc/make/view/649"
- Weather Lamp "http://kocoafab.cc/make/view/668"
- ESP-13 Sheild Document "https://fineshang.gitbooks.io/esp8266-based-serial-wifi-shield-for-arduino-user/content/"
- ESP-13 Shield(Blynk Community) "http://community.blynk.cc/t/esp-13-web-server-serial-wifi-shield/7125"
- HTTP Request for ESP8266 module "https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/HTTPSRequest/HTTPSRequest.ino"
