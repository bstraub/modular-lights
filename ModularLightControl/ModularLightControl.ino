/* **********************************************************
  *  ModularLightControl.ino
  *
  *  Author:  Ben Straub
  *  Revision: 1.0
  *  Date: 06/06/13
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

//timing
#define FLOP_TIME 4000 //in us
#define VALID_READ_TIME 200 //in us
#define VALID_READ_COUNTS 25  //each count is 8us

//settings
#define MODE_OFF 0
#define MODE_ON 1
#define MODE_BLINK 2
#define MODE_ALT 3
   
typedef const byte pin;

pin btnUp    = 11;//7;  ///placeholder
pin btnDown  = 10;//8;  ///pin numbers
pin btnEnter = 12;//6;  // make sure to
pin btnBack  = 13; // set correctly 

pin lcdRS    = 2;
pin lcdRW    = 3;
pin lcdEN    = 4;
pin lcdD4    = 8;//9;
pin lcdD5    = 7;//10;
pin lcdD6    = 6;//11;
pin lcdD7    = 5;//12;

pin hbEN     = 9; //must be PWM-able. WARNING: if you change this, you must change the timer registers
                    // used in checkEnSwitch()
pin hb1      = A0;
pin hb2      = A1;

pin stat12V  = A2; //12V rail
pin stat5V   = A3; // 5V rail
pin statI1   = A4; //Ch A current
pin statI2   = A5; //Ch B current

   
LiquidCrystal lcd(lcdRS,lcdRW,lcdEN,lcdD4,lcdD5,lcdD6,lcdD7);
char menuContext, menuNum, menuSelect, menuCursor;
int ch1Mode, ch2Mode, brightness, onTime, offTime; //settings
int v12, v5, iCh1, iCh2;
int settingHolder, minSetting, maxSetting;
int *currentSetting;
byte priority = 0xE4; //holds the stat-reading priority: 11 10 01 00 = i2 i1 v12 v5
byte curCh = 0; //current channel, 0=off, 1=ch1, 2=ch2
long changeTime = 0; //timestamp (in ms) of the last HB switch

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
  initHBridge();
  initStats();
}

void loop()
{
  checkBtn();
  updateHB();
  readStat();
}
