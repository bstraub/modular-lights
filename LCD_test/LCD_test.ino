#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,9,10,11,12);
long time;
long msec, sec, minute;

void setup()
{
  lcd.begin(16,2);
  lcd.print("-This is a test-");
}

void loop()
{
  lcd.setCursor(0,1);
  time = millis();
  msec = time % 1000;
  sec = time/1000;
  minute = sec / 60;
  sec = sec % 60;
  
  if (0 < minute)
  {
    lcd.print(minute);
    lcd.print(":");
  }
  if ((10 > sec) && (0 < minute))
    lcd.print("0");
  lcd.print(sec);
  lcd.print(".");
  lcd.print(msec);
  
}
