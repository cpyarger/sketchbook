/*
# this code has been modified from the original by cpyarger
 # This Sample code is for testing the Digital Servo Shield.

 # Editor : YouYou
 # Date   : 2014.10.24
 # Ver    : 1.0
 # Product: Digital Servo Shield for Arduino
 # SKU    :

 # Hardwares:
 1. Arduino UNO
 2. Digital Servo Shield for Arduino
 3. Digital Servos( Compatible with AX-12,CDS55xx...etc)
 4. Power supply:6.5 - 12V

 */

#include <SPI.h>
#include <ServoCds55.h>
ServoCds55 myservo;

int pos;
int interval = 50;

void setup (void)
{
  Serial.begin (115200);
  myservo.begin ();
  Serial.println("Begin Start Test");
  pos = 0;
  myservo.write(1,pos);//ID:1  Pos:300  velocity:150
  delay(1000);
  pos = 300;
  myservo.write(1,pos);//ID:1  Pos:300  velocity:150
  delay(1000);
  pos = 150;
  myservo.write(1,pos);//ID:1  Pos:300  velocity:150
  delay(1000);
  Serial.println("Start Test Complete");
}

void loop (void)
{

  if(Serial.available()){
    char val = Serial.read();
    if(val != -1)
    {
      switch(val)
      {
      case 'a':
        pos = pos - interval;
        pos = constrain(pos, 0, 300);
        myservo.write(1,pos);//ID:1  Pos:300  velocity:150
        break;

      case 's':
       pos = 150;
       myservo.write(1, pos);//ID:1  Pos:0  velocity:150
       break;

      case 'd':
       pos = pos + interval;
       pos = constrain(pos, 0, 300);
       myservo.write(1,pos);//ID:1  Pos:300  velocity:150
       break;
//      case 'i':
//        myservo.SetID(1,2);//ID:1   newID:2
//        break;
//
//        case 'r':
//         myservo.Reset(2);//Restore ID2 servo to factory Settings ( ID:1  Baud rate:1000000)
//         break;

      }
    }
    else
      Serial.println("Wait");
    delay(500);

  }

}
