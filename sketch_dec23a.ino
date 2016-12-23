#include <Time.h>
#include <TimeAlarms.h>
String str;
//hour가 세시간 지날때마다 데이터를 확인하자
void setup()
{
  Serial.begin(9600);
  setTime(14,29,0,1,5,16); 
  int dataArray[] = {year(), month(), day(), hour(),minute(),second()};
  for (int i=0; i < 6; i++) {
    if(dataArray[i] < 10) {
      str += "0";
    }
    str += dataArray[i];
    str += ".";
  }
  Serial.println(str);
}
void  loop(){  
  Serial.println(second());
  delay(2000);
}
