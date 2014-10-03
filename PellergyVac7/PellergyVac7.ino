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
#include <chrisMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>
//#include "TimeStuff.h"

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
  // create the various menus setting the items, font size, spacing, padding, justification and titles

  TouchScreenMenu MenuMain = TouchScreenMenu(mainItems, 2, 10, 10, CENTERJ, "Main Menu");
TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 10, CENTERJ, "Main Menu");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &MenuMain;

char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

  String h; //Hours
  String m; //Minutes
    String timeString =  "00:00";
int lmin;
void DrawTime(){
  
  if (lmin != minute(now())){
int    hs=hour(now());
   int ms=minute(now());
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
  char *TimeString = strdup(timeString.c_str()); //Convert String into char*

  Serial.print("time: ");
  Serial.println(TimeString);
 
if(curMenu == &MenuMain){
  Serial.println("drawTime");
        TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 200, 5, 10, true);
        lbl1.draw();
   
  }
       lmin=minute(now());
}}
void setup(void) {
  Serial.begin(57600);
  TSC.setBackColor(TSC.createColor(0, 100, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  setSyncProvider(RTC.get);//Get Time from RTC
  mainMenu.setClearScreenOnDraw(true);
  curMenu->draw(); // put up the main menu
  
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

// check various buttons and perform actions if any was pressed
void checkButtons(){

}
// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &MenuMain){

      if (item->getText() == "Menu"){

        Serial.println("Menu");
        curMenu=&mainMenu;
        TSC.clearScreen();

        curMenu->draw();
      }




      // if the menu item didn't get handled redraw it unpressed
      if(handled==false)
      Serial.println("curMenu->drawItem(item,false)");
    }
  }
}

