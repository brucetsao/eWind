#include "crc16.h"
#include <SoftwareSerial.h>
uint8_t outdata[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38 } ;
uint8_t incomingdata[9] ;

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
    
    sendQuery() ;
    if (receiveQuery())
        {
          Serial.println("Get Data OK");
           Serial.print("Wind Direction  is :(");
           Serial.print(incomingdata[3],HEX) ;
           Serial.print("/");
           Serial.print(incomingdata[4],HEX) ;
           Serial.print("/");
           Serial.print((double)incomingdata[3]*256+(double)incomingdata[4]) ;
           Serial.print("  m/s ) \n");
           Serial.print("Wind Direction  is :(");
           Serial.print(incomingdata[5],HEX) ;
           Serial.print("/");
           Serial.print(incomingdata[6],HEX) ;
           Serial.print("/");
           Serial.print((double)incomingdata[5]*256+(double)incomingdata[6]) ;
           Serial.print("  angle ) \n");
         if (CompareCRC16(ModbusCRC16(incomingdata,7),incomingdata[8],incomingdata[7]))
                {
                 Serial.println("The Data is Correct") ; 
                }
                else
                {
                 Serial.println("The Data is not Correct") ; 
                }
                
        }
        else
        {
          Serial.println("Fail to Get Data");
          
        }
        
    delay(2000) ;
    
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
              
            if (mySerial.available() >= 9)
                {
                    mySerial.readBytes(incomingdata, 9) ;
                      ret = true ;
                      return  ret ;
                }
        }
}

