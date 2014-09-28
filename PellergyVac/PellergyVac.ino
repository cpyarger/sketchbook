// Draw Texts in the four directions of the TFT by Frankie.Chu
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/*
    Modified record:
    
*/
#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM A0  // can be a digital pin
#define XP A3  // can be a digital pin



#define TS_MINX 140
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


int MenuInt=0;
void setup()
{

  Tft.init();  //init TFT library
  Tft.setDisplayDirect(DOWN2UP);
  MenuInt=1;
  
}
void MainScreen(){
  if (MenuInt == 1 )
  {
  
  Tft.drawString("10:41",10,290,7,BLUE);
    Tft.setDisplayDirect(LEFT2RIGHT);

  Tft.fillRectangle(150, 0, 75,385,ABLUE);
   Tft.setDisplayDirect(DOWN2UP);

  Tft.drawString("MENU",180,203.5,3.5,WHITE);
  MenuInt=0;
}
}

int MainMenu(){

          p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
  Tft.fillCircle(p.x, p.y, 2,GREEN);
    }
}

void ManualOveride(){

}
void ReadTime()
{
}

void SetTime(int Hours, int Mins){
Serial.println(Hours, Mins);
}

void Button(int x, int y, String color){
}
int menuparse(){
    Tft.setDisplayDirect(LEFT2RIGHT);
  TSPoint p = ts.getPoint();
    if (p.z > 50) {
      case
void loop()
{
  
MainScreen();
MainMenu();
//DebugScreen()
}
