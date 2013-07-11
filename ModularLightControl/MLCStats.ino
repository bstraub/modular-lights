/* **************************************************
  *  MLCScreen.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 1.0.
  *  Date: 06/06/13
  *
  *  Description:  This handles all  the stat  reading
  *     stuff, including making the screen update call
  *     if the statistics menu is open.
  *
   ***************************************************/

#define RATIO_5V 0.692724212398374
#define RATIO_12V 1.3240892059949

#define STAT_REFRESH_TIME 500 //in ms


void initStats()
{
  analogReference(EXTERNAL);
}

void readStat5V()
{
  int temp = analogRead(stat5V);
  v5 = (int)(((double)temp)*RATIO_5V);
}

void readStat12V()
{
  int temp = analogRead(stat12V);
  v12 = (int)(((double)temp)*RATIO_12V);
}

void readStatI1()
{
  int temp = analogRead(statI1);
  iCh1 = temp;  // !fix
}

void readStatI2()
{
  int temp = analogRead(statI2);
  iCh2 = temp; // !fix
}

void readStat()
{
  static long displayCount = 0;

  if (checkValidPriority( priority & 0x3 ))
  {
    readStatCall( priority & 0x3 );
    priority = ( ( (priority & 0x3) << 6 ) | ( priority >> 2 ) );
  } else if (checkValidPriority( (priority >> 2) & 0x3 ))
  {
    readStatCall( (priority >> 2) & 0x3 );
    priority = ( ( (priority & 0xC) << 4 ) | ( (priority & 0xF0) >> 2 ) | (priority & 0x3) );
  } else if (checkValidPriority( (priority >> 4) & 0x3 ))
  {
    readStatCall( (priority >> 4) & 0x3 );
    priority = ( ( (priority & 0x30) << 2 ) | ( (priority & 0xC0) >> 2 ) | (priority & 0xF) );
  } else if (checkValidPriority( (priority >> 6) & 0x3 ))
  {
    readStatCall( (priority >> 6) & 0x3 );
    //no change in priority
  }
  
  if (menuContext == 2)
  {
    if ((millis()-displayCount)>STAT_REFRESH_TIME)
    {
      updateScreen();
      displayCount=millis();
    }
  }
    
}

void readStatCall( byte pri )
{
  if (pri == 0)
    readStat5V();
  else if (pri == 1)
    readStat12V();
  else if (pri == 2)
    readStatI1();
  else if (pri == 3)
    readStatI2();
}
  

boolean checkValidPriority( byte pri ) 
{
  if ((pri & 2) == 0) //voltage reading
    return true; 
  if (pri == 2) // i1
  { //make sure HB is in the right state and it's been long enough since the last switch
    if ((hbState == 1) && ((micros()-changeTime) >= VALID_READ_TIME) && (checkEnSwitch()))
      return true;
    else
      return false;
  }
  if (pri == 3) // i2
  { //make sure HB is in the right state and it's been long enough since the last switch
    if ((hbState == 2) && ((micros()-changeTime) >= VALID_READ_TIME) && (checkEnSwitch()))
      return true;
    else
      return false;
  }
  
  return false; //shouldn't get here, but just in case
}
    
boolean checkEnSwitch()
{
  if (TCNT1 > (256-OCR1A)) // then it's disabled
    return false;
  if (TCNT1 < VALID_READ_COUNTS) //hasn't been enabled long enough
    return false;
  if ( ((256-OCR1A)-TCNT1) < 4 ) //is it about to turn off?
    return false;
  return true;
}
