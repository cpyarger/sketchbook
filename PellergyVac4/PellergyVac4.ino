
/*
Pellergy Vaccume Control System
 */
 
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <EEPROM.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
//Title, And Version information
#define VERSION "0.1"
#define TITLE "Vacuum Control System"

int lmin; //Last minute variable to determine main screen refresh
int runtime=1000;
int curtime;
int startTime = 1300 ;
int stopTime = 1430 ;
int TestHour=0;
int TestMin=0;
int THL=0;
int TML=1;

char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

TouchScreenArea lbl1 = TouchScreenLabel("Big label.", TSC.createColor(0, 200, 0), TSC.createColor(255, 0, 255), 0, 120, 3, 2, true);
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Setup"),
  TouchScreenMenuItem("Tests"),
  TouchScreenMenuItem("About"),
  TouchScreenMenuItem("ENDOFMENU")

  };

  // create the array of items for the setup timing menu
TouchScreenMenuItem SetupMenuItems[] = {
  TouchScreenMenuItem("Set Time"),
  TouchScreenMenuItem("Set Run Times"),
  //TouchScreenMenuItem("Set "),
  //TouchScreenMenuItem("Setup"),
  TouchScreenMenuItem("ENDOFMENU")
  };

  TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 20, CENTERJ, TITLE);

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

TouchScreenButton backFromCurrentSetup = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
//TouchScreenButton backFromAbout = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);

TouchScreenForm TimeMenu1 = TouchScreenForm("Set Time", 2);



TouchScreenLabel TimeMenuLabels[] = {
  TouchScreenLabel("Hour", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 20, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("Minute", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 110, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("ENDOFFORM")
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
  
void MainMenu(){
  // display the main menu
  mainMenu.setTitleFontSize(1);
  mainMenu.setClearScreenOnDraw(true);
  if(minute() != lmin){ 
    curMenu->draw(); // put up the main menu
    DrawTime();
  }
  lmin=minute();
}

//-------------------------------------------------------------------------------
// display the about screen
//-------------------------------------------------------------------------------
void displayAbout()
{
  TouchScreenForm form = TouchScreenForm("About", 2);
  form.setClearScreenOnDraw(true);
  char versionStr[100];
  strcpy(versionStr,"Version: ");
  strcat(versionStr,VERSION);

  TouchScreenLabel labels[] = {
    TouchScreenLabel(TITLE, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 35, 1, 2, true),
    TouchScreenLabel("Written By: Chris Yarger", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 50, 1, 2, true),
    TouchScreenLabel(versionStr, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 65, 1, 2, true),
    TouchScreenLabel("ENDOFFORM")
    };
  TouchScreenButton buttons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
    };
    form.setLabels(labels);
  form.setButtons(buttons);
  form.draw();

  while(1){ // stay on this screen until the back button is hit
    TouchScreenArea *item = form.process(true);
    if(item!=NULL){
      if(!strcmp(item->getText(),"<- Back")){
        return;
      }
    }
  }
}

//-------------------------------------------------------------------------------
// display the current settings
//-------------------------------------------------------------------------------
void displaySetup()
{

   TouchScreenMenu setupMenu = TouchScreenMenu(SetupMenuItems, 2, 10, 20, CENTERJ, TITLE);

  setupMenu.setClearScreenOnDraw(true);


  TouchScreenButton buttons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
    };


    


    
  
}

//-------------------------------------------------------------------------------
// display the current Tests
//-------------------------------------------------------------------------------
void displayTests()
{
  TouchScreenForm form = TouchScreenForm("Tests", 2);
  form.setClearScreenOnDraw(true);


  TouchScreenButton buttons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
    };


    form.setButtons(buttons);
  form.draw();

  while(1){ // stay on this screen until the back button is hit
    TouchScreenArea *item = form.process(true);
    if(item!=NULL){
      if(!strcmp(item->getText(),"<- Back")){
        return;
      }
    }
  }
}



void checkButtons()
{
  if(backFromCurrentSetup.process())
    curMenu = &mainMenu;
  //DrawTime();
  /*else if(backFromAbout.process())
   curMenu = &mainMenu;*/

  if(curMenu != NULL)
    curMenu->draw();
}


//-------------------------------------------------------------------------------
// check to see if any menu item was pressed and do something
//-------------------------------------------------------------------------------
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"Tests"))
      {
        displayTests();
        //TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Setup"))
      {
        displaySetup();
        //TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"About"))
      {
        displayAbout();
        //TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
      curMenu->drawItem(item,false);
  }
}

void MenuProcess(){
  if(curMenu!=NULL)
  {
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  }
  else
  {
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
  }
}




int checktime(){
curtime = hour(now()) * 60 + minute(now()) ; //gives exact minute of current time
  if (curtime > startTime && curtime < stopTime ) {
   Serial.print("True:");
    return 1;    
  }
  else {
   Serial.print("False:");
    return 0;
  }
}

void RunVac(){
int ctime1=checktime();
int curtime1 = hour(now()) * 60 + minute(now()) ;
Serial.print(curtime1);
Serial.print(":" );
Serial.print(hour(now()));
Serial.print(":" );
Serial.print(minute(now()));
Serial.print(":" );
Serial.print(curtime );
Serial.print(":" );
Serial.print(ctime1);
Serial.print(":" );
Serial.print(startTime);
Serial.print(":" );
Serial.print(stopTime);
Serial.print(":" );
if ( ctime1 == 1 ){
   Serial.println("True");
   digitalWrite(13, HIGH);
   delay(runtime);

  }
  else{
  Serial.println("False");  
  digitalWrite(13, LOW);
  delay(100);
  
  }
  


}
  void DrawTestTime(int h, int v){
    char *s1;//Character array that date strings are dumped into 
    String s2=String(TestHour)+":"+String(TestMin); //Time format string
    s1 = strdup(s2.c_str()); //Convert String into char*
    Tft.drawString(s1,h,v,3,BLUE);
  }
// keep track of which form is the currently active one



void redraw(){
  TSC.clearScreen();
  curMenu->draw();
}

void DrawTime(){
  String h; //Hours
  String m; //Minutes
//Give Leading Zero's to Hours  
  if (hour() >= 0 && hour() < 10) {
    h=leadingZero[hour()];
  }
  else{
    h=String(hour());
  }
  //Give Leading Zeros To Minutes
    if (minute() >= 0 && minute() < 10) {
    m=leadingZero[minute()];
  }
  else{
    m=String(minute());
  }
  char *s1;//Character array that date strings are dumped into 
  String s2=h+":"+m; //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char* 
  Tft.drawString(s1,60,260,3,BLUE); //Draw Time
}






void setup(){
 Serial.begin(9600);
  setSyncProvider(RTC.get);//Get Time from RTC
  // set up the TouchScreenMenu libary
  TestHour=EEPROM.read(THL);
  //read value for startTime minute
  TestMin=EEPROM.read(TML);
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
