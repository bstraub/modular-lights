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

//settings
#define MODE_OFF 0
#define MODE_ON 1
#define MODE_BLINK 2
#define MODE_ALT 3
   
LiquidCrystal lcd(1,2,3,4,5,6,7);
int menuContext, menuNum;
int ch1Mode, ch2Mode, brightness, onTime, offTime; //settings
int v12, v5, iCh1, iCh2;




   
void setup()
{
  //initialize lcd
  //initialize h-bridge
  

}

void loop()
{
  
  
  
  
}
