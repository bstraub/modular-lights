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
   
#define DEBOUNCE_COUNT 300 //tweak these
#define HOLD_COUNT 30000   //values !4

const int MENU_OPTIONS[] = {3,5,4,6};
int deb, hold, debEnter, debBack;
byte btnState, prevState;

void initButtons()
{
  deb = hold = debEnter = debBack = 0;
  btnState = prevState = 0;
  menuContext = menuNum = menuSelect = 0;
}

void checkBtn()
{
  if (!menuSelect) {
    checkEnterBack();
    checkUpDown();
  } else {
    //!1
  }
}
  
void checkEnterBack()
{
  if (digitalRead(btnEnter)) {
    if (debEnter <= 0) {
      debEnter = DEBOUNCE_COUNT; 
      if (menuContext == 0)
      {
        menuContext = menuNum + 1;
        menuNum = 0;
        printScreen();
      } else {
        menuSelect = true;
        drawSelection();
      }
    }
  } else {
    if (debEnter > 0) {
      debEnter--;
    }
  }
  
  if (digitalRead(btnBack)) {
    if (debEnter <= 0) {
      debBack = DEBOUNCE_COUNT;
      if (menuContext != 0)
      {
        menuContext = 0;
        menuNum =0;
        printScreen();
      }
    }
  } else {
    if (debBack > 0) {
      debBack--;
    }
  }
      
}
    

void checkUpDown()
{
  btnState = (digitalRead(btnUp) << 1) | (digitalRead(btnDown));
  
  switch (btnState) {
    case 1: //down
      if (prevState==1) {
        if(hold <= 0) {
          menuNum = ((menuNum - 1) + MENU_OPTIONS[menuContext]) % MENU_OPTIONS[menuContext];
          hold = HOLD_COUNT;
          deb = DEBOUNCE_COUNT;
          printScreen();
        } else {
          hold--;
        }
      } else if (prevState==2) {
        menuNum = ((menuNum - 1) + MENU_OPTIONS[menuContext]) % MENU_OPTIONS[menuContext];
        hold = HOLD_COUNT;
        deb = DEBOUNCE_COUNT;
        printScreen();
      } else { //prevState == 0 or 3
        if (deb == 0){
          menuNum = ((menuNum - 1) + MENU_OPTIONS[menuContext]) % MENU_OPTIONS[menuContext];
          hold = HOLD_COUNT;
          deb = DEBOUNCE_COUNT;
          printScreen();
        }
      }
      break; //end case 1
    case 2: //up
      if (prevState==2) {
        if(hold <= 0) {
          menuNum = (menuNum + 1) % MENU_OPTIONS[menuContext];
          hold = HOLD_COUNT;
          deb = DEBOUNCE_COUNT;
          printScreen();
        } else {
          hold--;
        }
      } else if (prevState==1) {
        menuNum = (menuNum + 1) % MENU_OPTIONS[menuContext];
        hold = HOLD_COUNT;
        deb = DEBOUNCE_COUNT;
        printScreen();
      } else { //prevState == 0 or 3
        if (deb == 0){
          menuNum = (menuNum + 1) % MENU_OPTIONS[menuContext];
          hold = HOLD_COUNT;
          deb = DEBOUNCE_COUNT;
          printScreen();
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
  prevState = btnState;
}
        
      
