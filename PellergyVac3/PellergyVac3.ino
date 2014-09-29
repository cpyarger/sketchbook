/*
This example shows the various abilities of the TouchScreenMenu library.

In this example there are two different TouchScreenForm objects which contain
other objects.  Using TouchScreenForm you don't have to keep track of what
buttons or other controls should be handled.

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
#include <EEPROM.h>

int TestHour=0;
int TestMin=0;
int THL=0;
int TML=1;
TouchScreenForm form2 = TouchScreenForm("Set Time", 2);



TouchScreenLabel labels2[] = {
  TouchScreenLabel("Hour", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 20, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("Minute", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 110, TSC.getScreenHeight() - 145, 2, 2, true),
  TouchScreenLabel("ENDOFFORM")
};



TouchScreenArrowButton arrowButtons[] = {
  TouchScreenArrowButton("HUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 30, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("HDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 30, TSC.getScreenHeight() - 120, 60, 30, DOWN),
   TouchScreenArrowButton("MUp", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 140, TSC.getScreenHeight() - 180 , 60, 30, UP),
  TouchScreenArrowButton("MDown", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 140, TSC.getScreenHeight() - 120, 60, 30, DOWN),
  TouchScreenArrowButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 20, TSC.getScreenHeight() - 40, 60, 30, LEFT),
  TouchScreenArrowButton("Save", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 130, TSC.getScreenHeight() - 40, 60, 30, RIGHT),
  TouchScreenArrowButton("ENDOFFORM")
};


void DrawTestTime(int h, int v){
    char *s1;//Character array that date strings are dumped into 
  String s2=String(TestHour)+":"+String(TestMin); //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char*
  Tft.drawString(s1,h,v,3,BLUE);
}
// keep track of which form is the currently active one
TouchScreenForm *curForm = &form2;

void redraw(){
  TSC.clearScreen();
  curForm->draw();
  DrawTestTime(10,80);

        //DrawTime();

}


void setup(void) {
  Serial.begin(57000);
   setSyncProvider(RTC.get);//Get Time from RTC
//read value for startTime hour
TestHour=EEPROM.read(THL);
//read value for startTime minute
TestMin=EEPROM.read(TML);

  form2.setArrowButtons(arrowButtons);
  form2.setLabels(labels2);


  TSC.setBackColor(TSC.createColor(0, 0, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  redraw(); // put up the current form
}

void loop() {
  // handle the current form
  if(curForm!=NULL){
    // process the current menu
    TouchScreenArea *item = curForm->process(false);

    if(item!=NULL){
      if(!strcmp(item->getText(),"MUp")){
        TestMin++;
        redraw();


          //Serial.print("Minute Up: ");
          //Serial.println(TestMin);
        }
        else if(!strcmp(item->getText(),"MDown")){
          TestMin--;
          redraw();
          //Serial.print("Minute Down: ");
          //Serial.println(TestMin); 
      }
        else if(!strcmp(item->getText(),"HUp")){
          TestHour++;
          redraw();
          //Serial.print("Hour up: ");
      //Serial.println(TestHour);  
      }
        else if(!strcmp(item->getText(),"HDown")){
        TestHour--;
        redraw();
          //Serial.print("Hour Down: ");
          //Serial.println(TestHour); 
        }
        else if(!strcmp(item->getText(),"Save")){
          EEPROM.write(THL,TestHour);
          Serial.print("Wrote TestHour with Value: ");
          Serial.println(TestHour);
          EEPROM.write(TML,TestMin);
          Serial.print("Wrote TestMin with Value: ");
          Serial.println(TestMin);
          
        
        redraw();
          //Serial.print("Hour Down: ");
          //Serial.println(TestHour); 
        }
      }

    }
TestHour= constrain(TestHour, 0, 24);
TestMin = constrain(TestMin, 0, 59);



String s12=String(TestHour)+":"+String(TestMin); //Time format string
Serial.println(s12);


}


