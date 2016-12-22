#define HC06 Serial3
//#define PIN 13
String tempStr = "";
String strArray[35];
int count = 0;
int statusFlag = 0;     // 0 : default, 1 : recieving data, 2 : control LED


void setup()
{
  delay(1000);
  Serial.begin(9600);
  HC06.begin(9600);
  //pinMode(PIN, OUTPUT);
  
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
        statusFlag = 0;
      }
      break;
    case 2:
      Serial.println("Control LED status");
      break;
  }//end of switch
  HC06.print("Connected");
  delay(100);
}
