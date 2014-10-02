/*
This example shows the various abilities of the TouchScreenMenu library.
 See the readme.txt file for information on find the libraries this library uses.
 
 TouchScreenMenu Library. 
 
 2012 Copyright (c) Scott Shaver
 
 Authors: Scott Shaver
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
#include "TimeStuff.h"
#
int runtime=1000;
int curtime;
int startTime = 1300 ;
int stopTime = 1430 ;
int TestHour=0;
int TestMin=0;
int THL=0;
int TML=1;
String h; //Hours
String m; //Minutes
String timeString =  "00:00";
char *TimeString;
int lmin=255;
int ms;
int hs;
boolean debugEnable=false;
  
  

// create the array of items for the first sub menu


// create the array of items for the main menu
TouchScreenMenuItem mainItems[] = {
  TouchScreenMenuItem("Menu"),
  TouchScreenMenuItem("ENDOFMENU")
  };

TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Setup"),
  TouchScreenMenuItem("Tests"),
  TouchScreenMenuItem("About"),
  TouchScreenMenuItem("<- BACK"),
  TouchScreenMenuItem("ENDOFMENU")

  };
TouchScreenMenuItem SetupMenuItems[] = {
  TouchScreenMenuItem("Time"),
  TouchScreenMenuItem("Modes"),
  TouchScreenMenuItem("Start Time"),
  TouchScreenMenuItem("Stop Time"),
  TouchScreenMenuItem("<- BACK"),
  TouchScreenMenuItem("ENDOFMENU")
  };

TouchScreenArrowButton TimeMenuButtons[] = {
  TouchScreenArrowButton("HUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 30, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("HDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 30, TSC.getScreenHeight() - 120, 60, 30, DOWN),
  TouchScreenArrowButton("MUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 140, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("MDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 140, TSC.getScreenHeight() - 120, 60, 30, DOWN),
  TouchScreenArrowButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 20, TSC.getScreenHeight() - 40, 60, 30, LEFT),
  TouchScreenArrowButton("Save", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 130, TSC.getScreenHeight() - 40, 60, 30, RIGHT),
  TouchScreenArrowButton("ENDOFFORM")
  };
  // create the various menus setting the items, font size, spacing, padding, justification and titles
  TouchScreenLabel aboutLabels[] = {
    TouchScreenLabel("Vacume Control", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 35, 1, 2, true),
    TouchScreenLabel("Written By: Chris Yarger", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 50, 1, 2, true),
    TouchScreenLabel("ENDOFFORM")
    };
  TouchScreenButton aboutButtons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
    };

  TouchScreenMenu startScreen = TouchScreenMenu(mainItems, 2, 10, 10, CENTERJ, "VacControl");
TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 20, CENTERJ, "Menu");
TouchScreenMenu testMenu = TouchScreenMenu(mainMenuItems, 2, 10, 20, CENTERJ, "Tests");
TouchScreenMenu setupMenu = TouchScreenMenu(mainMenuItems, 2, 10, 20, CENTERJ, "Setup");
  TouchScreenForm aboutForm = TouchScreenForm("About", 2);

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &startScreen;
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

void DrawTime(){
  delay(10);
  if (debugEnable==true){
    Serial.print("DrawTime");
    Serial.print(lmin);
    Serial.print(":");
    Serial.print(ms);
    Serial.print(":");
    Serial.print(hs);
    Serial.print(":");
  }
  if (lmin != minute(now())){
    Serial.println("DrawTime+ExecuteDraw");
    hs=hour(now());
    ms=minute(now());
    if (hs >= 0 && hs < 10) {
      h=leadingZero[hs];
    }
    else{
      h=String(hs);
    }
    //Give Leading Zeros To Minutes

      if (ms >= 0 && ms < 10) {
      m=leadingZero[ms];
    }
    else{
      m=String(ms);
    }
    timeString=h+":"+m;
    TimeString = strdup(timeString.c_str()); //Convert String into char*

    Serial.print("time: ");
    Serial.println(TimeString);

    if(curMenu == &startScreen){
      Serial.println("drawTime");
      TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 200, 5, 10, true);
      lbl1.draw();

    }
    lmin=minute(now());

  }
  //Serial.println(timeString);

}
void drawMainScreen(){
  Serial.println("MAinScreen");
  //Serial.println(freeRam());  


  lmin=99;
//Serial.println(freeRam());  

  curMenu=&startScreen;
//Serial.println(freeRam());  

  //TSC.clearScreen();

  curMenu->draw();
//Serial.println(freeRam());  

}

void drawMainMenu(){
  Serial.println("Menu");
  curMenu=&mainMenu;
  curMenu->draw();
}
void drawAbout(){
  //curMenu=NULL;
  Serial.println("drawAboutScreen");
  
/*char versionStr[100];
  strcpy(versionStr,"Version: ");
  strcat(versionStr,VERSION);
*/
  aboutForm.draw();

  while(1){ // stay on this screen until the back button is hit
    TouchScreenArea *item = aboutForm.process(true);
    if(item!=NULL){
      if(!strcmp(item->getText(),"<- Back")){
        Serial.println("Return from About");
        drawMainMenu();
        return;
      }
    }
  }
}
void drawSetup(){
  Serial.println("drawSettings");
}
void drawSetTime(){
  Serial.println("drawSetTime");
}
void runVac(){
  Serial.println("runVac");
}
void drawStartTimeMenu(){
  Serial.println("drawStartTimeMenu");
}
void drawStopTimeMenu(){
  Serial.println("drawStopTimeMenu");
}
void drawModes(){
  Serial.println("drawModes");
}
void drawTests(){
  Serial.println("DrawTests");
}
void drawRun(){
  Serial.println("DrawRun");
}
void drawAlarm(){
  Serial.println("DrawAlarm Mode Screen");
}
void Alarm(){
  Serial.println("Alarm");
}
void drawEnabled(){
  Serial.println("DrawEnabled Screen");
}
void checkSensors(){
  Serial.println("CheckSensors");
}
void status(){
  Serial.println("Status Screen");
}

// check various buttons and perform actions if any was pressed
void checkButtons(){
  Serial.println("CheckButtons");
}

// check to see if any menu item was pressed and do something

void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &startScreen){

      if (item->getText() == "Menu"){
        drawMainMenu();
      }
    }//end if StartScreen
    if(curMenu == &mainMenu){
      Serial.println("inMain Menu");
      if (item->getText() == "About"){
        //Serial.println("About");
        drawAbout();
      }
      else if (item->getText() == "Tests"){
        Serial.println("to Tests Screen");
        drawTests();
      }

      else if (item->getText() == "Setup"){
        Serial.println("to Setup Screen");
        drawSetup();
      }

      else if (item->getText() == "<- BACK"){
        Serial.println("Back to Main Screen");
        drawMainScreen();
      }
      // if the menu item didn't get handled redraw it unpressed
    }//end If mainMenu
    
  }//end if Not Null
}//end checkMenuSelection





void setup(void) {
  Serial.begin(9600);
  Serial.println("start Setup");
  //Serial.println(freeRam());
  TSC.setBackColor(TSC.createColor(0, 100, 0)); // change the default background color
  //Serial.println(freeRam());
  TSC.init(); // make sure everything get initialized
  //Serial.println(freeRam());
  setSyncProvider(RTC.get);//Get Time from RTC
  //Serial.println(freeRam());
  mainMenu.setClearScreenOnDraw(true);
  //  StartScreen.setClearScreenOnDraw(true);
  //aboutForm.setClearScreenOnDraw(true);
  //Serial.println(freeRam());
  aboutForm.setLabels(aboutLabels);
  //Serial.println(freeRam());
  aboutForm.setButtons(aboutButtons);
  //Serial.println(freeRam());
  //curMenu=&startScreen;
drawMainScreen();
//Serial.println(freeRam());  
  Serial.println("<Setup complete");
}

void loop(void) {
  // handle the current menu
//Serial.println(freeRam());  
  
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
    DrawTime();
//Serial.println(freeRam());  

  }
  else{
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
//Serial.println(freeRam());  

  }
}


