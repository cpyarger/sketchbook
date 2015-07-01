#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"

BioloidController bioloid = BioloidController(1000000);

const int SERVOCOUNT = 2;
int id;
int pos;
boolean IDCheck;
boolean RunCheck;

void setup(){
   pinMode(0,OUTPUT);  
   
   //initialize variables 
   id = 1;
   pos = 0;
   IDCheck = 1;
   RunCheck = 0;
  //open serial port
   Serial.begin(9600);
   delay (500);   
    Serial.println("###########################");    
   Serial.println("Serial Communication Established.");    

  //Check Lipo Battery Voltage
  CheckVoltage();

  MoveCenter();
  
  //Scan Servos, return position.
  ScanServo();
  
  MoveTest();
  
  
  MenuOptions();
 
  RunCheck = 1;
}

void loop(){
  // read the sensor:
  
    int inByte = Serial.read();

    switch (inByte) {

    case '1':    
      ScanServo();
      break;

    case '2':    
      MoveCenter();
      break; 

     case '3':    
      RelaxServos();
      break;     

    case '4':
      MoveCenter();
      MoveTest();
      break;
      
    case '5':    
      CheckVoltage();
      break;

    case '6':
      LEDTest();
      break;
      
    case '7':
      KeyTest();
      break;

    } 
  
}


void ScanServo(){
  id = 1;  
  Serial.println("###########################");
  Serial.println("Starting Servo Scanning Test.");
  Serial.println("###########################");
      
  while (id <= SERVOCOUNT){
  pos =  ax12GetRegister(id, 36, 2);
  Serial.print("Servo ID: ");
  Serial.println(id);
  Serial.print("Servo Position: ");
  Serial.println(pos);
  
  if (pos <= 0){
  Serial.println("###########################");
  Serial.print("ERROR! Servo ID: ");
  Serial.print(id);
  Serial.println(" not found. Please check connection and verify correct ID is set.");
  Serial.println("###########################"); 
  IDCheck = 0;
  }
  
  id = (id++)%SERVOCOUNT;
  delay(1000);
  }
  if (IDCheck == 0){
    Serial.println("###########################");
    Serial.println("ERROR! Servo ID(s) are missing from Scan. Please check connection and verify correct ID is set.");
    Serial.println("###########################");  
  }
  else{
  Serial.println("All servo IDs present.");
  }
    if (RunCheck == 1){
    MenuOptions();
  }

}



void CheckVoltage(){  
   // wait, then check the voltage (LiPO safety)
  float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;
  Serial.println("###########################");   
  Serial.print ("System Voltage: ");
  Serial.print (voltage);
  Serial.println (" volts.");
  if (voltage < 10.0){
    Serial.println("Voltage levels below 10v, please charge battery.");
    while(1);
  }  
  if (voltage > 10.0){
  Serial.println("Voltage levels nominal.");
  }
    if (RunCheck == 1){
      MenuOptions();
  }
      Serial.println("###########################"); 
}

void MoveCenter(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Center);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    Serial.println("###########################");
    Serial.println("Moving servos to centered position");
    Serial.println("###########################");    
    delay(1000);
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
    if (RunCheck == 1){
      MenuOptions();
  }
}





void MoveTest(){
  Serial.println("###########################");
  Serial.println("Initializing Movement Sign Test");  
  Serial.println("###########################");
  delay(500);  
  id = 1;
  pos = 512;
  while(id <= SERVOCOUNT){
  Serial.print("Moving Servo ID: ");
  Serial.println(id);  

  while(pos >= 312){  
  SetPosition(id, pos);
  pos = pos--;
  delay(10);
  }

  while(pos <= 512){  
  SetPosition(id, pos);
  pos = pos++;
  delay(10);
  }

  //iterate to next servo ID
  id = id++;

  }
    if (RunCheck == 1){
   MenuOptions();
  }
  
}

void KeyTest(){
  Serial.println("###########################");
  Serial.println("Initializing Keyboard Control Test");  
  Serial.println("###########################");
  delay(500);  
  int testing = 1;
  int ipos = 512;
  int ipos2 = 512;
  int diff = 10;
  Serial.println("1) Servo id: 1 Increase pos");        
  Serial.println("2) Servo id: 1 Decrease pos");    
  Serial.println("3) Servo id: 2 Increase pos");            
  Serial.println("4) Servo id: 2 Decrease pos");                
  Serial.println("9) Exit Test");   
     
    while (testing == 1){
      int inByte2 = Serial.read();
        int inByte3 = abs(inByte2)-48;
      
      //Debug Serial.println(inByte3);
      //Debug delay(200);

      switch (inByte3){
        case 1:  
          ipos = ipos+diff;
          ipos = constrain(ipos, 0,1024);
          SetPosition(1, ipos);
          delay(10);
          Serial.print("ID: 1, POS: ");
          Serial.println(ipos);
          break;
        case 2:
          ipos = ipos-diff;
          ipos = constrain(ipos, 0,1024);
          SetPosition(1, ipos);
          

          delay(10);
          Serial.print("ID: 1, POS: ");
          Serial.println(ipos);
          break;
        case 3:  
          ipos2 = ipos2+diff;
          ipos2 = constrain(ipos2, 0,1024);
          SetPosition(2, ipos2);
          
          delay(10);
          Serial.print("ID: 2, POS: ");
          Serial.println(ipos2);
          break;
        case 4:
          ipos2 = ipos2-diff ;
          ipos2 = constrain(ipos2, 0,1024);
          SetPosition(2, ipos2);
          
          delay(10);
          Serial.print("ID: 2, POS: ");
          Serial.println(ipos2);
          break;
        case 9:  
          testing=0;
          delay(10);
          break;

    }//endCase
  
} // End while true


    if (RunCheck == 1){
   MenuOptions();
  }
  
}



void MenuOptions(){
  
    Serial.println("###########################"); 
    Serial.println("Please enter option 1-5 to run individual tests again.");     
    Serial.println("1) Servo Scanning Test");        
    Serial.println("2) Move Servos to Center");    
    Serial.println("3) Relax Servos");            
    Serial.println("4) Perform Movement Sign Test");                
    Serial.println("5) Check System Voltage");   
    Serial.println("6) Perform LED Test");       
    Serial.println("7) Keyboard Control Test");       
    Serial.println("###########################"); 
}

void RelaxServos(){
  id = 1;
  Serial.println("###########################");
  Serial.println("Relaxing Servos.");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    Relax(id);
    id = (id++)%SERVOCOUNT;
    delay(50);
  }
   if (RunCheck == 1){
      MenuOptions();
  }
}

void LEDTest(){
    id = 1;
  Serial.println("###########################");
  Serial.println("Running LED Test");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    ax12SetRegister(id, 25, 1);
    Serial.print("LED ON - Servo ID: ");
    Serial.println(id);
    delay(3000);
    ax12SetRegister(id, 25, 0);  
    Serial.print("LED OFF - Servo ID: ");
    Serial.println(id);    
    delay(3000);    
    id = id++;
  }
  
   if (RunCheck == 1){
      MenuOptions();
  }
}
    
