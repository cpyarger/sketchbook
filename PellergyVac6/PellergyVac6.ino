<<<<<<< HEAD
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
#include <EEPROM.h>
=======
#include <chrisMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
>>>>>>> d370890d54998733991f67489e372c59450597bf
//#include "TimeStuff.h"

int runtime=1000;
int curtime;
int startTime = 1300 ;
int stopTime = 1430 ;
<<<<<<< HEAD
#define DS1307_CTRL_ID 0x68 


=======
int TestHour=0;
int TestMin=0;
int THL=0;
int TML=1;
>>>>>>> d370890d54998733991f67489e372c59450597bf
String h; //Hours
String m; //Minutes
String timeString =  "00:00";
char *TimeString;
int lmin=255;
int ms;
int hs;
<<<<<<< HEAD
int StartMin = 0;
int StopMin=0;
int StartHour=0;
int StopHour=0;
int StartMinEEPROM = 1;
int StartHourEEPROM = 2;
int StopMinEEPROM = 3;
int StopHourEEPROM = 4;
int runTimeEEPROM = 5;
int alarmp = 9;

TouchScreenButton TimeMenuButtons[] = {
  TouchScreenButton("HUp",   TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 10, TSC.getScreenHeight() - 180 , 2, 10),
  TouchScreenButton("HDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 10, TSC.getScreenHeight() - 120,  2, 10),
  TouchScreenButton("MUp",   TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 120, TSC.getScreenHeight() - 180 , 2, 10),
  TouchScreenButton("MDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 120, TSC.getScreenHeight() - 120,  2, 10),
  TouchScreenButton("Back",  TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0),   10, TSC.getScreenHeight() - 40,   2, 10),
  TouchScreenButton("Save",  TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0),   120, TSC.getScreenHeight() - 40,  2, 10),
  TouchScreenButton("ENDOFFORM")
  };


TouchScreenButton setupButtons[] = {
  TouchScreenButton("Time", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 290, 2, 10),
  TouchScreenButton("Start", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 240, 2, 10),
  TouchScreenButton("Stop", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 190, 2, 10),
  TouchScreenButton("Mode", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 140, 2, 10),
  TouchScreenButton("Run", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 90, 2, 10),
  TouchScreenButton("Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 40, 2, 10),
  TouchScreenButton("ENDOFFORM")
  };


TouchScreenButton menuButtons[] = {
  TouchScreenButton("Tests", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 200, 2, 10),
  TouchScreenButton("Setup", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 150, 2, 10),
  TouchScreenButton("Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
  };

TouchScreenButton startButtons[] = {
  TouchScreenButton("Menu", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 70, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
  };

  TouchScreenForm startScreen = TouchScreenForm("VacControl",2);
TouchScreenForm mainMenu = TouchScreenForm("Menu",2);
TouchScreenForm testMenu = TouchScreenForm("Tests",2);
TouchScreenForm startTimeMenu = TouchScreenForm("Start Time",2);

TouchScreenForm setupMenu = TouchScreenForm("Setup",2);
=======
int p1=3;
int p2=5;
int p3=6;
int p4=9;


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

TouchScreenForm startScreen = TouchScreenForm("VacControl",2);
TouchScreenForm mainMenu = TouchScreenForm("Menu",2);
TouchScreenForm testMenu = TouchScreenForm("Tests",2);
TouchScreenForm setupMenu = TouchScreenForm("Setup",2);
TouchScreenForm aboutForm = TouchScreenForm("About", 2);
>>>>>>> d370890d54998733991f67489e372c59450597bf

TouchScreenForm *curMenu = &startScreen;

char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros




void drawSetTime(){
  Serial.println("drawSetTime");
}

void runVac(){
  Serial.println("runVac");
}

void drawStartTimeMenu(){
<<<<<<< HEAD
  cs();
  startTimeMenu.draw();
  Serial.println("drawStartTimeMenu");
  int TestHour = constrain(TestHour, 0, 24);
  int TestMin = constrain(TestMin, 0, 59);


    // process the current menu
    TouchScreenArea *item = curMenu->process(false);

    if(item!=NULL){
      if(!strcmp(item->getText(),"MUp")){
        TestMin++;

        Serial.print("Minute Up: ");
        Serial.println(TestMin);
      }
      else if(!strcmp(item->getText(),"MDown")){
        TestMin--;

        Serial.print("Minute Down: ");
        Serial.println(TestMin); 
      }
      else if(!strcmp(item->getText(),"HUp")){
        TestHour++;

        //Serial.print("Hour up: ");
        //Serial.println(TestHour);  
      }
      else if(!strcmp(item->getText(),"HDown")){
        TestHour--;

        //Serial.print("Hour Down: ");
        //Serial.println(TestHour); 
      }
      else if(!strcmp(item->getText(),"Save")){
        EEPROM.write(StartHourEEPROM,TestHour);
        Serial.print("Wrote TestHour with Value: ");
        Serial.println(TestHour);
        EEPROM.write(StartMinEEPROM,TestMin);
        Serial.print("Wrote TestMin with Value: ");
        Serial.println(TestMin);


        //Serial.print("Hour Down: ");
        //Serial.println(TestHour); 
      }

    
  }
  Serial.println("Draw Start Time");
  char *s1;//Character array that date strings are dumped into 
  String s2=String(TestHour)+":"+String(TestMin); //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char*
  //Tft.drawString(s1,80,30,3,BLUE);
  TouchScreenArea lbl1 = TouchScreenLabel(s1, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 50, 5, 10, true);

  lbl1.draw();
=======
  Serial.println("drawStartTimeMenu");
>>>>>>> d370890d54998733991f67489e372c59450597bf
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

<<<<<<< HEAD
=======
void drawAlarm(){
  Serial.println("DrawAlarm Mode Screen");
}

>>>>>>> d370890d54998733991f67489e372c59450597bf
void Alarm(){
  Serial.println("Alarm");
}

void drawEnabled(){
  Serial.println("DrawEnabled Screen");
}

void checkSensors(){
  Serial.println("CheckSensors");
}

<<<<<<< HEAD

=======
void status(){
  Serial.println("Status Screen");
}
>>>>>>> d370890d54998733991f67489e372c59450597bf

// check various buttons and perform actions if any was pressed
void checkButtons(){
  Serial.println("CheckButtons");
}
<<<<<<< HEAD
void cs(){

  TSC.clearScreen();


}
void DrawTime(){

=======

void DrawTime(){
>>>>>>> d370890d54998733991f67489e372c59450597bf
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
<<<<<<< HEAD
    if (ms >= 0 && ms < 10) {
=======
      if (ms >= 0 && ms < 10) {
>>>>>>> d370890d54998733991f67489e372c59450597bf
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
<<<<<<< HEAD
      TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 50, 5, 10, true);

=======
      TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 200, 5, 10, true);
>>>>>>> d370890d54998733991f67489e372c59450597bf
      lbl1.draw();
    }
    lmin=minute(now());
  }
}

void drawMainScreen(){
  Serial.println("MAinScreen");
  lmin=99;
<<<<<<< HEAD
  cs();
  curMenu=&startScreen;
  curMenu->draw();
  digitalWrite(alarmp, HIGH);  
}

void drawMainMenu(){
  cs();
  digitalWrite(alarmp, LOW);
=======
  curMenu=&startScreen;
  curMenu->draw();
}

void drawMainMenu(){
>>>>>>> d370890d54998733991f67489e372c59450597bf
  Serial.println("Menu");
  curMenu=&mainMenu;
  curMenu->draw();
}



void drawSetup(){
<<<<<<< HEAD
  cs();
  Serial.println("drawSettings");
  curMenu=&setupMenu;
  curMenu->draw();

=======
  Serial.println("drawSettings");
  curMenu=&setupMenu;
  curMenu->draw();
  
>>>>>>> d370890d54998733991f67489e372c59450597bf
}




<<<<<<< HEAD
=======
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
>>>>>>> d370890d54998733991f67489e372c59450597bf

// check to see if any menu item was pressed and do something

void checkMenuSelection(TouchScreenArea *item) {
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
<<<<<<< HEAD

      if (item->getText() == "Tests"){
=======
      if (item->getText() == "About"){
        Serial.println("About");
        drawAbout();
      }
      else if (item->getText() == "Tests"){
>>>>>>> d370890d54998733991f67489e372c59450597bf
        Serial.println("to Tests Screen");
        drawTests();
      }

      else if (item->getText() == "Setup"){
        Serial.println("to Setup Screen");
        drawSetup();
      }
<<<<<<< HEAD

      else if (item->getText() == "Back"){
=======
      
      else if (item->getText() == "<- BACK"){
>>>>>>> d370890d54998733991f67489e372c59450597bf
        Serial.println("Back to Main Screen");
        drawMainScreen();
      }
      // if the menu item didn't get handled redraw it unpressed
    }//end If mainMenu
<<<<<<< HEAD
    if(curMenu == &setupMenu){
      if (item->getText() == "Time"){
        Serial.println("Back to Main Screen");
        drawMainMenu();
      }
      if (item->getText() == "Start"){
        Serial.println(" drawStartTimeMenu");
        drawStartTimeMenu();
      }
      if (item->getText() == "Stop"){
        Serial.println("Back to Main Screen");
        drawMainMenu();
      }
      if (item->getText() == "Mode"){
        Serial.println("Back to Main Screen");
        drawMainMenu();
      }
      if (item->getText() == "Run"){
        Serial.println("Back to Main Screen");
        drawMainMenu();
      }
      if (item->getText() == "Back"){
        Serial.println("Back to Main Screen");
        drawMainMenu();
      }
    }
=======

>>>>>>> d370890d54998733991f67489e372c59450597bf
  }//end if Not Null
}//end checkMenuSelection





void setup(void) {
  Serial.begin(9600);
<<<<<<< HEAD
    Wire.begin();
=======
>>>>>>> d370890d54998733991f67489e372c59450597bf
  Serial.println("start Setup");
  TSC.setBackColor(TSC.createColor(0, 100, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  setSyncProvider(RTC.get);//Get Time from RTC
  //setupMenu.setLabels(aboutLabels);
<<<<<<< HEAD
  pinMode(alarmp, OUTPUT);
  startScreen.setButtons(startButtons);
  mainMenu.setButtons(menuButtons);
  startTimeMenu.setButtons(TimeMenuButtons);

  setupMenu.setButtons(setupButtons);

=======
  aboutForm.setLabels(aboutLabels);
  aboutForm.setButtons(aboutButtons);
>>>>>>> d370890d54998733991f67489e372c59450597bf
  //curMenu=&startScreen;
  drawMainScreen(); 
  Serial.println("Setup complete");
}

void loop(void) {
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenArea *item = curMenu->process(false);
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



<<<<<<< HEAD


=======
>>>>>>> d370890d54998733991f67489e372c59450597bf
