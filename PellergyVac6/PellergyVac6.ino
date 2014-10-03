#include <chrisMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
#include "TimeStuff.h"

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
int p1=3;
int p2=5;
int p3=6;
int p4=9;
boolean debugEnable=false;

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

TouchScreenMenuItem setupMenuItems[] = {
  TouchScreenMenuItem("Time"),
  TouchScreenMenuItem("Modes"),
  TouchScreenMenuItem("Start Time"),
  //TouchScreenMenuItem("Stop Time"),
  TouchScreenMenuItem("<- BACK"),
  TouchScreenMenuItem("ENDOFMENU")
  };
  
TouchScreenMenuItem testMenuItems[] = {
  TouchScreenMenuItem("Alarm"),
  TouchScreenMenuItem("start/stop"),
  //TouchScreenMenuItem("Start Time"),
  //TouchScreenMenuItem("Stop Time"),
  TouchScreenMenuItem("<- BACK"),
  TouchScreenMenuItem("ENDOFMENU")
  };
  
TouchScreenButton TimeMenuButtons[] = {
  TouchScreenButton("HUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 30, TSC.getScreenHeight() - 180 , 60, 30),
  TouchScreenButton("HDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 30, TSC.getScreenHeight() - 120, 60, 30),
  TouchScreenButton("MUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 140, TSC.getScreenHeight() - 180 , 60, 30),
  TouchScreenButton("MDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 140, TSC.getScreenHeight() - 120, 60, 30),
  TouchScreenButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 20, TSC.getScreenHeight() - 40, 60, 30),
  TouchScreenButton("Save", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 130, TSC.getScreenHeight() - 40, 60, 30),
  TouchScreenButton("ENDOFFORM")
  };

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
TouchScreenMenu testMenu = TouchScreenMenu(testMenuItems, 2, 10, 20, CENTERJ, "Tests");
TouchScreenMenu setupMenu = TouchScreenMenu(setupMenuItems, 2, 10, 20, CENTERJ, "Setup");
TouchScreenForm aboutForm = TouchScreenForm("About", 2);

TouchScreenMenu *curMenu = &startScreen;

char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

void DrawTime(){
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
    Serial.println(timeString);
    if(curMenu == &startScreen){
      Serial.println("drawTime");
      TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 200, 5, 10, true);
      lbl1.draw();
    }
    lmin=minute(now());
  }
}

void drawMainScreen(){
  Serial.println("MAinScreen");
  lmin=99;
  curMenu=&startScreen;
  curMenu->draw();
}

void drawMainMenu(){
  Serial.println("Menu");
  curMenu=&mainMenu;
  curMenu->draw();
}

void drawTests(){
  Serial.println("DrawTests");
}

void drawSetup(){
  Serial.println("drawSettings");
  curMenu=&setupMenu;
  curMenu->draw();
  
}

void checkButtons(){
  Serial.println("CheckButtons");
}


void drawAbout(){
  Serial.println("drawAboutScreen");

  aboutForm.draw();

  //while(1){ // stay on this screen until the back button is hit
  TouchScreenArea *item = aboutForm.process(true);
  if(item!=NULL){
    if(!strcmp(item->getText(),"<- Back")){
      Serial.println("Return from About");
      drawMainMenu();
      return;
      //  }
    }
  }
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
      Serial.println("Main Menu");
      if (item->getText() == "About"){
        Serial.println("About");
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
  TSC.setBackColor(TSC.createColor(0, 100, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  setSyncProvider(RTC.get);//Get Time from RTC
  mainMenu.setClearScreenOnDraw(true);
  startScreen.setClearScreenOnDraw(true);
  
  setupMenu.setClearScreenOnDraw(true);
  aboutForm.setClearScreenOnDraw(true);
  //setupMenu.setLabels(aboutLabels);
  aboutForm.setLabels(aboutLabels);
  aboutForm.setButtons(aboutButtons);
  //curMenu=&startScreen;
  drawMainScreen(); 
  Serial.println("Setup complete");
}

void loop(void) {
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
    DrawTime();
  }
  else{
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
  }
}



