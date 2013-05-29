/* **********************************************************
  *  ModularLightControl.ino
  *
  *  Author:  Ben Straub
  *  Revision: 0.0
  *  Date: --
  *
  *  Description:  This is the main file for the modular light
  *     controller.  This controller will drive an LCD display
  *     (handled by MLCScreen), use pushbutton inputs (handled
  *     by MLCButtons),  control an H-bridge to drive the LEDs
  *     (MLCHBridge),  and read back stats  about  the  system
  *     (MLCStat).
  *     
   ***********************************************************/

#include <LiquidCrystal.h>
#include <EEPROM.h>

//settings
#define MODE_OFF 0
#define MODE_ON 1
#define MODE_BLINK 2
#define MODE_ALT 3
   
typedef const byte pin;

pin btnUp    = 7;  ///placeholder
pin btnDown  = 8;  ///pin numbers
pin btnEnter = 6;  // make sure to
pin btnBack  = 5;  // set correctly !2

pin lcdRS    = 2;
pin lcdRW    = 3;
pin lcdEN    = 4;
pin lcdD4    = 9;
pin lcdD5    = 10;
pin lcdD6    = 11;
pin lcdD7    = 12;
   
LiquidCrystal lcd(lcdRS,lcdRW,lcdEN,lcdD4,lcdD5,lcdD6,lcdD7);
char menuContext, menuNum, menuSelect, menuCursor;
int ch1Mode, ch2Mode, brightness, onTime, offTime; //settings
int v12, v5, iCh1, iCh2;
int settingHolder, minSetting, maxSetting;
int *currentSetting;

//menuContext tells which menu is open.
  // 0 is main (outermost) menu.
  // 1 is settings
  // 2 is statistics
  // 3 is about

//menuNum tells what line within a menu is on screen.
  // range of 0 to MENU_OPTIONS[menuContext]

//menuSelect tells if a setting or changeable option has been selected (pressed enter) within a menu.
  //true or false

//menuCursor tells whether the cursor is at the top or bottom line. Only used in menuContext = 0.
  // 0 is top
  // 1 is bottom


   
void setup()
{
  Serial.begin(9600);
  Serial.println("go");
  initButtons();
  initScreen();
  loadSettings();
  //initialize h-bridge !1

}

void loop()
{
  checkBtn();
  
}
