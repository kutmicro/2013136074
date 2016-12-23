#include <TimeLib.h>
#include <TimeAlarms.h>
String dataFromBluetooth;
AlarmId threeHourAlarmId;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Alarm.timerRepeat(3, Repeats);  //3초마다 Repeat호출
  printTime();
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
    //dataFromBluetooth = "(2016-12-23 15:00:00 Clear -4)(2016-12-23 18:00:00 Clouds -6)(2016-12-23 21:00:00 Clear -9)(2016-12-24 00:00:00 Clouds -7)(2016-12-24 03:00:00 Clear 0)(2016-12-24 06:00:00 Clear 2)(2016-12-24 09:00:00 Clouds -3)(2016-12-24 12:00:00 Clouds -7)(2016-12-24 15:00:00 Clear -9)(2016-12-24 18:00:00 Clouds -10)(2016-12-24 21:00:00 Clear -10)(2016-12-25 00:00:00 Clouds -7)(2016-12-25 03:00:00 Clouds 3)(2016-12-25 06:00:00 Clouds 5)(2016-12-25 09:00:00 Clouds 1)(2016-12-25 12:00:00 Clouds -2)(2016-12-25 15:00:00 Clouds -2)(2016-12-25 18:00:00 Clouds -3)(2016-12-25 21:00:00 Clouds -3)(2016-12-26 00:00:00 Clouds -1)END";
    dataFromBluetooth = "(2016-12-25 15:00:00 Clouds -2)(2016-12-25 18:00:00 Clouds -3)(2016-12-25 21:00:00 Clouds -3)(2016-12-26 00:00:00 Clouds -1)END";
    threeHourAlarmId =  Alarm.timerRepeat(3, Repeats);  //3초마다 Repeat호출
    break;
    
    //LED 제어
    case '3' :
    break;
  }//switch 끝
  
  delay(1000);
  Alarm.delay(1000);
}

void Repeats(){
  Serial.println("------------Repeats() Function-----------");
  //데이터 없을 경우
  if(dataFromBluetooth.indexOf("END") < 30) {
    Serial.println("End of Data");
    Alarm.free(threeHourAlarmId);
  }
  printTime();
  Serial.println("3 second timer");
  
  //받아온 데이터의 첫번째 부분만 꺼내기
  String nowData = dataFromBluetooth.substring(dataFromBluetooth.indexOf("("),dataFromBluetooth.indexOf(")")+1);
  Serial.println("nowData : " + nowData);
  //첫번째 부분 제거
  dataFromBluetooth = dataFromBluetooth.substring(dataFromBluetooth.indexOf(nowData) + nowData.length());
  Serial.println("dataFromBluetooth : " + dataFromBluetooth);
  Serial.println("-----------------------------------------");
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
