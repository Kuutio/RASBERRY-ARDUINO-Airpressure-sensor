#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>

#include <SPI.h> 
#include <Ethernet.h> 
#include <EthernetUdp.h>

#define  mac_6    0x52 

Adafruit_BMP085 bmp;
static uint8_t mymac[6] = { 0x54,0x55,0x58,0x10,0x00,mac_6 };
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

uint32_t paine;

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

void setup() {

  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600);
  Serial.println("test1");
  hae_IP();
  Serial.println("test2");
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
}
  
void loop() {

  paine = bmp.readPressure();
  
  lcd.setCursor(0,1);
  lcd.print(float(paine) / 100);
  lcd.setCursor(8,1);
  lcd.print("mbar");
    
  Serial.print("Pressure = ");
  Serial.print(paine);
  Serial.println(" Pa");
    
  Serial.println();
  delay(500);
}
