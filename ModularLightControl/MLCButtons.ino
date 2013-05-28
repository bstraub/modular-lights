/* **************************************************
  *  MLCButtons.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 0.0
  *  Date: --
  *
  *  Description:  This  handles  all  of  the  button
  *     presses,  and  calls screen update  functions.
  *     Also handles fast scrolling.
  *
   ***************************************************/
   
#define DEBOUNCE_COUNT 50 //tweak these
#define HOLD_COUNT 30000   //values !4

const int MENU_OPTIONS[] = {3,5,4,6};
long deb, hold, debEnter, debBack;
byte btnState, prevState;
byte enterState, backState;

void initButtons()
{
  deb = hold = debEnter = debBack = 0;
  btnState = prevState = 0;
  enterState = backState = 0;
  menuContext = menuNum = menuSelect = menuCursor = 0;
}

void checkBtn()
{
  checkEnterBack();
  checkUpDown();
}

void checkEnterBack()
{
  if (digitalRead(btnEnter)) {
    if (debEnter <= 0) {
      debEnter = DEBOUNCE_COUNT;
      
      if (menuContext == 0) //main menu
      {
        menuContext = ((menuNum + menuCursor) % 3 ) + 1; //math!
        menuNum = 0;
        printScreen();
      } else if (!menuSelect) { //other menus, but not selected
        if (menuContext == 1) //settings menu
        {
          menuSelect = true;
          drawSelection();
          storeSetting();
        }
        //otherwise, there's nothing to select
      } else { //something's selected
        menuSelect = false;
        drawDeselection();
      } //end context check
      
    } //end if debounced
    
  } else { //else, if not pressed
    if (debEnter > 0) {
      debEnter--; //decrease the debounce counter
    }
  } //end checking enter button

  if (digitalRead(btnBack)) {
    
    if (debBack <= 0) {
      debBack = DEBOUNCE_COUNT;
      
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
    } //end debounce check
  } else { //else if not pressed
    if (debBack > 0) {
      debBack--; //decrease the debounce counter
    }
  } //end checking back button
      
}
    

void checkUpDown()
{
  btnState = (digitalRead(btnUp) << 1) | (digitalRead(btnDown));
  
  if (!menuSelect)
  {
    
    switch (btnState) {
      case 1: //down
        if (prevState==1) { //held down
          if(hold <= 0) {
            menuDown();
            hold = HOLD_COUNT;
            deb = DEBOUNCE_COUNT;
          } else {
            hold--;
          }
        } else if (prevState==2) { //up then down
            menuDown();
            hold = HOLD_COUNT;
            deb = DEBOUNCE_COUNT;
        } else { //prevState == 0 or 3 //none then down, or both then just down
          if (deb == 0){
            menuDown();
            hold = HOLD_COUNT;
            deb = DEBOUNCE_COUNT;
          }
        }
        break; //end case 1
      case 2: //up
        if (prevState==2) { //held up
          if(hold <= 0) {
            menuUp();
            hold = HOLD_COUNT;
            deb = DEBOUNCE_COUNT;
          } else {
            hold--;
          }
        } else if (prevState==1) { //down then up
          menuUp();
          hold = HOLD_COUNT;
          deb = DEBOUNCE_COUNT;
        } else { //prevState == 0 or 3
          if (deb == 0){
            menuUp();
            hold = HOLD_COUNT;
            deb = DEBOUNCE_COUNT;
          }
        }
        break; //end case 2
      case 0:
      case 3:
        if (deb > 0)
          deb--;
        if ((prevState == 1) || (prevState == 2)) {
          hold = 0;
        }
        break; //end case 0 + 3
    } //end switch(btnState)
  
  } //end if not menuSelect
  
  else //selected
  {
    switch (btnState) {
      case 1: //down
        break;
    }
        
  
  }
  
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
      break;
    case 1:
      currentSetting = &ch2Mode;
      break;
    case 2:
      currentSetting = &brightness;
      break;
    case 3:
      currentSetting = &onTime;
      break;
    case 4:
      currentSetting = &offTime;
      break;
  }
  settingHolder = *currentSetting; 
}

void restoreSetting()
{
  *currentSetting = settingHolder;
}

void saveSetting()
{
  //!3 save setting into EEPROM
}

      
