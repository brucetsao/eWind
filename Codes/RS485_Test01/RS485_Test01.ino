#include "initPins.h"
void initAll()
{
    Serial.begin(9600);
     Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
     Serial.println("System Start") ;  


}
void setup() {
  // put your setup code here, to run once:
  initAll() ;
}

void loop() {
  // put your main code here, to run repeatedly:
      Serial2.println("System Start") ;
      Serial.println("System Start") ;
      delay(2000) ;
}
