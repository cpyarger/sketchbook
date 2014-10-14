#include "Version.h"
//-------------------------------------------------------------------------------
// Menus
//-------------------------------------------------------------------------------
// create the array of items for the main menu
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




