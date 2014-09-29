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
void redraw(){
  TSC.clearScreen();
  curMenu->draw();
  

  //DrawTime();

}
void setup(void) {
  Serial.begin(9600);
  setSyncProvider(RTC.get);//Get Time from RTC
  // set up the TouchScreenMenu libary
  TestHour=EEPROM.read(THL);
  //read value for startTime minute
  TestMin=EEPROM.read(TML);
  TimeMenu1.setArrowButtons(TimeMenuButtons);
  TimeMenu1.setLabels(TimeMenuLabels);
  TSC.setBackColor(TSC.createColor(0, 0, 0)); // change the default background color
  TSC.init(); // make sure everything gets initialized
  TouchScreenMenu *curMenu = &mainMenu;

redraw();
}


void loop(void) 
{
//MainMenu(); //Draw Main Menu
MenuProcess(); //Process Actions on menu
RunVac();
TestHour= constrain(TestHour, 0, 24);
TestMin = constrain(TestMin, 0, 59);



String s12=String(TestHour)+":"+String(TestMin); //Time format string
Serial.println(s12);
}

//-------------------------------------------------------------------------------
// check various buttons and perform actions if any was pressed
//-------------------------------------------------------------------------------


