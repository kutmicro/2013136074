#include <TimeAlarms.h>
//#include <Time.h>
//#include <TimeLib.h>

int curreuntTimeArray[6];
String timeFromBluetooth = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  printTime();
  
  timeFromBluetooth = "TIME2016-12-23 11:07:20";
  Serial.println("timeFromBluetooth : " + timeFromBluetooth);
  setTimeByString(timeFromBluetooth);
}

void loop() {
  // put your main code here, to run repeatedly:
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
    setTimeByString("TIME2016-12-25 12:30:20");
    break;

    //LED 제어
    case '3' :
    break;
    
  }//switch 끝
  delay(1000);
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

void setTimeByString(String s_time) {
  //시간 정보를 빼내어 배열에 저장하고, 시간을 세팅함
  //나중에 세팅한 시간에서 3시간 지날 때 마다
  Serial.println("--------setTimeByString() Function-------");
  s_time = s_time.substring(s_time.indexOf("TIME") + 4);
  Serial.println(s_time);
  curreuntTimeArray[0] = s_time.substring(2,4).toInt();   //year
  curreuntTimeArray[1] = s_time.substring(5,7).toInt();   //month
  curreuntTimeArray[2] = s_time.substring(8,10).toInt();  //day
  curreuntTimeArray[3] = s_time.substring(11,13).toInt(); //hour
  curreuntTimeArray[4] = s_time.substring(14,16).toInt(); //minute
  curreuntTimeArray[5] = s_time.substring(17).toInt();    //second

  //hour,minute,second,day,month,year
  setTime(curreuntTimeArray[3],curreuntTimeArray[4],curreuntTimeArray[5],
  curreuntTimeArray[2],curreuntTimeArray[1],curreuntTimeArray[0]);
  printTime();
  Serial.println("-----------------------------------------");
}

