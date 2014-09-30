











void TimeMenus(){

  TestHour= constrain(TestHour, 0, 24);
  TestMin = constrain(TestMin, 0, 59);
  String s12=String(TestHour)+":"+String(TestMin); //Time format string
  Serial.println(s12);
  if(curForm!=NULL){
    // process the current menu
    TouchScreenArea *item = curForm->process(false);

    if(item!=NULL){
      if(!strcmp(item->getText(),"MUp")){
        TestMin++;
        redraw();


        //Serial.print("Minute Up: ");
        //Serial.println(TestMin);
      }
      else if(!strcmp(item->getText(),"MDown")){
        TestMin--;
        redraw();
        //Serial.print("Minute Down: ");
        //Serial.println(TestMin); 
      }
      else if(!strcmp(item->getText(),"HUp")){
        TestHour++;
        redraw();
        //Serial.print("Hour up: ");
        //Serial.println(TestHour);  
      }
      else if(!strcmp(item->getText(),"HDown")){
        TestHour--;
        redraw();
        //Serial.print("Hour Down: ");
        //Serial.println(TestHour); 
      }
      else if(!strcmp(item->getText(),"Save")){
        EEPROM.write(THL,TestHour);
        Serial.print("Wrote TestHour with Value: ");
        Serial.println(TestHour);
        EEPROM.write(TML,TestMin);
        Serial.print("Wrote TestMin with Value: ");
        Serial.println(TestMin);


        redraw();
        //Serial.print("Hour Down: ");
        //Serial.println(TestHour); 
      }
    }

  }
  DrawTestTime(10,80);
}


