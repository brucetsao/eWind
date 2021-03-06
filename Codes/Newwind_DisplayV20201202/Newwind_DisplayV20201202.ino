#include "arduino_secrets.h" 
#include "lumex.h" 
#include "MQTTLib.h" 


#include <AutoConnect.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include <PubSubClient.h>



WiFiMulti wifiMulti;

WiFiClient mqclient;
PubSubClient mqttclient(mqclient) ;


//-----------------------



//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
          // your network key Index number (needed only for WEP)

  IPAddress ip ;
  long rssi ;
  
int status = WL_IDLE_STATUS;

String MacData ;
String IPData;
String IPData2;


//  control parameter 
boolean systemstatus = false ;
boolean Reading = false ;
boolean Readok = false ;
// int trycount = 0 ;
int wifierror = 0 ;
boolean btnflag = false ;
//---------------

String WindWay[] = {"北風","東北風","東風","東南風","南風","西南風","西風","西北風" } ;
//String WindEWay[] = {"North","East North","East","East South","South","West South","West","West North" } ;
int Winddir=0 ;
int Windangle=0 ;
String WinddirName =WindWay[Winddir]  ;
double Windspeed =0,  Temp=0, Humid =0 ;
//----------------
#include <ArduinoJson.h>


char clintid[20];
const int capacity = JSON_OBJECT_SIZE(6);
 StaticJsonDocument<512> doc;
char JSONmessageBuffer[300];
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


  
    wifiMulti.addAP(AP1, PW1);
    wifiMulti.addAP(AP2, PW2);
    wifiMulti.addAP(AP3, PW3);

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
  IPData = IpAddress2String(ip) ;
  IPData2 = LocalIpAddress2String(ip) ;
  ShowInternet() ;
  mqttclient.setServer("broker.shiftr.io", 1883);
  mqttclient.setCallback(callback);

  fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;

  connectMQTT();
  ClearScreen() ;
  ShowInternet() ;
  delay(5000);
  Serial.println("System  Ready");
    ClearScreen() ;
    SendImage(schoolname) ;
    delay(3000);
     SendImage(sitetitle) ;   
 
  }


void loop() {
   
    /*
     if (WiFi.status() == WL_IDLE_STATUS) 
    {
    
        #if defined(ARDUINO_ARCH_ESP8266)
            Serial.println("No Wifi and Rebooting") ;
            ESP.reset();
        #elif defined(ARDUINO_ARCH_ESP32)
            Serial.println("No Wifi and Rebooting") ;
            ESP.restart();
        #endif
            delay(1000);
  }
  */
 //  mqttclient.loop();
//  mqttclient.publish("/NCNU/Wind/", "world");
   if (!mqttclient.connected()) 
  {
     Serial.println("connectMQTT  again"); 
     connectMQTT();
  }
 
  mqttclient.loop();
  delay(10000);
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
  myHardwareSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("System Start ") ;

}


//------------------
void ShowInternet()
{
      SendMessage(0,0,MacData) ;
      SendMessage(1,0,"AP:"+Apname) ;
      SendMessage(2,0,IPData2) ;      

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
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  long strtime = millis() ;
  while (!mqttclient.connect(clintid, "try", "try")) {
//  while (!mqttclient.connect(clintid)) {
    Serial.print("-");
    delay(1000);
    if ((millis()-strtime )>WaitingTimetoReboot )
      {
            Serial.println("No Wifi and Rebooting") ;
            SendMessage(3,0,"Rebooting.") ;            
            ESP.restart();        
      }
  }
    Serial.print("\n");
  
  mqttclient.subscribe(SubTopic);
  Serial.println("\n MQTT connected!");


  // client.unsubscribe("/hello");
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");
   deserializeJson(doc, payload, length);
  JsonObject documentRoot = doc.as<JsonObject>();


  Serial.print("MAC:") ;
  const char* a1 = documentRoot.getMember("MAC") ;
  //讀取網路卡編號
  Serial.println(a1);
  //--------------------
 
  Serial.print("IP:");
  const char* a2 = documentRoot.getMember("IP") ;
  //讀取IP網址
  Serial.println(a2);
  //--------------------
  Serial.print("WindSpeed:");
  const char* a3 = documentRoot.getMember("WindSpeed") ;
    //讀取風速
  //SendMessage(0,0,a3) ;
  Serial.println(a3);
  //--------------------
  Serial.print("WindDirection:");
  const char* a4 = documentRoot.getMember("WindDirection") ;
    //讀取風向
  Serial.println(a4);
  //--------------------
  Serial.print("Temperature:");
  const char* a5 = documentRoot.getMember("Temperature") ;
    //讀取溫度
  Serial.println(a5);
  //--------------------
  Serial.print("Humidity:");
  const char* a6 = documentRoot.getMember("Humidity") ;
    //讀取濕度
  Serial.println(a6);
  //--------------------
 Windspeed = ChartoString(a3).toDouble(); //轉換風速
 Winddir = ChartoString(a4).toInt();  //轉換風向
 Temp = ChartoString(a5).toDouble();  //轉換溫度
 Humid = ChartoString(a6).toDouble(); //轉換溼度

    ClearScreen() ;   //清除獨立動態顯示裝置的畫面。
   SendImage(sitetitle) ;
   delay(1500);
  SendSensortoLumex(Windspeed,Winddir,Temp,Humid);
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
  //return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
  return ipAddress.toString() ;
}

String LocalIpAddress2String(const IPAddress& ipAddress)
{
  //return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
  return String(String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]))  ;
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
