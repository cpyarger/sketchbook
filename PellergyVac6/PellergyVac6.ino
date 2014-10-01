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

// create the array of items for the first sub menu


// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Menu"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the various menus setting the items, font size, spacing, padding, justification and titles

TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 10, CENTERJ, "Main Menu");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;


void setup(void) {
  TSC.setBackColor(TSC.createColor(0, 255, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized

  curMenu->draw(); // put up the main menu
}

void loop(void) {
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
        TouchScreenArea lbl1 = TouchScreenLabel("This is a label with bg.", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 45, 10, 1, 2, true);
        lbl1.draw();
  }else{
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
    if(curMenu == &mainMenu){




      
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
        curMenu->drawItem(item,false);
  }}
}

