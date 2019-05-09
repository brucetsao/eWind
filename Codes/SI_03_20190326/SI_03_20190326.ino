#include "crc16.h"
#include <SPI.h>
#include <WiFi101.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
//char iotserver[] = "192.168.88.105";    // name address for Google (using DNS)
IPAddress iotserver(192,168,0,199) ;
int iotport = 80 ;

String strGet="GET /wind/windadd.php";
String strHttp=" HTTP/1.1";
String strHost="Host: 192.168.0.199";  //OK
  String connectstr ;
WiFiClient client;
uint8_t outdata1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A } ;
// crc16 for data1 is 840A
uint8_t incomingdata1[7] ;
uint8_t outdata2[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38 } ;
// crc16 for data2 is  C438
uint8_t incomingdata2[9] ;
uint8_t outdata3[] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC5, 0xE9 } ;
// crc16 for data3 is  C5E9
uint8_t incomingdata3[9] ;
String WindWay[] = {"北風","東北風","東風","東南風","南風","西南風","西風","西北風" } ;
int Winddir=0 ;
int Windangle=0 ;
String WinddirName =WindWay[Winddir]  ;
double Windspeed =0,  Temp=0, Humid =0 ;
 IPAddress ip ;
String MacData ;
void setup() {

  initall() ;
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }
  
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED)
  {

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(4000);
  }

  printWiFiStatus();                        // you're connected now, so print out the status
  MacData = GetMacAddress() ;

  }


void loop() {
  digitalWrite(RSLED,HIGH) ;
  GetSensorData() ;
  ShowSensor() ;
  digitalWrite(RSLED,LOW) ;
   connectstr = "" ;
 connectstr = "?mac=" + MacData+"&ip="+IpAddress2String(ip)+"&speed="+ String(Windspeed)+"&way="+ Windangle+"&waydir="+ Winddir+"&temp="+ String(Temp)+"&humid="+ String(Humid);
             Serial.println(connectstr) ;
              if (client.connect(iotserver, iotport)) 
                 {
                     Serial.println("Make a HTTP request ... ");
                     //### Send to Server
                     String strHttpGet = strGet + connectstr + strHttp;
                     Serial.println(strHttpGet);
         
                     client.println(strHttpGet);
                     Serial.println(strHost);
                     client.println(strHost);
                      client.println("Connection: close");                   
                     client.println();
                   } 
                  

          if (client.connected()) 
          { 
              client.stop();  // DISCONNECT FROM THE SERVER
                Serial.println("client disonnected");
          }
    digitalWrite(AccessLED,LOW) ;
  delay(30000);
}

void GetSensorData()
{
   GetWindSpeed() ;
   ClearBuffer() ;
   GetWindDir() ;
   ClearBuffer() ;
   GetDHT() ;
   ClearBuffer() ;  
}

void ShowSensor()
{
  
  Serial.print("IP Address: ");
  Serial.println(ip);
    Serial.print("\n") ;  
    Serial.print("Wind Speed is :(") ;
    Serial.print(Windspeed) ;
    Serial.print(" m/s )\n") ;
    Serial.print("Wind Direction is :(") ;
    Serial.print(WinddirName) ;
    Serial.print(")\n") ;
    Serial.print("Temperature is :(") ;
    Serial.print(Temp) ;
    Serial.print(")\n") ;
    Serial.print("Humidity is :(") ;
    Serial.print(Humid) ;
    Serial.print(")\n") ;
  
}
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

//---------Speed----------
int GetWindSpeed()
{
      sendSpeedQuery() ;
      delay(250) ;
    if (receiveSpeedQuery())
        {
      
            if ( CompareCRC16(ModbusCRC16(incomingdata1,5),incomingdata1[6],incomingdata1[5]) )
                {
                  Winddir= ( ( (double)incomingdata1[3]*256+(double)incomingdata1[4] )/10)  ;
                  //Windangle= ( ( (double)incomingdata1[5]*256+(double)incomingdata1[6] )/10)  ;
                  return (1)  ;                  
                }
                else
                {
                    return (-1) ; 
                }
                
        }
        else
        {
            return (-2) ; 
          
        }
        
}

void sendSpeedQuery()
{
    Serial1.write(outdata1,8) ;
      
}

boolean receiveSpeedQuery()
{
    boolean ret = false ;
    unsigned strtime = millis() ;
      while(true)
        {
            if ( (millis() - strtime) > 3000)
            {
                 ret = false ;
                  return  ret ;
            }
              
            if (Serial1.available() >= 7)
                {
                    Serial1.readBytes(incomingdata1, 7) ;
                      ret = true ;
                      return  ret ;
                }
        }
}



//---------Win direction----------
String GetWindDir()
{ 
     sendDirQuery() ;
     delay(250) ;
    int tmp = GetWindDirCheck() ;
    Serial.print("GetWindDir():(") ;
    Serial.print(tmp) ;
    Serial.print(")\n") ;
if (tmp >= 0) 
  {
      return WindWay[tmp] ;
  }
  else
  {
      return "Undefined" ;
  }
}

int CalcWind(uint8_t Hi, uint8_t Lo)
{
    return (  Hi *  256+ Lo  ) ;
}
int CalcWind1(uint8_t Hi, uint8_t Lo)
{ 
    if ((Hi,7) == 1)
      {
        Hi =  bitWrite(Hi,7,0) ;
         return (  Hi *  256+ Lo  ) * (-1) ;
      }
      else
      {
            return (  Hi *  256+ Lo  ) ;
      }

}
int GetWindDirCheck()
{

    if (receiveDirQuery())
        {
       
            if ( CompareCRC16(ModbusCRC16(incomingdata2,7),incomingdata2[8],incomingdata2[7]) )
                {
                  Windangle = incomingdata2[5]*256+incomingdata2[6]
                  return (CalcWind(incomingdata2[3],incomingdata2[4]))  ;
                }
                else
                {
                  return (-1)  ;
                }
                
        }
        else
        {
            return (-2) ; 
          
        }
        
}

void sendDirQuery()
{
    Serial1.write(outdata2,8) ;
      
}

boolean receiveDirQuery()
{
    boolean ret = false ;
    unsigned strtime = millis() ;
      while(true)
        {
            if ( (millis() - strtime) > 3000)
            {
                 ret = false ;
                  return  ret ;
            }
              
            if (Serial1.available() >= 9)
                {
                    Serial1.readBytes(incomingdata2, 9) ;
                      ret = true ;
                      return  ret ;
                }
        }
}

//---------DHT ----------
int GetDHT()
{ 
     sendDHTQuery() ;
     delay(250) ;
    int tmp = GetDHTCheck() ;
    if (tmp == 1)
      {
        Temp = (double)(CalcWind1(incomingdata3[5],incomingdata3[6])/10) ;
        Humid = (double)(CalcWind(incomingdata3[3],incomingdata3[4])/10) ;
      }    
      else
      {
            Serial.print("GetDHTCheck Error Code is :(") ;
          Serial.print(tmp) ;
          Serial.print(")\n") ;      
      }
  return tmp ;
}

int GetDHTCheck()
{
  
    if (receiveDHTQuery())
        {
       
            if ( CompareCRC16(ModbusCRC16(incomingdata3,7),incomingdata3[8],incomingdata3[7]) )
                {
                  return 1  ;
                }
                else
                {
                  return (-1)  ;
                }
                
        }
        else
        {
            return (-2) ; 
          
        }
        
}

void sendDHTQuery()
{
    Serial1.write(outdata3,8) ;
      
}

boolean receiveDHTQuery()
{
    boolean ret = false ;
    unsigned strtime = millis() ;
      while(true)
        {
            if ( (millis() - strtime) > 3000)
            {
                 ret = false ;
                  return  ret ;
            }
              
            if (Serial1.available() >= 9)
                {
                    Serial1.readBytes(incomingdata3, 9) ;
                      ret = true ;
                      return  ret ;
                }
        }
}


//-----------
void ClearBuffer()
{
    unsigned char tt;
    if ( Serial1.available() >0)
      {
          while ( Serial1.available() >0)
            {
                tt = Serial1.read() ;
            }
      }
}

void initall()
{
  Serial.begin(9600);      // initialize serial communication
  Serial1.begin(9600);      // initialize serial communication
  pinMode(PowerLED,OUTPUT) ;
  pinMode(RSLED,OUTPUT) ;
  pinMode(AccessLED,OUTPUT) ;
  digitalWrite(PowerLED,HIGH) ;
  digitalWrite(RSLED,LOW) ;
  digitalWrite(AccessLED,LOW) ;
 
}

String GetMacAddress() {
  // the MAC address of your WiFi shield
  String Tmp = "" ;
  byte mac[6];
  
  // print your MAC address:
  WiFi.macAddress(mac);
  for (int i=5; i>=0; i--)
    {
        Tmp.concat(print2HEX(mac[i])) ;
    }
    Tmp.toUpperCase() ;
  return Tmp ;
}



String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
  }
  return ttt ;
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}
