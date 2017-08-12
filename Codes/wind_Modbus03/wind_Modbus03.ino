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
    
    sendQuery() ;
    if (receiveQuery())
        {
          Serial.println("Get Data OK");
           Serial.print("Wind Speed is :(");
           Serial.print(incomingdata[3],HEX) ;
           Serial.print("/");
           Serial.print(incomingdata[4],HEX) ;
           Serial.print("/");
           Serial.print(((double)incomingdata[3]*256+(double)incomingdata[4])/10) ;
           Serial.print("  m/s ) \n");
            Serial.println(ModbusCRC16(incomingdata,5)) ;
            Serial.println(incomingdata[6]*256+incomingdata[5]) ;
            if (CompareCRC16(ModbusCRC16(incomingdata,5),incomingdata[6],incomingdata[5]))
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
              
            if (mySerial.available() >= 7)
                {
                    mySerial.readBytes(incomingdata, 7) ;
                      ret = true ;
                      return  ret ;
                }
        }
}

