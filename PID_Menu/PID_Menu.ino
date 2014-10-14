/********************************************************
 * CPYarger PID Menu Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/
#include <EEPROM.h>
#include <PID_v1.h>
#include <SerialUI.h>
#include "PID_Menu.h"
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
int firstRun=EEPROM.read(1);
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void sWrite(double i)
{
EEPROM.write(2,i );
}

SUI::SerialUI mySUI = SUI::SerialUI(device_greeting);
void show_info()
{
   /* we will output some information.  To send 
      data to the user, always use the SerialUI 
      object (using it in exactly the same way 
      as the normal Arduino Serial):         
   */
   mySUI.print("SetPoint: ");
   mySUI.println(sRead());
}

void ssp() // Set SetPoint
{
   /* here we'd turn the device "on" 
      (whatever that means)
      for now, we just:
   */
   mySUI.print(F("Enter Setpoint"));
   mySUI.showEnterNumericDataPrompt();
   double someNum = mySUI.parseInt();
   sWrite(someNum);
}

void setup()
{
  if (firstRun == 255)
  {
    sWrite(100);
    EEPROM.write(1,0);
  }
  //initialize the variables we're linked to
  Input = analogRead(0);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  mySUI.begin(115200);

  SUI::Menu * mainMenu = mySUI.topLevelMenu();

  // Give the top-level menu a decent name
  mainMenu->setName(top_menu_title);

  /* we add commands using... addCommand()
     passing it the key, callback and 
     optionally help string
  */
  mainMenu->addCommand(info_key, show_info, info_help);
  mainMenu->addCommand(enable_ssp_key, ssp);

  // We are done, yay!
}
void run(){
   Setpoint = sRead();
  Input = analogRead(0);
  myPID.Compute();
  analogWrite(3,Output);
}
void loop()
{
  if (mySUI.checkForUser(150))
  {
    // we have a user initiating contact, show the 
    // greeting message and prompt
    mySUI.enter();

    // keep handling the serial user's 
    // requests until they exit or timeout.
    while (mySUI.userPresent())
    {
      // actually respond to requests, using
      mySUI.handleRequests();
    }

  } // end if we had a user on the serial line
 run();
}



double sRead()
{
  return (EEPROM.read(2));
}
