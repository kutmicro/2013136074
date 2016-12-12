#define HC06 Serial3
String str = "";

void setup()
{
  delay(1000);
  Serial.begin(9600);
  HC06.begin(9600);
  pinMode(13, OUTPUT);
  
  Serial.write("\nTest Start\n");
}

void loop()
{
  while(HC06.available())
  {
    char data = HC06.read();
    str += data;
    delay(10);
  }
  if( !str.equals("") )  //str 값이 있다면
  {
    Serial.println("Data from Bluetooth : " + str); //시리얼모니터에 str 출력
    str="";  //myString 변수값 초기화
  }
}
