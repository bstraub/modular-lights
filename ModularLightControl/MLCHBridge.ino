/* **************************************************
  *  MLCButtons.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 1.0
  *  Date: 06/06/13
  *
  *  Description:  This handles  the H-Bridge  control
  *     functions,     including     the      blinking 
  *     functionality.
  *     updateHBSettings() should be  called everytime
  *     that  the   settings  change,   and  updateHB()
  *     should be called continually.
  *
   ***************************************************/   
enum timingState {
  off,
  on1,
  on2,
  onFlop //aka both on
};

byte doubleFlag; // 0 = no double-on states, 1 = cycle contains double-on state (not necessarily now)
byte blinkFlag; // 0 = no blink, 1 = blink (on time), 2 = blink (off time)
timingState ts1, ts2; //for the two blinking states
timingState hbState; //the h-bridge state (won't use onFlop)
long blink1, blink2; //blink timers
int  ton, toff; //values actually used, in ms
byte mode;


void initHBridge()
{
  pinMode(hbEN, OUTPUT);
  pinMode(hb1,  OUTPUT);
  pinMode(hb2,  OUTPUT);
  digitalWrite(hbEN, LOW);
  digitalWrite(hb1,  LOW);
  digitalWrite(hb2,  LOW);
  hbState = off;

  updateHBSettings();
}

//uses the brightness global variable
void updateBrightness()
{
  if (((doubleFlag==0)||(blinkFlag==0)) //even if double, do normal if there's no blink
     || ((blinkFlag==1)&&(ts1==onFlop)) //or if in onFlop mode
     || ((blinkFlag==2)&&(ts2==onFlop)))
    analogWrite(hbEN, 255-brightness);
  else
    analogWrite(hbEN, (255-(brightness/2)));
    
}

//run when the mode is changed
void updateHBSettings()
{
  byte mode = (ch1Mode << 2) | ch2Mode;
  ton = onTime * 100;
  toff = offTime * 100;
  
  if ( ((mode & 2) == 2) || // ch2 = blink or alt
       ((mode & 8) == 8) )  // ch1 = blink or alt
  {
    blink1 = millis() + ton;
    blink2 = blink1 + toff;
    
    blinkFlag = 1;
  } else {
    blinkFlag = 0;
  }
  
  
  switch(mode)
  {
    case 0:  // both off
      ts1 = off;
      doubleFlag = 0;
      break;
    case 1:  // 1 off, 2 on
      ts1 = on2;
      doubleFlag = 0;
      break;
    case 2:  // 1 off, 2 blink
      ts1 = on2;
      ts2 = off;
      doubleFlag = 0;
      break;
    case 3:  // 1 off, 2 alt
      ts1 = off;
      ts2 = on2;
      doubleFlag = 0;
      break;
    case 4:  // 1 on, 2 off
      ts1 = on1;
      doubleFlag = 0;
      break;
    case 5:  // 1 on, 2 on
      ts1 = onFlop;
      doubleFlag = 1;
      break;
    case 6:  // 1 on, 2 blink
      ts1 = onFlop;
      ts2 = on1;
      doubleFlag = 1;
      break;
    case 7:  // 1 on, 2 alt
      ts1 = on1;
      ts2 = onFlop;
      doubleFlag = 1;
      break;
    case 8:  // 1 blink, 2 off
      ts1 = on1;
      ts2 = off;
      doubleFlag = 0;
      break;
    case 9:  // 1 blink, 2 on
      ts1 = onFlop;
      ts2 = on2;
      doubleFlag = 1;
      break;
    case 10: // 1 blink, 2 blink
      ts1 = onFlop;
      ts2 = off;
      doubleFlag = 1;
      break;
    case 11: // 1 blink, 2 alt
      ts1 = on1;
      ts2 = on2;
      doubleFlag = 0;
      break;
    case 12: // 1 alt, 2 off
      ts1 = off;
      ts2 = on1;
      doubleFlag = 0;
      break;
    case 13: // 1 alt, 2 on
      ts1 = on2;
      ts2 = onFlop;
      doubleFlag = 1;
      break;
    case 14: // 1 alt, 2 blink
      ts1 = on2;
      ts2 = on1;
      doubleFlag = 0;
      break;
    case 15: // 1 alt, 2 alt
      ts1 = off;
      ts2 = onFlop;
      doubleFlag = 1;
      break;
    default:
      ts1 = off;
      ts2 = off;
      doubleFlag = 0;
      break;
  } // end switch(mode)
  
  // now go ahead and get it started
  switch(ts1)
  {
    case off:
      HBOff();
      break;
    case on1:
      HBOn1();
      break;
    case on2:
      HBOn2();
      break;
    case onFlop:
      HBFlop();
      break;
  } // end switch(ts1)

  updateBrightness();
  
}

void updateHB()
{
  if(blinkFlag == 0) //no blinking
  {
    if (ts1 == onFlop)
      HBFlop();
  } else if (blinkFlag == 1)
  {
    if (((int)(millis()-blink1))>0) //is on time done?
    {
      blink1 = blink2 + ton;
      blinkFlag = 2;
      
      //get next portion started
      switch(ts2)
      {
        case off:
          HBOff();
          break;
        case on1:
          HBOn1();
          break;
        case on2:
          HBOn2();
          break;
        case onFlop:
          HBFlop();
          break;
      } // end switch(ts2)
      updateBrightness();
    } //end if(ontime done)
    else if (ts1 == onFlop) //not done, but flop
    {
      HBFlop();
    }
  } else if (blinkFlag == 2)
  {
    if (((int)(millis()-blink2))>0) //is on time done?
    {
      blink2 = blink1 + toff;
      blinkFlag = 1;
      //get next portion started
      switch(ts1)
      {
        case off:
          HBOff();
          break;
        case on1:
          HBOn1();
          break;
        case on2:
          HBOn2();
          break;
        case onFlop:
          HBFlop();
          break;
      } // end switch(ts1)
      updateBrightness();
    } //end if(offtime done)
    else if (ts2 == onFlop) //not done, but flop
    {
      HBFlop();
    }
  } // end flag checking
}

void HBOn1()
{
  digitalWrite(hb2, LOW);
  digitalWrite(hb1, HIGH);
  hbState = on1;
  changeTime = micros();
}

void HBOn2()
{
  digitalWrite(hb1, LOW);
  digitalWrite(hb2, HIGH);
  hbState = on2;
  changeTime = micros();
}

void HBOff()
{
  digitalWrite(hb1, LOW);
  digitalWrite(hb2, LOW);
  hbState = off;
  changeTime = micros();
}

void HBFlop()
{
  if ((micros() - changeTime) >= FLOP_TIME) {
    if ((hbState) != on1)
      HBOn1();
    else
      HBOn2();
  }
}
