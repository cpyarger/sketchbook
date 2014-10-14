char* leadingZero[]={
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"}; //Char array for Leading Zeros

  String h; //Hours
  String m; //Minutes

void DrawTime(){
    h=String(hour(now()));
    m=String(minute(now()));
  //DrawTimes();
  char *s1;//Character array that date strings are dumped into 
  String s2=h+":"+m; //Time format string
  s1 = strdup(s2.c_str()); //Convert String into char*
  Serial.print("time: ");
  Serial.println(s1);
  Tft.drawString(s1,10,100,3,WHITE);
}

