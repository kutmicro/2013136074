/*
Arduino Due + HC-06 (Bluetooth) -echo bluetooth data

Serial (Tx/Rx) communicate to PC via USB
Serial3 (Tx3/Rx3) connect to HC-06
HC-06 Rx - Due Tx3
HC-06 Tx - Due Rx3
HC-06 GND - Due GND
HC-06 VCC - Due 5V

*/

void loop() {
    byte data;
    data = Serial.read();
    if(data=='a'){
      digitalWrite(13, HIGH);   // set the LED on
    }
    if(data=='b'){
      digitalWrite(13, LOW);    // set the LED off
    }
}

#define HC06 Serial3

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
    Serial.write("\nTxxxx\n");
    char data = HC06.read();
    Serial.write(data);
    HC06.write(data);
  }
}
