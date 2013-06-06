/* **************************************************
  *  MLCEEPROM.ino
  *   Part of the ModularLightControl sketch.
  *
  *  Author:  Ben Straub
  *  Revision: 1.0
  *  Date: 06/06/13
  *
  *  Description:  This handles the  reads and writes
  *     to EEPROM.
  *     
   ***************************************************/

const int settingAddr[6] = {0, 2, 4, 6, 8};

void saveSetting()
{
  EEPROM.write(settingAddr[menuNum], lowByte(*currentSetting));
  EEPROM.write(settingAddr[menuNum]+1, highByte(*currentSetting));
}

void loadSettings()
{
  ch1Mode = EEPROM.read(settingAddr[0]);
  ch2Mode = EEPROM.read(settingAddr[1]);
  brightness = word(
    EEPROM.read(settingAddr[2]+1), //high byte
    EEPROM.read(settingAddr[2]) ); //low byte
  onTime = word(
    EEPROM.read(settingAddr[3]+1), //high byte
    EEPROM.read(settingAddr[3]) ); //low byte
  offTime = word(
    EEPROM.read(settingAddr[4]+1), //high byte
    EEPROM.read(settingAddr[4]) ); //low byte
}

