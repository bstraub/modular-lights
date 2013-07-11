/* **************************************************
  *  MLCButtons.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 1.0
  *  Date: 06/06/13
  *
  *  Description:  This  handles  all  of  the  button
  *     presses,  and  calls screen update  functions.
  *     Also handles fast scrolling.
  *
   ***************************************************/
   
#define DEBOUNCE_COUNT 50 
#define HOLD_COUNT 1000   

const int MENU_OPTIONS[] = {3,5,4,6};
long lastHold;
int scrollStart;
byte btnState, prevState;
byte enterState, backState, upState, downState; //debounced states
byte enterRising, backRising; //rising edges

void initButtons()
{
  lastHold = 0;
  btnState = prevState = 0;
  enterRising = backRising = 0;
  enterState = backState = upState = downState= 0;
  menuContext = menuNum = menuSelect = menuCursor = 0;
}

void checkBtn()
{
  checkEnter();
  checkBack();
  checkUpDown();
}

void debounceEnter()
{
  static int lastReading;
  static long lastDeb;
  
  int reading = digitalRead(btnEnter);
  if (reading != lastReading)
    lastDeb = millis();
  if ((millis()-lastDeb) > DEBOUNCE_COUNT)
  {
    if ((enterState != reading) && reading) //check for rising edge
      enterRising = true;
    enterState = reading;
  }
  lastReading = reading;
}

void debounceBack()
{
  static int lastReading;
  static long lastDeb;
  
  
  int reading = digitalRead(btnBack);
  if (reading != lastReading)
    lastDeb = millis();
  if ((millis()-lastDeb) > DEBOUNCE_COUNT)
  {
    if ((backState != reading) && reading) //check for rising edge
      backRising = true;
    backState = reading;
  }
  lastReading = reading;
}

void debounceUpDown()
{
  static int lastReadingUp, lastReadingDown;
  static long lastDebUp, lastDebDown;
  
  int readingUp = digitalRead(btnUp);
  int readingDown = digitalRead(btnDown);
  if (readingUp != lastReadingUp)
    lastDebUp = millis();
  if (readingDown != lastReadingDown)
    lastDebDown = millis();
  if ((millis()-lastDebUp) > DEBOUNCE_COUNT)
    upState = readingUp;
  if ((millis()-lastDebDown) > DEBOUNCE_COUNT)
    downState = readingDown;
    
  lastReadingUp = readingUp;
  lastReadingDown = readingDown;
}

void checkEnter()
{
  debounceEnter();
  if (enterRising) {
    if (menuContext == 0) //main menu
    {
      menuContext = ((menuNum + menuCursor) % 3 ) + 1; //math!
      menuNum = 0;
      printScreen();
    } else if (!menuSelect) { //other menus, but not selected
      if (menuContext == 1) //settings menu
      {
        menuSelect = true;
        prevState = 0; //done to make sure it doesn't keep scrolling
        drawSelection();
        storeSetting();
      }
      //otherwise, there's nothing to select
    } else { //something's selected
      menuSelect = false;
      saveSetting();
      updateHBSettings();
      drawDeselection();
    } //end context check
    enterRising = false; //it's been handled
  } //end rising edge check
}//end checkEnter()


void checkBack()
{
  debounceBack();
  if (backRising) {
    if ((menuContext != 0) && (!menuSelect))
    {
      menuNum = (menuContext + 2 - menuCursor) % 3; //math!
      menuContext = 0;
      printScreen();
    } else if (menuSelect) {
      //go back out of selection mode without saving value
      
      restoreSetting();
      drawDeselection();
      menuSelect = false;
    } //end context check
    backRising = false; //it's been handled
  }//end rising edge check
}
    

void checkUpDown()
{
  double heldTime;
  double heldTime4;
  debounceUpDown();
  btnState = (upState << 1) | (downState);

  
  if (!menuSelect)
  {
    
    switch (btnState) {
      case 1: //down
        if (prevState==1) { //held down
          if(millis()-lastHold > HOLD_COUNT) {
            menuDown();
            lastHold = millis();
          }
        } else if ((prevState & 1) == 0) { //state is 0 or 2 (from nothing or just up)
            menuDown();
            lastHold = millis();
        } else { //let go of up btn but still hold down
            lastHold = millis(); //reset hold counter, but don't scroll
        }
        break; //end case 1
      case 2: //up
        if (prevState==2) { //held up
          if(millis()-lastHold > HOLD_COUNT) {
            menuUp();
            lastHold = millis();
          }
        } else if ((prevState & 2) == 0) { //state 0 or 1 (from nothing or just down)
          menuUp();
          lastHold = millis();
        } else { //just let go of down btn but still hold up
          lastHold = millis(); //reset hold counter, but don't scroll
        }
        break; //end case 2
      case 0:
      case 3:
        break; //end case 0 + 3
    } //end switch(btnState)
  
  } //end if not menuSelect
  
  else //selected
  {
    switch (btnState) {
      case 1: //down
        if (*currentSetting != minSetting)
        {
          if (prevState==1) //held down
          {
            heldTime = (millis()-lastHold)/1000.0;
            heldTime4 = heldTime * heldTime;
            heldTime4 = heldTime4 * heldTime4;
            *currentSetting = scrollStart - (int)((heldTime+heldTime4/4)+0.5);
          } else if ((prevState&1)==0) { //0 or 2
            *currentSetting -= 1;
            scrollStart = *currentSetting;
            lastHold = millis();
          } else { //from 3 (both)
            scrollStart = *currentSetting;
            lastHold = millis();
          }
          if (*currentSetting < minSetting)
            *currentSetting = minSetting;
        }
        break;
      case 2: //up
        if (*currentSetting != maxSetting)
        {
          if (prevState==2) //held up
          {
            heldTime = (millis()-lastHold)/1000.0;
            heldTime4 = heldTime * heldTime;
            heldTime4 = heldTime4 * heldTime4;
            *currentSetting = scrollStart + (int)((heldTime+heldTime4/4)+0.5);
          } else if ((prevState&2)==0) { //0 or 1
            *currentSetting += 1;
            scrollStart = *currentSetting;
            lastHold = millis();
          } else { //from 3 (both)
            scrollStart = *currentSetting;
            lastHold = millis();
          }
          if (*currentSetting > maxSetting)
            *currentSetting = maxSetting;
        }
        break;
      case 0:
      case 3:
        break; //nothin to do
    }
        
  updateScreen();
  }//end if selected
  
  prevState = btnState;
}

void menuDown()
{
  if (menuContext == 0) { //main menu
    menuNum = (menuNum + menuCursor);
    menuCursor = 1;
  } else { //other menus
    menuNum = (menuNum + 1);
  }
  if (menuNum >= MENU_OPTIONS[menuContext])
    menuNum = 0; //wrap around
  printScreen();
}

void menuUp()
{
  if (menuContext == 0) {
    menuNum = (menuNum + menuCursor - 1);
    menuCursor = 0;
  } else {
    menuNum = (menuNum - 1);
  }
  if (menuNum < 0)
    menuNum = (menuNum + MENU_OPTIONS[menuContext]);
  printScreen();
}

void storeSetting()
{
  switch( menuNum )
  {
    case 0:
      currentSetting = &ch1Mode;
      minSetting = 0;
      maxSetting = 3;
      break;
    case 1:
      currentSetting = &ch2Mode;
      minSetting = 0;
      maxSetting = 3;
      break;
    case 2:
      currentSetting = &brightness;
      minSetting = 0;
      maxSetting = 254;
      break;
    case 3:
      currentSetting = &onTime;
      minSetting = 0;
      maxSetting = 99;
      break;
    case 4:
      currentSetting = &offTime;
      minSetting = 0;
      maxSetting = 99;
      break;
  }
  settingHolder = *currentSetting; 
}

void restoreSetting()
{
  *currentSetting = settingHolder;
}


      
