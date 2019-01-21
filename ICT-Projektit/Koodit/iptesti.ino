#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>
//
#include <SPI.h> 
#include <Ethernet.h> 
#include <EthernetUdp.h>
//#include <raspIP.h>

//Luokan osoite
#define  mac_6    0x52 
#define SERIAL_ON

Adafruit_BMP085 bmp;
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

//mac osoite
static uint8_t mymac[6] = { 0x54,0x55,0x58,0x10,0x00,mac_6 };

byte ip[] = {192,168,112,99};

EthernetClient ethClient;
uint32_t paine;


void hae_IP(void)
{
  Serial.print("test1: ");
byte rev=1;

//  if (Ethernet.begin(mymac, ip) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // no point in carrying on, so do nothing forevermore:
//    while(true);
//  }

  //rev=Ethernet.begin(mymac,ip);
  Ethernet.begin(mymac,ip);
  Serial.print("test2: ");
  
  #ifdef SERIAL_ON  
    Serial.print( F("\nW5100 Revision ") );
  #endif
  Serial.print("test3: ");
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

void setup() {

  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600);
  hae_IP();
 // if (!bmp.begin()) {
 // Serial.println("Could not find a valid BMP085 sensor, check wiring!");
 // }
}
  
void loop() {

 // paine = bmp.readPressure();
  
 // lcd.setCursor(0,1);
 // lcd.print(float(paine) / 100);
 // lcd.setCursor(8,1);
 // lcd.print("mbar");
    
  //Serial.print("Pressure = ");
  //Serial.print(paine);
  //Serial.println(" Pa");
    
  //Serial.println();
  delay(500);
}


