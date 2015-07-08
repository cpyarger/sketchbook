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
#include <NewPing.h>
#define TRIGGER_PIN  22
#define ECHO_PIN     22
#define MAX_DISTANCE 500

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int rALI = 6;
int rBLI = 8;
int lALI = 9;    // LED connected to digital pin 9
int lBLI = 10;
float sspeed = 55;
float cspeed = 0;
unsigned char last; 
int turn_delay = 200; //delay used to smooth out driving
int cmax = 150; // max speed. no higher than 250
int cinterval = 10; // speed increase / decrease interval
int avoid_on = 0;
int avoid_speed=55; //Turning speed for object avoidence
int pingd = 50; //object detection distace
int avoidState = '0';

void setup()  { 
  // we set all 4 control pins to output
  pinMode(rALI, OUTPUT);
  pinMode(rBLI, OUTPUT);
  pinMode(lALI, OUTPUT);
  pinMode(lBLI, OUTPUT); 
  // Lets start the serial
  Serial.begin(9600);
  Serial.println(" Welcome To the snobot controls"); 
  Serial.println("   Drive Safely!");

  Serial1.begin(9600);
  Serial1.println(" Welcome To the snobot controls"); 
  Serial1.println("   Drive Safely!");
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

  Serial1.println(" Controls(All CAPS):");
  Serial1.println(" Stop       = q");
  Serial1.println(" Forward    = w");
  Serial1.println(" Backward   = s");
  Serial1.println(" Left       = a");
  Serial1.println(" Right      = d");
  Serial1.println(" Speed-Up   = o");
  Serial1.println(" Speed-Down = l"); 
  Serial1.println(" Distance   = x"); 
  Serial1.println(" Run_Demo   = t"); 
  Serial1.println(" Help       = h");
}
void Left(int mspeed){

  digitalWrite(rBLI, LOW);
  digitalWrite(lALI, LOW);
  analogWrite(rALI, mspeed/2);
  analogWrite(lBLI, mspeed/2);
  delay(turn_delay);
}
void Right(int mspeed){
  digitalWrite(rALI, LOW);
  digitalWrite(lBLI, LOW);
  analogWrite(rBLI, mspeed/2);
  analogWrite(lALI, mspeed/2);
}
void Forward(int mspeed){
  digitalWrite(rBLI, LOW);
  digitalWrite(lBLI, LOW);
  analogWrite(rALI, mspeed);
  analogWrite(lALI, mspeed);
}
void Backward(int mspeed){
  digitalWrite(rALI, LOW);
  digitalWrite(lALI, LOW);
  analogWrite(rBLI, mspeed);
  analogWrite(lBLI, mspeed);

}
void Stop(){
  // by resetting all control pins to low the motors turn off.
  digitalWrite(rALI, LOW);
  digitalWrite(lALI, LOW);
  digitalWrite(rBLI, LOW);
  digitalWrite(lBLI, LOW);
  speedy('S');
}
void tStop(){
  // by resetting all control pins to low the motors turn off.
  digitalWrite(rALI, LOW);
  digitalWrite(lALI, LOW);
  digitalWrite(rBLI, LOW);
  digitalWrite(lBLI, LOW);
  delay(turn_delay);

}

int serialRead(){
  //if (Serial.available() > 0) {
  switch(Serial.read()){

  case 'w': //Forward
    tStop();
    ctrl('B');
    last = 'B';
    Serial.print("Forward: ");
    Serial.println(speedy('R'));
    break;
  case 's': //Backward
    Serial.print("Backward: ");
    Serial.println(speedy('R'));
    tStop();

    ctrl('F');
    last = 'F';
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
    print_dist();
    break;
  case 'e':
   avoid_toggle('S');
   break;
  default:
    ctrl(last);
    break;

  }// End switch
  //}  //end if SERIAL READ
  //if (Serial1.available() > 0) {
  switch(Serial1.read()){

  case 'w': //Forward
    tStop();
    ctrl('B');
    last = 'B';
    Serial1.print("Forward: ");
    Serial1.println(speedy('R'));
    break;
  case 's': //Backward
    tStop();
    Serial1.print("Backward: ");
    Serial1.println(speedy('R'));
    ctrl('F');
    last = 'F';
    break;
  case 'a': // Left
    tStop();
    Serial1.print("Left: ");
    Serial1.println(speedy('R'));
    ctrl('R');
    last = 'R';
    break;  
  case 'd': //Right
    tStop();
    Serial1.print("Right: ");
    Serial1.println(speedy('R'));
    ctrl('L');
    last = 'L';
    break;
  case 'o': // Speed-Up
    ctrl('U');
    Serial1.print("Speed_up: ");
    Serial1.println(speedy('R'));
    break;
  case 'l': //Speed-Down
    ctrl('D');
    Serial1.print("Speed_Down: ");
    Serial1.println(speedy('R'));
    break;
  case 't': //Run_Demo
    ctrl('T');
    last = 'T';
    break;
  case 'q': //Stop
    Serial1.print("Stop: ");
    Serial1.println(speedy('R'));
    ctrl('S');
    last='S';
    break;
  case 'h':
    help();
    break;
  default:
    ctrl(last);
    break;

  }// End switch
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
  Serial1.print("Backward: ");
  Serial1.println(speedy('R'));
  Serial.print("Backward: ");
  Serial.println(speedy('R'));
  delay (500); 
  Forward(sspeed);
  Serial1.print("Forward: ");
  Serial1.println(speedy('R'));
  Serial.print("Forward: ");
  Serial.println(speedy('R'));
  delay (500);
  Left(sspeed);
  Serial1.print("Left: ");
  Serial1.println(speedy('R'));
  Serial.print("Left: ");
  Serial.println(speedy('R'));
  delay (500);
  Right(sspeed);
  Serial1.print("Right: ");
  Serial1.println(speedy('R'));
  Serial.print("Right: ");
  Serial.println(speedy('R'));
  delay (500);
  Stop();
  Serial1.print("Stop: ");
  Serial1.println(speedy('R'));
  Serial.print("Stop: ");
  Serial.println(speedy('R'));

  delay (500);
}//end demo
void Run(){

  serialRead();
}
void speedTest(){
  serialRead();
  Serial1.print("Speed: ");
  Serial1.println(speedy('R'));
  Serial.print("Speed: ");
  Serial.println(speedy('R'));

}
void runTest(){
  Run();
}
void avoid(){
  if (avoid_toggle('R') =='1'){
  if ( sonar.ping_in() < pingd){
    //  Backward(20);
    //Stop();
//Serial.println("OBJECT AHEAD....AVOIDING!");
    while (sonar.ping_in()<pingd) {
      Right(avoid_speed);
      delay(10);
    }//end while
    Forward(speedy('R'));
  }//if ping< pingd
  }// if avoid_on
}  //end avoid
int avoid_toggle(int tgl){
  switch (tgl){
    case 'S':
      switch (avoidState){
        case '0':
          avoidState= '1';
          Serial.println("Avoidence on");
          break; 
        case '1':
          avoidState='0';
          Serial.println("Avoidence off");
          break; 
        }    
       break;
     case 'R':
        return avoidState;
        break; 
  }
}

void print_dist(){
  Serial.print("Distance:  ");
  Serial.print(sonar.ping_in());
  Serial.println(" in.");
  Serial.print("avoidance Distance:  ");
  Serial.print(pingd);
  Serial.println(" in.");
  Serial1.print("Distance:  ");
  Serial1.print(sonar.ping_in());
  Serial1.println(" in.");
  Serial1.print("avoidance Distance:  ");
  Serial1.print(pingd);
  Serial1.println(" in.");
}
void loop()  { 
  avoid();
  //speedTest(); //tests speed control
  //Demo(); // show working LEDs
  runTest(); //print's current speed to serial line
  //Run();// Run the program
}
