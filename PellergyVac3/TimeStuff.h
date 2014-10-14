char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

  String h; //Hours
  String m; //Minutes
void DrawTimes(){


//Give Leading Zero's to Hours  
  if (hour() >= 0 && hour() < 10) {
    h=leadingZero[hour()];
  }
  else{
    h=String(hour());
  }
  //Give Leading Zeros To Minutes
  
    if (minute() >= 0 && minute() < 10) {
    m=leadingZero[minute()];
  }
  else{
    m=String(minute());
  }
}
void DrawTime(){
  DrawTimes();
  char *s1;//Character array that date strings are dumped into 
  String s2=h+":"+m; //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char*
  Tft.drawString(s1,60,260,3,BLUE);
}


