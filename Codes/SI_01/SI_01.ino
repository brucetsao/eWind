#include <String.h>
#include "crc16.h"
#include <SPI.h>
#include <WiFi101.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);
uint8_t outdata1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A } ;
// crc16 for data1 is 840A
uint8_t incomingdata1[7] ;
uint8_t outdata2[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38 } ;
// crc16 for data2 is  C438
uint8_t incomingdata2[9] ;
uint8_t outdata3[] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38 } ;
// crc16 for data3 is  C5E9
uint8_t incomingdata3[9] ;
String WindWay[] = {"北風","東北風","東風","東南風","南風","西南風","西風","西北風" } ;
int Winddir=0 ;
String WinddirName =""  ;
double Windspeed =0,  Temp=0, Humid =0 ;

void setup() {
  Serial.begin(9600);      // initialize serial communication
  Serial1.begin(9600);      // initialize serial communication
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(4000);
  }
  server.begin();                           // start the web server on port 80
  printWiFiStatus();                        // you're connected now, so print out the status
}


void loop() {
  GetSensorData() ;
  ShowSensor() ;

  /*
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
        //  digitalWrite(9, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
       //   digitalWrite(9, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  */
  delay(5000);
}

void GetSensorData()
{
   Windspeed =  GetWindSpeed() ;
   ClearBuffer() ;
   WinddirName = GetWindDir() ;
   ClearBuffer() ;
}

void ShowSensor()
{
    Serial.print("Wind Speed is :(") ;
    Serial.print(Windspeed) ;
    Serial.print(" m/s )\n") ;
    Serial.print("Wind Direction is :(") ;
    Serial.print(WinddirName) ;
    Serial.print(")\n") ;
  
}
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
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
double GetWindSpeed()
{
      sendSpeedQuery() ;
      delay(250) ;
    if (receiveSpeedQuery())
        {
      
            if ( CompareCRC16(ModbusCRC16(incomingdata1,5),incomingdata1[6],incomingdata1[5]) )
                {
                  return ( ( (double)incomingdata1[3]*256+(double)incomingdata1[4] )/10)  ;
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
int GetWindDirCheck()
{

    if (receiveDirQuery())
        {
       
            if ( CompareCRC16(ModbusCRC16(incomingdata2,7),incomingdata2[8],incomingdata2[7]) )
                {
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
