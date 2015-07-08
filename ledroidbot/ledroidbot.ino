#include <ax12.h>

/**
Ledroid Xbee/arbotix packatized control firmware


Developed by Christopher P. Yarger <cpyarger@gmail.com> 

Released under GPLv3.

Notes:

Packets are Broadcast to ALL Xbee modules on mesh network, Device ID is by the induvidual devices to determine if they either execute the commands or discard them. 




Packet makeup

(<Command code>,<DeviceID>,<servoID1Position>,<ServoID2Position>,<ServoID3Position>,<ServoID4Position>,<LightPWMLevel>)
Example

ABC,0123,512,512,512,512,128

When this packet is sent, On Device "0123", All Servos should center themselves, and the PWM for the light should be set to 25%




**/
String DeviceID="0001";
String incoming; 
bool Execute = 0;



void setup(){
Serial.begin(9600);
Serial.print("Arbotix controller ");
Serial.print(DeviceID);
Serial.println(" Initializing");
}
void loop(){}


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
  Serial.println("###########################"); 
}
