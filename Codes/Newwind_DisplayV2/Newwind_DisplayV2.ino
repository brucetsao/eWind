#include "arduino_secrets.h" 


#include <WiFi.h>
#include <WiFiMulti.h>
#include <MQTT.h>

WiFiMulti wifiMulti;
WiFiClient client;
WiFiClient mqclient;
MQTTClient mqttclient;


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
   // mqttclient.begin("broker.shiftr.io", mqclient);
 // mqttclient.onMessage(messageReceived);
//  mqttclient.onMessage(messageReceived); 

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

 // printWiFiStatus();
  ShowInternet() ;
  ShowString("init System...") ;
   mqttclient.begin("broker.shiftr.io", mqclient);
  mqttclient.onMessage(messageReceived);


  connectMQTT();
  
  ShowString("System  Ready") ;    
  Serial.println("System  Ready");

 
  }


void loop() {
 //  mqttclient.loop();
//  mqttclient.publish("/NCNU/Wind/", "world");
   if (!mqttclient.connected()) 
  {
     Serial.println("connectMQTT() again"); 
     connectMQTT();
  }
 
  mqttclient.loop();
  delay(2000);
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

void initall()
{
  Serial.begin(9600);      // initialize serial communication
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);     // initialize serial communication
  Serial.println("System Start ") ;
  MacData = GetMacAddress() ; 
    fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ; 
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
  
 // lcd.setCursor(12,0);
//  lcd.print("/");  
//  lcd.print(Apname);  

}
void ClearShow()
{
 //   lcd.setCursor(0,0);
//   lcd.clear() ;
 //   lcd.setCursor(0,0);
}

void ShowMAC()
{
 // lcd.setCursor(0,0);
//  lcd.print("                    ");  
//  lcd.setCursor(0,0);
//  lcd.print(MacData);  

}
void ShowIP()
{
//  lcd.setCursor(0,1);
//  lcd.print("                    ");  
//  lcd.setCursor(0,1);
//  lcd.print("IP:");  
//  lcd.print(ip);  

}




void ShowString(String ss)
{
//  lcd.setCursor(0,3);
//  lcd.print("                    ");  
//  lcd.setCursor(0,3);
//  lcd.print(ss.substring(0,19)); 
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




 void connectMQTT()
 {
//  Serial.print("MQTT ClientID is :(") ;
//  Serial.print(clintid) ;
//  Serial.print(")\n") ;
Serial.println("Now Connect MQTT Server") ;
  while (!mqttclient.connect(clintid, "try", "try")) {
    Serial.print("-");
    delay(1000);
  }
    Serial.print("\n");
  
  mqttclient.subscribe("/NCNU/Wind");
  mqttclient.onMessage(messageReceived);  
  Serial.println("\n MQTT connected!......");


  // client.unsubscribe("/hello");
}



void messageReceived(String &topic, String &payload) {
  Serial.println("Got Messaged");
  Serial.println("incoming: " + topic + " - " + payload);
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





void fillCID(String mm)
{
  // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
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


  }
