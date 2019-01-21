

/*
*    Arduino W5100 EtherCard ICT 2016 alkukoodit - edit 15.1.2016 KN
*/

#define DEBUG
#define SERIAL_ON

#include <SPI.h> 
#include <Ethernet.h> 
#include <EthernetUdp.h>
#include <PubSubClient.h> 
#include <TimerOne.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

#include <avr/pgmspace.h>

//#include <raspIP.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
//
//   
//   Kanavanumerot
//
//                    A     B
//
// Tuulen nopeus 1    1    21
// Tuulen suunta 1    2    22
// Lampotila 1        3    23
// Sademaara 1        4    24
// Kosteus sisa       5    25
// Kostesus ulko      6    26
// Tuulen suunta2     7    27
// Tuulen nopeus2     8    28
// Lampotila2         9    29
// Sademaara2         10   30 
// Ilmanpaine1        11   31
// Ilmanpaine2        12   32
/////////////////////////////////////////////////////////////////////////////////////////////
/// signaali                                                                              ///
/// ILmanpaine 1                                                                          ///
/////////////////                                                                         ///
///  Kanava nro                                                                           ///
                                                                                          /// 
#define   kanava         32       // A                                                    ///
#define   kanava_numero "32"                                                              ///
                                                                                          ///
////////////////                                                                          ///  
///MAC nro       // eka kanava MAC6 A luokka 0x21-0x32                                    ///
//                                  B luokka 0x41-0x52                                    /// 
#define  mac_6    0x52                                                                    ///
                                                                                          ///
/////////////////////////////////////////////////////////////////////////////////////////////


Adafruit_BMP085 bmp;

#define mittaus_m 1

LiquidCrystal lcd(8,7,6,5,4,3);

static uint8_t mymac[6] = { 0x54,0x55,0x58,0x10,0x00,mac_6 };

unsigned int localPort = 8888;       // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


 #define inTopic    "ict1_in_2018"         // * MQTT channel where lelylan updates are received 
 #define outTopic   "ict1_out_2018"        // * MQTT channel where lelylan updates are received 

 char* deviceId     = "2016";             // * set your device id (will be the MQTT client username) 
 char* deviceSecret = "tamk";         // * set your device secret (will be the MQTT client password) 
 char* clientId     = "1234567890";             // * set a random string (max 23 chars, will be the MQTT client id) 

 char website[] = "ntp.tamk.fi";

//////////////////
//// Raspberryn IP     Luetaan tiedostoa raspIP.h
//#define  IP_1    192
//#define  IP_2    168
//#define  IP_3    1
//#define  IP_4    2
//// Raspberryn IP TAMK ICT
//#ifdef IP_tamk
#define  IP_1    10
#define  IP_2    10
#define  IP_3    206
#define  IP_4    141
//#endif

 /* Server settings */ 
 byte server[] = { IP_1, IP_2, IP_3,IP_4 }; // MQTT serveri osoite
 
 
 /* Ethernet configuration */ 
EthernetClient ethClient; 
 
 /* MQTT communication */ 
void callback(char* topic, byte* payload, unsigned int length); // subscription callback 
 
PubSubClient client(server, 1883, callback, ethClient);         // mqtt client 
 

// Find list of servers at http://support.ntp.org/bin/view/Servers/StratumTwoTimeServers
// Please observe server restrictions with regard to access to these servers.

 // luetaan tiedostosra raspIP.h 
  //char website[]PROGMEM = "ntp2d.mcc.ac.uk";
 //char website[]PROGMEM = "ntp2c.mcc.ac.uk";
 //char website[]PROGMEM = "ntp.exnet.com";
 //char website[]PROGMEM = "ntp.cis.strath.ac.uk";
 //char website[]PROGMEM = "clock01.mnuk01.burstnet.eu";


byte aika_nro=0;  // onnistuneiden ajan lukujen maara
uint32_t paine;

uint32_t timeLong;
// Number of seconds between 1-Jan-1900 and 1-Jan-1970, unix time starts 1970
// and ntp time starts 1900.


void setup()
{
   
  
   lcd.begin(16,2); // Näytön koon määrittely 16 merkkiä 2 riviä
   
    lcd.setCursor(0,0);
    lcd.print("14.3.2018 ");
    
       
    lcd.setCursor(0,1);
          //   1234567890123456
    lcd.print("Odotan IP nroa");
     
    #ifdef SERIAL_ON
        Serial.begin(9600);
        Serial.println( F("EtherCard/Nanode NTP Client" ) );
    #endif
   
     hae_IP();    // Kytkeydytään TAMK:n verkkoon ja pyydetään IP-osoite.
     
    if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }

  lcd.clear();
} // Setup END


////////////////////////////////////////////////////
/////////////////  LOOPIN ALKU
///////////////////////////////////////////////////

void loop()
{

  
  paine = bmp.readPressure();  // Adafruit kirjaston funktio joka palauttaa painedatan pascaleina.
  lcd.setCursor(0,0);
  lcd.print(float(paine) / 100);
  lcd.setCursor(8,0);
  lcd.print("mbar");
    
  Serial.print("Pressure = ");
  Serial.print(paine);
  Serial.println(" Pa");
    
  
    

  lcd.setCursor(0,1);
  printTime();            // Noutaa palvelimelta kellonajan ja tulostaa sen LCD-näytölle muodossa hh:mm:ss
  
  Connect_MQTT_server();  // Otetaan yhteys MQTT serveriin
  send_message(paine);     // Ottaa parametrina painelukeman ja lähettää sen MQTT serverille
 
  Serial.println();

  delay(500);



}  // Loop End  


void printTime()
{
  laheta_pyynto();  // Lähettää ajanhakupyynnön palvelimelle.
  
  time_t aika = timeLong;           //Parsii ajanhakupyynnöstä saadun datan tulostettavaan aikamuotoon.
  int tunnit = hour(aika) + 2;
  int minuutit = minute(aika);
  int sekunnit = second(aika);
  
  if (tunnit < 10)
  {
    lcd.print("0");
    lcd.print(tunnit);
  }
  else
  {
    lcd.print(tunnit);
  }
  lcd.print(":");
  if (minuutit < 10)
  {
    lcd.print("0");
    lcd.print(minuutit);
  }
  else
  {
    lcd.print(minuutit);
  }
  lcd.print(":");
  if (sekunnit < 10)
  {
    lcd.print("0");
    lcd.print(sekunnit);
  }
  else
  {
    lcd.print(sekunnit);
  }
}







///////////////////////////////////////////
//////// IP numeron haku
//////////////////////////////////////////
void hae_IP(void)
{
byte rev=1;

  rev=Ethernet.begin( mymac);
  
  #ifdef SERIAL_ON  
    Serial.print( F("\nW5100 Revision ") );
  #endif
  
  if ( rev == 0){
                   #ifdef SERIAL_ON
                      Serial.println( F( "Failed to access Ethernet controller" ) );
                   #endif   
                                                // 1234567890123456 
                    lcd.setCursor(0,1); lcd.print("FAIL E cont conn");
                 }    
                 
  #ifdef SERIAL_ON               
     Serial.println( F( "Setting up DHCP" ));
     Serial.print("Connected with IP: "); 
     Serial.println(Ethernet.localIP()); 
  #endif
  
  lcd.setCursor(0,0);
  lcd.print("IP=");
  lcd.print(Ethernet.localIP());

  
}


///////////////////////////////////////////////
//////////  ajan hakupyynto
///////////////////////////////////////////////

void laheta_pyynto(void)
{
byte DSNError=1;

   Udp.begin(localPort);

   sendNTPpacket(website); // send an NTP packet to a time server

   delay(200);
   
   if (Udp.parsePacket()) 
   {
     // We've received a packet, read the data from it
    
     Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

     // the timestamp starts at byte 40 of the received packet and is four bytes,
     // or two words, long. First, extract the two words:

     unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
     unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
     // combine the four bytes (two words) into a long integer
     // this is NTP time (seconds since Jan 1 1900):
     unsigned long secsSince1900 = highWord << 16 | lowWord;
     
     // now convert NTP time into everyday time:
     // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
     const unsigned long seventyYears = 2208988800UL;
     // subtract seventy years:
     unsigned long epoch = secsSince1900 - seventyYears;
     timeLong=epoch;
   }
   else
   {
          delay(10000);
          Ethernet.maintain();
   }
         
   aika_nro++;     // ajan luku onnistui
     
  
}    /// ajan paivitys loppuuu


// send an NTP request to the time server at the given address

void sendNTPpacket(char* address) {

   // set all bytes in the buffer to 0
   memset(packetBuffer, 0, NTP_PACKET_SIZE);
   // Initialize values needed to form NTP request
   // (see URL above for details on the packets)
   packetBuffer[0] = 0b11100011;   // LI, Version, Mode
   packetBuffer[1] = 0;     // Stratum, or type of clock
   packetBuffer[2] = 6;     // Polling Interval
   packetBuffer[3] = 0xEC;  // Peer Clock Precision
   // 8 bytes of zero for Root Delay & Root Dispersion
   packetBuffer[12]  = 49;
   packetBuffer[13]  = 0x4E;
   packetBuffer[14]  = 49;
   packetBuffer[15]  = 52;

   // all NTP fields have been given values, now
   // you can send a packet requesting a timestamp:
   Udp.beginPacket(address, 123); //NTP requests are to port 123
   Udp.write(packetBuffer, NTP_PACKET_SIZE);
   Udp.endPacket();
}

/////////////////////////////////////////////////////////////////////////////////
//////////////// MQTT RUTIINIT
////////////////////////////////////////////////////////////////////////////////

/// Laheta sanoma
void send_message(uint32_t data)
{
    char bufa[25];
    sprintf(bufa,"32:+%06lu",(unsigned long)data);

    
    #ifdef SERIAL_ON
           Serial.println( bufa );
     #endif
          
     client.publish(outTopic,bufa);
  
}




 /* Receive MQTT message and confirm the physical change */ 
 
 void callback(char* topic, byte* payload, unsigned int length) 
 { 
   // copu the payload content into a char* 
   char* json; 
   json = (char*) malloc(length + 1); 
   memcpy(json, payload, length); 
   json[length] = '\0'; 
       lcd.setCursor(0,0);
       lcd.print(json);

    #ifdef SERIAL_ON
           Serial.println( json );
    #endif
   
    
   free(json); 
 } 



  /* MQTT server connection */ 
 
 void Connect_MQTT_server() 
 { 
 
   // add reconnection logics 

   if (!client.connected()) 
     { 
       // connection to MQTT server 
      if (client.connect(clientId, deviceId, deviceSecret)) 
        { 
           lcd.setCursor(0,1);
                    //0123456789012345//
           lcd.print("Connect         "); 

           #ifdef SERIAL_ON
              Serial.println(" Connected" );
           #endif

           MQTTSubscribe(); // topic subscription 
        } 
      } 
    
 }


  /* MQTT subscribe */ 
 
 void MQTTSubscribe() 
 { 
   client.subscribe(inTopic); 
 } 
 
 


