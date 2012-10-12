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
   
#define DEBOUNCE_COUNT 30

const int MENU_OPTIONS[] = {3,5,4,6};




void checkButton()
{
  btnState = (digitalRead(btnUp) << 1) | (digitalRead(btnDown));
  
  switch (btnState) {
    case 1: //down
      if (prevState==1) {
        if(!hold--) {
          menuNum = (menuNum - 1) //modulo?  fix this tomorrow
        ///////
        //////
