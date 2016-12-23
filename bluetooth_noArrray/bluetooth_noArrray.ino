#define HC06 Serial3
String tempStr = "";
String finalStr = "";
String timeStr = "";
int count = 0;
int statusFlag = 0;     // 0 : default, 1 : recieving data, 2 : control LED
int indexOfNow = 0;


void setup()
{
  delay(1000);
  Serial.begin(9600);
  HC06.begin(9600);
  //timeStr = changeTime("(2016-12-25 00:00:00 Clouds 1)");
  Serial.write("\nTest Start\n");
}

void loop()
{
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
        count++;
        Serial.println(count);
      }
      if(tempStr.indexOf("TIME") != -1)
      {
        Serial.println(tempStr);
        timeStr = tempStr;
        count = 0;
        tempStr="";  //tempStr 변수값 초기화
        statusFlag = 0;
        parseData();
      }
      if(tempStr.indexOf("END") != -1) 
      {
        Serial.println("Data from Bluetooth : " + tempStr); //시리얼모니터에 str 출력
        Serial.println("End of Recieving");
        count = 0;
        finalStr = tempStr;
        tempStr="";  //tempStr 변수값 초기화
        statusFlag = 2;
      }
      break;
    case 2:
      Serial.println("Control LED status");
      delay(1000);
      break;
  }//end of switch
  HC06.print("Connected");
  delay(100);
}

void parseData()
{
  timeStr = timeStr.substring(timeStr.indexOf("TIME") + 4);
  finalStr = finalStr.substring((finalStr.indexOf("(") + 1), + 20);
  Serial.println("time : " + timeStr);
  Serial.println("final : " + finalStr);
  Serial.println(timeStr.compareTo(finalStr));
}

String changeTime(String str) 
{
  String s_hour = str.substring(15,17);
  String s_day = str.substring(12,14);
  char* c_hour;
  char* c_day;
  
  int i_hour = s_hour.toInt();
  int i_day = s_day.toInt();
  
  Serial.println("hour " + s_hour);
  Serial.println("day " + s_day);
  
  Serial.println("i_hour " + i_hour);
  Serial.println("i_day " + i_day);

  s_hour.toCharArray(c_hour, 3);
  s_day.toCharArray(c_day, 3);
  
  if(i_hour >= 9 ) {    
    i_hour = i_hour - 9;
    itoa(i_hour,c_hour,10);
    if(i_hour < 10) {
      s_hour[0] = '0';
      s_hour[1] = c_hour[1];
    }
    else {
      s_hour[0] = c_hour[0];
      s_hour[1] = c_hour[1];
    }
  }
  else {
    i_day = i_day - 1;
    i_hour = 24 - 9;
    
    itoa(i_day,c_day,10);
    itoa(i_hour,c_hour,10);
    
    s_hour[0] = c_hour[0];
    s_hour[1] = c_hour[1];
    s_day[0] = c_day[0];
    s_day[1] = c_day[1];
    //Serial.println("i_day " + i_day);
    Serial.println("i_day " + i_day);
    Serial.println("i_hour " + i_hour);
  }
  Serial.println(s_hour + "시 " + s_day + "일");
  return "";
}

