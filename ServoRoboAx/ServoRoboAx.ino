/***************************
 * AXSimpleTest
 * This sketch sends positional commands to the AX servo 
 * attached to it - the servo must set to ID # 1
 * The sketch will send a value, i, to the servo.
 * 'For' loops are used to increment and decrement the value of 'i'
 ***************************/

//import ax12 library to send DYNAMIXEL commands
#include <ax12.h>

int servo_id = 1;
#define AX_CW_ANGLE_LIMIT_L 6
#define AX_CW_ANGLE_LIMIT_H 7
#define AX_CCW_ANGLE_LIMIT_L 8
#define AX_CCW_ANGLE_LIMIT_H 9
void setup()
{
ax12SetRegister2(servo_id, AX_CW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id, AX_CCW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id+1, AX_CW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(servo_id+1, AX_CCW_ANGLE_LIMIT_L, 0); // write two bytes to memory (ADDR, ADDR+1)
ax12SetRegister2(1, 32,  0);
ax12SetRegister2(2, 32,  0);
}

void loop()
{

}

void stopall(){
ax12SetRegister2(1, 32,  0);
ax12SetRegister2(2, 32,  0);
}  
void dance (){
  forward(200);
    delay(1000);
      back(200);
    delay(1000);
    tleft(200);
      delay(1000);
    tright(200);
      delay(1000);
}
void forward(int ispeed){
  ax12SetRegister2(1, 32,  ispeed);
   ax12SetRegister2(2, 32,  1024+ispeed);
}

void back(int ispeed){
  ax12SetRegister2(1, 32,  1024+ispeed);
   ax12SetRegister2(2, 32,  ispeed);
}

void tleft(int ispeed){
  ax12SetRegister2(1, 32,  1024+ispeed);
   ax12SetRegister2(2, 32,  1024+ispeed);
}

void tright(int ispeed){
  ax12SetRegister2(1, 32,  ispeed);
   ax12SetRegister2(2, 32,  ispeed);
}
