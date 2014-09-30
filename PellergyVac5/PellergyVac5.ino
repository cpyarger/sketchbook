/*
 */
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
#include <EEPROM.h>
#include "timestuff.h"
//#include "Menus.h"
//Global Variables
int StartHour=0;
int StartMin=0;
int THL=0;
int TML=1;
TouchScreenForm formMain = TouchScreenForm("Vacume Control", 2);
TouchScreenForm formMenu = TouchScreenForm("Menu", 2);
TouchScreenForm formSettings = TouchScreenForm("Settings", 2);
TouchScreenForm formTests = TouchScreenForm("Tests", 2);
TouchScreenForm formSetTime = TouchScreenForm("Set Time", 2);
TouchScreenForm formStartTime = TouchScreenForm("Start Time", 2);
TouchScreenForm formStopTime = TouchScreenForm("Stop Time", 2);
TouchScreenForm formModes = TouchScreenForm("Modes", 2);
TouchScreenForm formxxx = TouchScreenForm("Set Time", 2);

TouchScreenArrowButton TimeMenuButtons[] = {
  TouchScreenArrowButton("HUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 30, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("HDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 30, TSC.getScreenHeight() - 120, 60, 30, DOWN),
  TouchScreenArrowButton("MUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 140, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("MDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 140, TSC.getScreenHeight() - 120, 60, 30, DOWN),
  TouchScreenArrowButton("ENDOFFORM")
  };
TouchScreenButton TimeButtons[] = {
  TouchScreenButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 20, TSC.getScreenHeight() - 40, 60, 30),
  TouchScreenButton("Save", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 130, TSC.getScreenHeight() - 40, 60, 30),
  TouchScreenButton("ENDOFFORM")
  };
  TouchScreenButton SetupMenuButton[] = {
   TouchScreenButton("Base Time", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 200, 2, 10),
  TouchScreenButton("Start Time", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 275, 2, 10),
  TouchScreenButton("Stop Time", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 250, 2, 10),
  TouchScreenButton("Set Mode", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 150, 2, 10),
  };
TouchScreenButton MenuButton[] = {
  TouchScreenButton("Setup", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 150, 2, 10),
  TouchScreenButton("Tests", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 100, 2, 10),
  TouchScreenButton("<- Home", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 10, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
  };

TouchScreenButton MainButton[] = {
  TouchScreenButton("Menu", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
  };
TouchScreenLabel TimeLabels[] = {
  TouchScreenLabel("Hour", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 20, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("Minute", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 110, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("ENDOFFORM")
  };
  TouchScreenForm *curForm = &formMain;

void setup(void) {
  Serial.begin(57600);
  setSyncProvider(RTC.get);//Get Time from RTC

  StartHour=EEPROM.read(THL);
  //read value for startTime minute
  StartMin=EEPROM.read(TML);
formMain.setClearScreenOnDraw(true);
formMenu.setClearScreenOnDraw(true);
formSettings.setClearScreenOnDraw(true);
formTests.setClearScreenOnDraw(true);
formSetTime.setClearScreenOnDraw(true);
formStartTime.setClearScreenOnDraw(true);
formStopTime.setClearScreenOnDraw(true);
formModes.setClearScreenOnDraw(true);
  formMain.setButtons(MainButton);
  formMenu.setButtons(MenuButton);
  formSettings.setButtons(SetupMenuButton);
  formStartTime.setLabels(TimeLabels);
  formStartTime.setArrowButtons(TimeMenuButtons);
  TSC.setBackColor(TSC.createColor(0, 20, 0));   
  TSC.init(); // make sure everything get initialized
curForm->draw();
  // change the default background color
Serial.println("Setup Complete");
}

void drawStartTime(){
Serial.println("DrawStartTime");
  TSC.clearScreen();
  curForm->draw();

  char *s1;//Character array that date strings are dumped into 
  String s2=String(StartHour)+":"+String(StartMin); //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char*
  Tft.drawString(s1,10,80,3,WHITE);
}
void redraw(){
  TSC.clearScreen();
  curForm->draw();
  DrawTime();

}

void loop(void) {

  

  TouchScreenArea *item = curForm->process(true);
  if (item->getText() == "Menu"){
    Serial.println("Menu");
    curForm=&formMenu;
    TSC.clearScreen();
    curForm->draw();
  }
  
  else if (item->getText() == "Start Time"){
    Serial.println("Start Time");
    curForm=&formStartTime;
    TSC.clearScreen();
    curForm->draw();
  }
  else if (item->getText() == "<- Home"){
    Serial.println("Home");
    curForm=&formMain;
    TSC.clearScreen();
    curForm->draw();
  }




  StartHour= constrain(StartHour, 0, 24);
  StartMin = constrain(StartMin, 0, 59);
}



