/*
Pellergy Vaccume Control System
.
 */
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <EEPROM.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
#include "Timestuff.h"
#include "Menus.h"
#include "Global.h"
#include "functions.h"



//-------------------------------------------------------------------------------
// Program logic
//-------------------------------------------------------------------------------

void setup(void) {
  Serial.begin(9600);
  setSyncProvider(RTC.get);//Get Time from RTC
  // set up the TouchScreenMenu libary
  TSC.setBackColor(TSC.createColor(0, 0, 0)); // change the default background color
  TSC.init(); // make sure everything gets initialized
}

void loop(void) 
{
MainMenu(); //Draw Main Menu
MenuProcess(); //Process Actions on menu
RunVac();
}

//-------------------------------------------------------------------------------
// check various buttons and perform actions if any was pressed
//-------------------------------------------------------------------------------


