#include <String.h>

#define RXD2 16
#define TXD2 17
String Apname;


char Oledchar[30] ;
//char* AP2 = "Brucetsao" ;
//char* PW2 = "12345678";

char* AP2 = "NCNUIOT" ;
char* PW2 = "12345678";
char* AP1 = "lib-tree" ;
char* PW1 = "wtes26201959";

#define maxfeekbacktime 5000

char cmd ;

String ChartoString(const char* cc)
{
    int count= 0 ;
    String tmp ;
    while (cc[count] != 0x00)
      {
        tmp.concat(cc[count]) ;
        count++ ;
      }
   return tmp ;
}


 
