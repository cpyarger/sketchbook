Ledroid Xbee/arbotix packatized control firmware

**************************************************


Developed by Christopher P. Yarger <cpyarger@gmail.com> 

Released under GPLv3.

**************************************************



Notes:

Packets are Broadcast to ALL Xbee modules on mesh network, Device ID is by the induvidual devices to determine if they either execute the commands or discard them. 

Packet makeup

Prepend,DeviceID,servoID1Position,ServoID2Position,ServoID3Position,ServoID4Position,LightPWMLevel

Example

ABC,0123,512,512,512,512,128


When this packet is sent, On Device "0123", All Servos should center themselves, and the PWM for the light should be set to 25%


or maybe...


Prepend,DeviceID,CommandCode,SID1P,SID2P,SID3P,SID4P,LPWML,CommandCodeParams,?,?,




Potential Commands, 
ChangeLightLevel
ReportVoltage
ReportServoTemps
SelfTestAndReport
???
???
???
