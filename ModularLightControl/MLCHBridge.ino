/* **************************************************
  *  MLCButtons.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 0.0
  *  Date: --
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
  onFlop
};

byte blinkFlag; // 0 = no blink, 1 = blink (on time), 2 = blink (off time)
timingState ts1, ts2; //for the two blinking states

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

  updateHBSettings();
}

//uses the brightness global variable
void updateBrightness()
{
  analogWrite(hbEN, brightness);
}

//run when the mode is changed
void updateHBSettings()
{
  byte mode = (ch1Mode << 2) | ch2Mode;
  ton = onTime * 100;
  toff = offTime * 100;
  updateBrightness();
  
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
      break;
    case 1:  // 1 off, 2 on
      ts1 = on2;
      break;
    case 2:  // 1 off, 2 blink
      ts1 = on2;
      ts2 = off;
      break;
    case 3:  // 1 off, 2 alt
      ts1 = off;
      ts2 = on2;
      break;
    case 4:  // 1 on, 2 off
      ts1 = on1;
      break;
    case 5:  // 1 on, 2 on
      ts1 = onFlop;
      break;
    case 6:  // 1 on, 2 blink
      ts1 = onFlop;
      ts2 = on1;
      break;
    case 7:  // 1 on, 2 alt
      ts1 = on1;
      ts2 = onFlop;
      break;
    case 8:  // 1 blink, 2 off
      ts1 = on1;
      ts2 = off;
      break;
    case 9:  // 1 blink, 2 on
      ts1 = onFlop;
      ts2 = on2;
      break;
    case 10: // 1 blink, 2 blink
      ts1 = onFlop;
      ts2 = off;
      break;
    case 11: // 1 blink, 2 alt
      ts1 = on1;
      ts2 = on2;
      break;
    case 12: // 1 alt, 2 off
      ts1 = off;
      ts2 = on1;
      break;
    case 13: // 1 alt, 2 on
      ts1 = on2;
      ts2 = onFlop;
      break;
    case 14: // 1 alt, 2 blink
      ts1 = on2;
      ts2 = on1;
      break;
    case 15: // 1 alt, 2 alt
      ts1 = off;
      ts2 = onFlop;
      break;
    default:
      ts1 = off;
      ts2 = off;
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
}

void HBOn2()
{
  digitalWrite(hb1, LOW);
  digitalWrite(hb2, HIGH);
}

void HBOff()
{
  digitalWrite(hb1, LOW);
  digitalWrite(hb2, LOW);
}

void HBFlop()
{
  static byte flop = 0;
  
  if (flop)
    HBOn1();
  else
    HBOn2();
  
  flop = !flop; //flip flop
}
