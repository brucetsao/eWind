#include "arduino_secrets.h" 
#include "MQTTLib.h" 
#include "crc16.h" 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>

WiFiMulti wifiMulti;

WiFiClient mqclient;
PubSubClient mqttclient(mqclient);

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//-----------------------



//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
          // your network key Index number (needed only for WEP)

  IPAddress ip ;
  long rssi ;
  
int status = WL_IDLE_STATUS;
char iotserver[] = "ncnu.arduino.org.tw";    // name address for Google (using DNS)
int iotport = 9999 ;
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
String strGet="GET /wind/windadd.php";
String strHttp=" HTTP/1.1";
String strHost="Host: ncnu.arduino.org.tw";  //OK
 String connectstr ;
String MacData ;
WiFiClient client;

//  control parameter 
boolean systemstatus = false ;
boolean Reading = false ;
boolean Readok = false ;
// int trycount = 0 ;
int wifierror = 0 ;
boolean btnflag = false ;
//---------------

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
//----------------
#include <ArduinoJson.h>


char clintid[20];
const int capacity = JSON_OBJECT_SIZE(6);
 StaticJsonDocument<200> doc;
char JSONmessageBuffer[200];
String payloadStr ;
    //-----------------
void setup() 
{
  

  initall() ;

  WiFi.disconnect(true);
  WiFi.setSleep(false);

  
    wifiMulti.addAP(AP2, PW2);
    wifiMulti.addAP(AP1, PW1);

    Serial.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) 
    {
        Apname = WiFi.SSID();
        ip = WiFi.localIP();
        Serial.println("");
        Serial.print("Successful Connectting to Access Point:");
        Serial.println(WiFi.SSID());
        Serial.print("\n");
        
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(ip);
         //ShowAP() ;
              
    }
  MacData = GetMacAddress() ;  
 // printWiFiStatus();
  ShowInternet() ;
  ShowString("init System...") ;
  mqttclient.setServer("broker.shiftr.io", 1883);
  // mqttclient.begin("broker.shiftr.io", mqclient);
 // mqttclient.onMessage(messageReceived);
  fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;

  connectMQTT();
    PowerOn() ;
    btnflag = false ;     
  ShowString("System  Ready") ;    
  Serial.println("System  Ready");

 
  }


void loop() {
 // mqttclient.loop() ;
 // GetSensorData() ;
  ShowSensor() ;
  ShowSensoronLCD() ;
  //CreateJsonData(String Mac,String IPStr,double sp, int way, double t,double h )
  payloadStr = CreateJsonData2(MacData,IpAddress2String(ip),Windspeed,Windangle,Temp,Humid) ;

   connectstr = "" ;
   //http://ncnu.arduino.org.tw:9999/wind/windadd.php?mac=246F289E432C&ip=192.168.1.102&speed=0.00&way=0&waydir=0&temp=0.00&humid=0.00

 connectstr = "?mac=" + MacData+"&ip="+IpAddress2String(ip)+"&speed="+ String(Windspeed)+"&way="+ Windangle+"&temp="+ String(Temp)+"&humid="+ String(Humid);
             Serial.println(connectstr) ;
              if (client.connect(iotserver, iotport)) 
                 {
                     ShowString("Send Data to DB") ;
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
  //   ShowString("Create MQTT Data") ;
  //  serializeJson(doc, JSONmessageBuffer) ;
     if (!mqttclient.connected()) 
  {
     Serial.println("ReconnectMQTT()"); 
     connectMQTT();
       ShowString("MQTT Reconnected...") ;
  } 

      
  payloadStr.toCharArray(JSONmessageBuffer,payloadStr.length()+1) ;
      JSONmessageBuffer[payloadStr.length()+2]='\n' ;
  Serial.print("Size is :") ;
   Serial.print(payloadStr.length()) ;
   Serial.print("/") ;
      Serial.print(payloadStr) ;
   Serial.print("/") ;
      Serial.print(JSONmessageBuffer) ;
      Serial.print("\n") ;
//    if (mqttclient.publish("/NCNU/Wind/",JSONmessageBuffer,payloadStr.length()+1))
//    if (mqttclient.publish("/NCNU/Wind/","aaaaaaaaaaaaaaaa"))
if (mqttclient.publish("/NCNU/Wind/",JSONmessageBuffer,payloadStr.length()+1))
{
   //  mqttclient.publish("/NCNU/Wind/","aaaaaaa" );
//     mqttclient.publish("/NCNU/Wind/",JSONmessageBuffer);
   //  mqttclient.publish("/NCNU/Wind/","aaaaaaa" );
  //   mqttclient.publish("/NCNU/Wind/","aaaa");
        ShowString("MQTT Sent success.....") ;
        mqttclient.loop() ;
    }else
    {
        ShowString("MQTT Fail....") ;
    }  



  
    /*
  //  doc.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    MQTT_Publish(JSONmessageBuffer) ;
    MQTT_Publish("asdfggg") ;
       serializeJson(doc, Serial);
  Serial.println("\n----------------") ;       
 // Serial.println(JSONmessageBuffer);  
  Serial.println("----------------") ;  
  */
  /*
    mqttclient.loop();

   if (!mqttclient.connected()) 
  {
     Serial.println("connectMQTT()"); 
     connectMQTT();
  }  
  */
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
void ShowSensoronLCD()
{
    lcd.setCursor(0,2);
  lcd.print("                    ");  
  lcd.setCursor(0,2);
    lcd.print(Windspeed) ;
    lcd.print("/") ;
    lcd.print(Windangle) ;
    lcd.print("/") ;
    lcd.print(Temp) ;
    lcd.print("/") ;
    lcd.print(Humid) ;

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
  //Apname = WiFi.SSID();
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(Apname);

  // print your WiFi shield's IP address:
 // ip = WiFi.localIP();
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
                  Windspeed= ( ( (double)incomingdata1[3]*256+(double)incomingdata1[4] )/10)  ;
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
                  Windangle = incomingdata2[5]*256+incomingdata2[6] ;
                  Winddir = incomingdata2[3]*256+incomingdata2[4]  ;
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
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);     // initialize serial communication
  pinMode(PowerLed,OUTPUT) ;
  pinMode(AccessLED,OUTPUT) ;
  digitalWrite(PowerLed,HIGH) ;
  digitalWrite(AccessLED,LOW) ;
   lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  doc["MAC"] = "112233445566";
  doc["IP"] = "192.168.99.254";
  doc["WindSpeed"] = String(0.0);
  doc["WindDirection"] = String(0);
  doc["Temperature"] = String(0.0);
  doc["Humidity"] = String(0.0);  
}


//------------------
void ShowInternet()
{
    ShowMAC() ;
    ShowAP() ;
    ShowIP()  ;
}

void ShowAP()
{
  lcd.setCursor(12,0);
  lcd.print("/");  
  lcd.print(Apname);  

}
void ClearShow()
{
    lcd.setCursor(0,0);
    lcd.clear() ;
    lcd.setCursor(0,0);
}

void ShowMAC()
{
  lcd.setCursor(0,0);
  lcd.print("                    ");  
  lcd.setCursor(0,0);
  lcd.print(MacData);  

}
void ShowIP()
{
  lcd.setCursor(0,1);
  lcd.print("                    ");  
  lcd.setCursor(0,1);
  lcd.print("IP:");  
  lcd.print(ip);  

}




void ShowString(String ss)
{
  lcd.setCursor(0,3);
  lcd.print("                    ");  
  lcd.setCursor(0,3);
  lcd.print(ss.substring(0,19)); 
  //delay(1000);
}

String SPACE(int sp)
{
    String tmp = "" ;
    for (int i = 0 ; i < sp; i++)
      {
          tmp.concat(' ')  ;
      }
    return tmp ;
}


String strzero(long num, int len, int base)
{
  String retstring = String("");
  int ln = 1 ;
    int i = 0 ; 
    char tmp[10] ;
    long tmpnum = num ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base) ;
        tmp[ln-1] = hexcode[tmpchr] ;
        ln++ ;
         tmpnum = (long)(tmpnum/base) ;
 
        
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
    
  return retstring;
}

unsigned long unstrzero(String hexstr, int base)
{
  String chkstring  ;
  int len = hexstr.length() ;
  
    unsigned int i = 0 ;
    unsigned int tmp = 0 ;
    unsigned int tmp1 = 0 ;
    unsigned long tmpnum = 0 ;
    String hexcode = String("0123456789ABCDEF") ;
    for (i = 0 ; i < (len ) ; i++)
    {
  //     chkstring= hexstr.substring(i,i) ; 
      hexstr.toUpperCase() ;
           tmp = hexstr.charAt(i) ;   // give i th char and return this char
           tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(base,(len -i -1) )  ;
 
        
    }
  return tmpnum;
}

long POW(long num, int expo)
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
          tmp = tmp * num ;
         return tmp ; 
  } 
  else
  {
   return tmp ; 
  }
}



void PowerOn()
{
    digitalWrite(PowerLed,HIGH) ;
}

void PowerOff()
{
    digitalWrite(PowerLed,LOW) ;
}




void AccessOn()
{
    digitalWrite(AccessLED,HIGH) ;
}

void AccessOff()
{
    digitalWrite(AccessLED,LOW) ;
}

 void connectMQTT()
 {
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  while (!mqttclient.connect(clintid, "try", "try")) {
    Serial.print("-");
    delay(1000);
  }
    Serial.print("\n");
  
//  mqttclient.subscribe("/nhri/result/#");
  Serial.println("\n MQTT connected!");


  // client.unsubscribe("/hello");
}



void messageReceived(String &topic, String &payload) {
            //CarNumber = payload ;
            ShowString("Msg:"+payload) ;
       Serial.println("MSG:" +payload);

      
}


void msgDecode(String tt)
{
  /*
   payloadcount =SplitbyTag(tt, ',') ; 
  if (payloadcount >0)
    {
        DecodeParameter(payloadcount) ;
    }
   jobName = jobDecode(payloadcount) ;
   if (jobName != "error")
    {  
        if (ValueDecode(payloadcount,rfidnum)==CarNumber)
          {
                displayValueonHMI(jobName)  ;
          }
    }
    */
}


 void listNetworks() 
 {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) 
    {
        Serial.println("no networks found");
    } 
    else 
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) 
          {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");
}




String GetMacAddress() {
  // the MAC address of your WiFi shield
  String Tmp = "" ;
  byte mac[6];
  
  // print your MAC address:
  WiFi.macAddress(mac);
  for (int i=0; i<6; i++)
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




String chrtoString(char *p)
{
    String tmp ;
    char c ;
    int count = 0 ;
    while (count <100)
    {
        c= *p ;
        if (c != 0x00)
          {
            tmp.concat(String(c)) ;
          }
          else
          {
              return tmp ;
          }
       count++ ;
       p++;
       
    }
}


void CopyString2Char(String ss, char *p)
{
         //  sprintf(p,"%s",ss) ;

  if (ss.length() <=0)
      {
           *p =  0x00 ;
          return ;
      }
    ss.toCharArray(p, ss.length()+1) ;
   // *(p+ss.length()+1) = 0x00 ;
}

boolean CharCompare(char *p, char *q)
  {
      boolean flag = false ;
      int count = 0 ;
      int nomatch = 0 ;
      while (flag <100)
      {
          if (*(p+count) == 0x00 or *(q+count) == 0x00)
            break ;
          if (*(p+count) != *(q+count) )
              {
                 nomatch ++ ; 
              }
             count++ ; 
      }
     if (nomatch >0)
      {
        return false ;
      }
      else
      {
        return true ;
      }
      
        
  }




/*
void MQTT_Publish(char msg)
{
     if (!mqttclient.connected()) 
  {
     Serial.println("connectMQTT()"); 
     connectMQTT();
       ShowString("MQTT Reconnected...") ;
  } 
 // int ll = msg.length() ;
  Serial.println("====in MQTT Send=====") ;
  Serial.println(msg) ;
  Serial.println("===end of in MQTT Send=====") ;
  
   mqttclient.publish("/NCNU/Wind/",msg);
   ShowString("MQTT Sent.....") ;
}
*/
void fillCID(String mm)
{
  // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 'p' ;  
  clintid[1]= 'q' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;

}
String CreateJsonData2(String Mac,String IPStr,double sp, int way, double t,double h )
{
    String tmp = "{" ;
    //---------------
   
    tmp.concat("\"MAC\": ") ;
    tmp.concat("\"") ;
    tmp.concat(Mac) ;
    tmp.concat("\",") ;
    //---------------
    tmp.concat("\"IP\": ") ;
    tmp.concat("\"") ;
    tmp.concat(IPStr) ;
    tmp.concat("\",") ;
      //---------------
    tmp.concat("\"WindSpeed\": ") ;
    tmp.concat("\"") ;
    tmp.concat(String(sp)) ;
    tmp.concat("\",") ;
    //---------------
    tmp.concat("\"WindDirection\": ") ;
    tmp.concat("\"") ;
    tmp.concat(String(way)) ;
    tmp.concat("\",") ;
    //---------------
    tmp.concat("\"Temperature\": ") ;
    tmp.concat("\"") ;
    tmp.concat(String(t)) ;
    tmp.concat("\",") ;
    //---------------
    tmp.concat("\"Humidity\": ") ;
    tmp.concat("\"") ;
    tmp.concat(String(h)) ;
    tmp.concat("\" ") ;
     //---------------   
    tmp.concat("}") ;
    //---------------
   Serial.println(tmp) ;
    return tmp ;
  }

void CreateJsonData(String Mac,String IPStr,double sp, int way, double t,double h )
{
 
      doc["MAC"].set(Mac);
    doc["IP"].set(IPStr);
    doc["WindSpeed"].set(String(sp));
    doc["WindDirection"].set(String(way));
    doc["Temperature"].set(String(t));
    doc["Humidity"].set(String(h));

  /*  
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
    JSONencoder["MAC"] = Mac;
    JSONencoder["IP"] = IPStr;
    JSONencoder["WindSpeed"] = sp;
    JSONencoder["WindDirection"] = way;
    JSONencoder["Temperature"] = t;
    JSONencoder["Humidity"] = h;
    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
   
    if (client.publish("NCNU/WIND/", JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
    */
  }
