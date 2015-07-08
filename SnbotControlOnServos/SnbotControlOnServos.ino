/*
 Snobot Controller
 
 This sketch is used to control the snobot 
 Written by Christopher P. Yarger      cpyarger@gmail.com
 Copyright 2013 CPYarger IT Services.  http://cpyarger.com
 Released under GPLv3.
 
 
 
 Controls(All CAPS):
 
 Stop       = Q
 Forward    = W
 Backward   = S
 Left       = A
 Rught      = D
 Speed-Up   = O
 Speed-Down = L 
 Run_Demo   = T
 
 NOTES:
 
 * Speed control returns values between 25 and 255.
 * The stop command bypasses the speed controller, 
 with the excaption of issuing a speed reset command.
 * You can only change speed when the motors are in use.
 * any attempt to change speed otherwise will result in 
 the value being reset to 25
 
 */
#include <ax12.h>

int servo_id = 1;
#define AX_CW_ANGLE_LIMIT_L 6
#define AX_CW_ANGLE_LIMIT_H 7
#define AX_CCW_ANGLE_LIMIT_L 8
#define AX_CCW_ANGLE_LIMIT_H 9
#define TRIGGER_PIN  22
#define ECHO_PIN     22
#define MAX_DISTANCE 500

int rALI = 6;
int rBLI = 8;
int lALI = 9;    // LED connected to digital pin 9
int lBLI = 10;
float sspeed = 55;
float cspeed = 0;
unsigned char last; 
int turn_delay = 200; //delay used to smooth out driving
int cmax = 700; // max speed. no higher than 250
int cinterval = 10; // speed increase / decrease interval
int avoid_on = 0;
int avoid_speed=55; //Turning speed for object avoidence
int pingd = 50; //object detection distace
int avoidState = '0';

void setup()  { 
  // we set all 4 control pins to output
ax12SetRegister2(servo_id, AX_CW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id, AX_CCW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id+1, AX_CW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id+1, AX_CCW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(1, 32,  0);
ax12SetRegister2(2, 32,  0);
  // Lets start the serial
  Serial.begin(9600);
  Serial.println(" Welcome To the snobot controls"); 
  Serial.println("   Drive Safely!");

  help();
} 
void help(){
  Serial.println(" Controls(All lower case):");
  Serial.println(" Stop       = q");
  Serial.println(" Forward    = w");
  Serial.println(" Backward   = s");
  Serial.println(" Left       = a");
  Serial.println(" Right      = d");
  Serial.println(" Speed-Up   = o");
  Serial.println(" Speed-Down = l");
  Serial.println(" Distance   = x"); 
  Serial.println(" Run_Demo   = t"); 
  Serial.println(" Help       = h");

  
}
void Left(int mspeed){

  ax12SetRegister2(1, 32,  1024+mspeed);
   ax12SetRegister2(2, 32,  1024+mspeed);
  delay(turn_delay);
}
void Right(int mspeed){
  ax12SetRegister2(1, 32,  mspeed);
   ax12SetRegister2(2, 32,  mspeed);
     delay(turn_delay);

}
void Forward(int mspeed){
  ax12SetRegister2(1, 32,  mspeed);
   ax12SetRegister2(2, 32,  1024+mspeed);
}
void Backward(int mspeed){
  ax12SetRegister2(1, 32,  1024+mspeed);
   ax12SetRegister2(2, 32,  mspeed);


}
void Stop(){
  // by resetting all control pins to low the motors turn off.
ax12SetRegister2(1, 32,  0);
ax12SetRegister2(2, 32,  0);

  speedy('S');
}
void tStop(){
  // by resetting all control pins to low the motors turn off.
ax12SetRegister2(1, 32,  0);
ax12SetRegister2(2, 32,  0);
  delay(turn_delay);

}

int serialRead(){
  //if (Serial.available() > 0) {
  switch(Serial.read()){

  case 'w': //Forward
    tStop();
    ctrl('F');
    last = 'F';
    Serial.print("Forward: ");
    Serial.println(speedy('R'));
    break;
  case 's': //Backward
    Serial.print("Backward: ");
    Serial.println(speedy('R'));
    tStop();

    ctrl('B');
    last = 'B';
    break;
  case 'a': // Left
    Serial.print("Left: ");
    Serial.println(speedy('R'));
    tStop();

    ctrl('R');
    last = 'R';
    break;  
  case 'd': //Right
    Serial.print("Right: ");
    Serial.println(speedy('R'));
    tStop();

    ctrl('L');
    last = 'L';
    break;
  case 'o': // Speed-Up
    ctrl('U');
    Serial.print("Speed_up: ");
    Serial.println(speedy('R'));
    break;
  case 'l': //Speed-Down
    ctrl('D');
    Serial.print("Speed_Down: ");
    Serial.println(speedy('R'));
    break;
  case 't': //Run_Demo
    ctrl('T');
    last = 'T';
    break;
  case 'q': //Stop
    Serial.print("Stop: ");
    Serial.println(speedy('R'));
    ctrl('S');
    last='S';
    break;
  case 'h':
    help();
    break;
  case 'x':

    break;
  case 'e':

   break;
  default:
    ctrl(last);
    break;

  }// End switch
  //}  //end if SERIAL READ
  //if (Serial1.available() > 0) {

  //}  //end if SERIAL READ
}//end Serial1Read
void ctrl(unsigned char ctr){
  unsigned char Last = ctr;
  switch(Last){

  case 'F':

    Forward(speedy('R'));
    Last='F';
    break;
  case 'B':

    Backward(speedy('R'));
    Last='B';
    break;
  case 'R':

    Left(speedy('R'));
    Last='R';
    break;  
  case 'L':
    Right(speedy('R'));
    Last='L';
    break;
  case 'U':
    speedy('U');
    break;
  case 'D':
    speedy('D');
    break;
  case 'T':
    Demo();
    Last='S';
    break;
  case 'S':
    Stop();
    Last = 'S';
    break;


  }// End switch

}//end Serial1Read
int speedy(unsigned char sCtl){

  switch(sCtl){
  case 'U': 
    //    if (cspeed >= 250){ cspeed=255;}
    //    else { cspeed+=25; }
    cspeed+=cinterval;
    //        cspeed = constrain(cspeed, 0, 255);

    break;
  case 'D':  
    //      if (cspeed <= 5){ cspeed=0;}
    //      else { cspeed-=25; }
    cspeed-=cinterval;

    break;
  case 'S':
    cspeed=0;
    break;
  case 'R':
    cspeed = constrain(cspeed, 0, cmax);
    //      cspeed = max(cspeed, 0);    
    return cspeed;
    break;
  }

}
void Demo(){
  Backward(sspeed); //go backwards at full speed

  Serial.print("Backward: ");
  Serial.println(speedy('R'));
  delay (500); 
  Forward(sspeed);
  Serial.print("Forward: ");
  Serial.println(speedy('R'));
  delay (500);
  Left(sspeed);
  Serial.print("Left: ");
  Serial.println(speedy('R'));
  delay (500);
  Right(sspeed);
  Serial.print("Right: ");
  Serial.println(speedy('R'));
  delay (500);
  Stop();
  Serial.print("Stop: ");
  Serial.println(speedy('R'));

  delay (500);
}//end demo
void Run(){

  serialRead();
}
void speedTest(){
  serialRead();
  Serial.print("Speed: ");
  Serial.println(speedy('R'));

}
void runTest(){
  Run();
}
void loop()  { 
  //speedTest(); //tests speed control
  //Demo(); // show working LEDs
  runTest(); //print's current speed to serial line
  //Run();// Run the program
}
