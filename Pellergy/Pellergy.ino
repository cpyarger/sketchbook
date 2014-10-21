#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Adafruit_STMPE610.h>//touchscreen
#include <Adafruit_ILI9341.h> //tft

int runtime=1000;
int j=0;


int menuInt=1;


int TestHour=0;
int TestMin=0;
int THL=0;
int TML=1;
String H; //Hours
String M; //Minutes
String timeString =  "00:00";
char *TimeString;
int lmin=255;
int ms;
int hs;
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
int curtime;
int cmin;
int i=0;

/*
#define TS_MINX 400
 #define TS_MINY 650
 #define TS_MAXX 3600
 #define TS_MAXY 3800
 */
#define TS_MINX 400
#define TS_MINY 270
#define TS_MAXX 3690
#define TS_MAXY 3500
#define STMPE_CS 8


Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define Version 0.1
#define x1 0
#define x2 101
#define y1 0
#define y2 51
#define y3 102
#define y4 153
#define dts 3
int done=0;
int x=0;
int h=50;

TS_Point p;

char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int w=tft.height();
boolean EEPROMSave(){
}


void drawButton( int y, String title, int color, int tcolor ){
  tft.fillRect(x,y,w,h, ILI9341_BLUE );

  tft.fillRect(x+2,y+2,w-4,h-4, color);
  tft.setCursor(x+5, y+5);
  tft.setTextColor(tcolor);
  tft.print(title);
}
int statBar(int statLevel, String Label){
  int x = 0;
  int y = tft.height()-40;
  int w = tft.width();
  int h = 40;
  tft.fillRect(x,y,w,h, ILI9341_BLUE );
  int color;
  tft.setTextColor(ILI9341_BLACK); 
  switch (statLevel){

  case 0: 
    color=ILI9341_GREEN;  
    break; // Ok
  case 1: 
    color=ILI9341_YELLOW; 
    break; // Alert
  case 2: 
    color=ILI9341_RED;    
    break; // Alarm
  default: 
    color=ILI9341_GREEN; 
    break; // Default Value
  }
  tft.fillRect(x+2,y+2,w-4,h-4, color);
  tft.setCursor(x+5,y+5);
  tft.println(Label);
  tft.setTextColor(ILI9341_GREEN);
  return statLevel;
}
boolean checkButton(int y){

  int Y=p.x;

  y=tft.height()-y;

  Serial.println("y:"+String(y)+"; y+50:"+String(y+h));
  Serial.println("Y:"+String(Y));


  if((Y > y) && (Y <= (y +h))) {
    Serial.println("Pressed");
    return true;
  }
  else {
    return false;
  }

}
void setup(){
  Serial.begin(9600);
  Serial.println("Serial Begin");
  tft.begin();
  Serial.println("tft Begin");
  tft.setRotation(1); 
  drawScreen(5);

  if (!ts.begin()) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }

  //tft.fillScreen(ILI9341_BLUE);
  // origin = left,top landscape (USB left upper)
  setSyncProvider(RTC.get);

  Serial.println("Got Time");


  Serial.print("tft.width: ");
  Serial.println(tft.width());
  Serial.print("tft.height: ");
  Serial.println(tft.height());

  drawScreen(1);

}
String theTime(){
  if (menuInt==1){
    if (lmin != minute(now())){
      Serial.println("TheTime function");
      int    hs=hour(now());
      int ms=minute(now());
      if (hs >= 0 && hs < 10) {
        H=leadingZero[hs];
      }
      else{
        H=String(hs);
      }
      //Give Leading Zeros To Minutes
      if (ms >= 0 && ms < 10) {
        M=leadingZero[ms];
      }
      else{
        M=String(ms);
      }

      timeString=H+":"+M;
      char *TimeString = strdup(timeString.c_str()); //Convert String into char*
      Serial.print("time: ");
      Serial.println(TimeString);
      Serial.println("drawTime");
      //  tft.fillScreen(ILI9341_BLACK);

      tft.setCursor(95, 30);
      tft.setTextSize(4);
      tft.fillRect(95,30,125,30,ILI9341_BLACK );

      tft.print(TimeString);
      tft.setTextSize(dts);

      lmin=minute(now());
    }
  }
  return timeString;
}



int checkPress(){
  //Serial.println("Function CheckPress");

  p = ts.getPoint();



  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  if (p.z > 50){
    Serial.print("px2:");
    Serial.print(p.x);
    Serial.print(",");
    Serial.print("py2u:");
    Serial.println(p.y);



    if (ts.touched())
    {
      switch (menuInt){
      case 1:
        if (checkButton(y3)){
          Serial.print("Success:");
          Serial.println(menuInt);
          return 2;
          
        }
        break;
      
      case 2:
      if (checkButton(y1)){
          Serial.println("TestsBtn");
          return 11;
        }
        
        else if (checkButton(y2)){
          Serial.println("SetupBtn");
          return 3;
        }
        else if (checkButton(y3)){
          Serial.println("InfoBtn");
          return 5;
        }
        else if (checkButton(y4)){
          Serial.println("BackBtn");
          return 1;
        } 
        break;//end if checkButton
      case 3:
      if (checkButton(y4)){
          Serial.println("Success");
          return 4;
        }
        
        break;
        case 4:
        if (checkButton(y1)){
          Serial.println("Success");
          return 6; //Mode
        }
        if (checkButton(y2)){
          Serial.println("Success");
          return 7; //RunTiem
        }
        if (checkButton(y3)){
          Serial.println("Success");
          return 8;//SetTime
        }
        if (checkButton(y4)){
          Serial.println("Saving");
          if (!EEPROMSave()){
            Serial.println("EEPROM Save Success");
          }
          else {
            Serial.println("EEPROM Save Success");
          }
          return 1;
        }
        break;
        case 5:
        if (checkButton(y4)){
        return 2;
      }
        break;
    } //endSwitch

    }//end  ts.touced
  }//End Pressure
  return 0;
}//end Funtion

void drawStatus(){
  //     drawButton(tft.height()-40,40,"Status: XXXX",ILI9341_GREEN,ILI9341_RED );
}
void drawScreen(int menu){
  Serial.print("Function Drawscreen:");
  Serial.println(menu);
  menuInt=menu;
  switch(menu){
  case 1: 
    //Default Screen
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y3,"Menu",ILI9341_GREEN,ILI9341_RED );  
    done = 1;
    lmin=255;
    Serial.println("Draw Default Screen");
    break;
  case 2:
    //Main Menu
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y1,"Tests",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Setup",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Info",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Back",ILI9341_GREEN, ILI9341_BLACK); 
    done = 1;
    Serial.println("Draw Main Menu");
    break;
  case 3://Setting  Screen
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y1,"Mode",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Run Time",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"SetTime",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Next",ILI9341_GREEN, ILI9341_BLACK); 
    break;
  case 4://TestScreen 2
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y1,"Start Time",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Stop Time",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Back",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Save and end",ILI9341_GREEN, ILI9341_BLACK); 
    break;
  case 5: //Info Screen
    tft.setTextColor(ILI9341_RED);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.println ("Pellergy Vaccume control");
    tft.println("System");
    tft.println("");
    tft.print("Version: ");
    tft.println(Version);
    tft.setTextSize(dts);
        drawButton(y4,"Back",ILI9341_GREEN, ILI9341_BLACK); 

    break;
  case 6: //Mode Setting
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_RED);
    drawButton(y1,"Disable",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Enable + Time",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Enabled W/O Time",ILI9341_GREEN, ILI9341_RED);  

    break;
  case 7: //RunTime
    tft.setTextColor(ILI9341_RED);
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y3,"Up",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Down",ILI9341_GREEN, ILI9341_RED);  
    break;
  case 8: //SetTime
    tft.setTextColor(ILI9341_RED);
    drawButton(y1,"Hour Up",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Hour Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Minute Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Minute Down",ILI9341_GREEN, ILI9341_RED);  
    break;
  case 9: //Start Time
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y1,"Hour Up",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Hour Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Minute Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Minute Down",ILI9341_GREEN, ILI9341_RED);  

    break;
  case 10: //StopTime
    tft.fillScreen(ILI9341_BLACK);
    drawButton(y1,"Hour Up",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y2,"Hour Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y3,"Minute Down",ILI9341_GREEN, ILI9341_RED);  
    drawButton(y4,"Minute Down",ILI9341_GREEN, ILI9341_RED);  

    break;
  case 11: //TestsMenu    tft.setTextColor(ILI9341_RED);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.println ("Pellergy Vaccume control");
    tft.println("System");
    tft.println("");
    tft.print("Version: ");
    tft.println(Version);
    tft.setTextSize(dts);
    break;
  case 12: //Info Screen
    tft.setTextColor(ILI9341_RED);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.println ("Pellergy Vaccume control");
    tft.println("System");
    tft.println("");
    tft.print("Version: ");
    tft.println(Version);
    tft.setTextSize(dts);
    break;
    //  default:
    //    Serial.print("Drawscreen error, No screen # ");
    //    Serial.println(menu); 
  }//End Case

}//End Function
void RunVac(){
}
void    drawRunTime(){
}

void loop(){

  //  if (i == 12){
  //    i=0;
  //  }
  //  if ( j == 3) {
  //    j=0;
  //  }
  //  drawScreen(i);
  //  statBar(j, "Alrighty");
  //  delay (1000);
  //  j++;
  //  i++;
  int x0 =checkPress();
  if (x0!=0){
    drawScreen(x0);
  }//end x0 if
  theTime(); 
  if (menuInt == 1){
    RunVac();
  }

  //FixMe: Implement Menu auto-back later
  //  if (menuInt!=1){
  //cmin=millis()
  //   if (cmin >999){
  //     drawScreen(1);
  //     lcmin=0;
  //   }
  //   cmin = lcmin);
  //  }
  //Serial.println(theTime());

}





