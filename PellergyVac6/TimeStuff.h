char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

  String h; //Hours
  String m; //Minutes
    String timeString =  "00:00";
int lmin;
void DrawTime(){
  if (lmin != minute(now())){
int    hs=hour(now());
   int ms=minute(now());
 if (hs >= 0 && hs < 10) {
    h=leadingZero[hs];
  }
  else{
    h=String(hs);
  }
  //Give Leading Zeros To Minutes
  
    if (ms >= 0 && ms < 10) {
    m=leadingZero[ms];
  }
  else{
    m=String(ms);
  }
   timeString=h+":"+m;
  char *TimeString = strdup(timeString.c_str()); //Convert String into char*

  Serial.print("time: ");
  Serial.println(TimeString);
 

        TouchScreenArea lbl1 = TouchScreenLabel(TimeString, TSC.createColor(255, 255, 255), TSC.createColor(0, 100, 0), 10, 200, 5, 10, true);
        lbl1.draw();
   
  }
       lmin=minute(now());
}
