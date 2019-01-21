#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

void setup() {
  // put your setup code here, to run once:

  lcd.begin(16, 2);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.setCursor(0,0);
  lcd.print("TEST");
  delay(1000);
}
