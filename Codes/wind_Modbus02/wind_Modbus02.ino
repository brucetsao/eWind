#include "crc16.h"
#include <SoftwareSerial.h>
uint8_t outdata[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A } ;
uint8_t incomingdata[7] ;

 SoftwareSerial mySerial(0, 1); // RX, TX
 //modbusDevice dev = new modbusDevice() ;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600)  ;
 Serial.println("Program Start Here");
 mySerial.begin(9600) ;
 // Modbus(&mySerial,9600)
}

void loop() {
    
  // put your main code here, to run repeatedly:
    if (mySerial.available()>0) 
    {
      Serial.println(mySerial.read(),HEX) ;
    }
    
}

void sendQuery()
{
    mySerial.write(outdata,8) ;
      
}

boolean receiveQuery()
{
    boolean ret = false ;
    unsigned strtime = millis() ;
      while(true)
        {
            if ( (millis() - strtime) > 2000)
            {
                 ret = false ;
                  return  ret ;
            }
              
            if (mySerial.available() >= 7)
                {
                    mySerial.readBytes(incomingdata, 7) ;
                      ret = true ;
                      return  ret ;
                }
        }
}

