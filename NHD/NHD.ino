
 
boolean debug = true;
 
#include <SoftwareSerial.h>
SoftwareSerial lcd(NULL, 10); // RX, TX

void setup() {
     if (debug) {
        Serial.begin(9600);
        while (!Serial) {
               ; // wait for serial port to connect. Needed for Leonardo only
        }
        Serial.println("LCD Test");
     }
     
     // set the data rate for the LCD's SoftwareSerial port
     lcd.begin(9600);
     delay(100);  // data sheet states 100mS is required upon power-up for the display's controller to initialize 
     
     lcdClearDisplay();
     lcdBlinkingCursorOn();
     
     if (debug) { // show the display's current status
        lcdReportSelf();
     }
     
     lcdClearDisplay();
     lcd.write("\nHello, world !!    "); // drops the "H" without the \n in front of the character string. Why ??
     lcd.write("Line2");                 // completely skips the 2nd line of the display
     lcdSetCursor(0);                    // displays garbage instead of repositioning the cursor
}

void loop() {

}



////////////////////////////////////////////////////////////////////////////
// LCD Command Set
//--------------------------------------------------------------------------
// Specific to: New Haven Serial LCD Display, 4x20
// Manufacturer Part Number: NHD-0420D3Z-FL-GBW-V3
//--------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////
// LCD Command Set
//--------------------------------------------------------------------------
// Specific to: New Haven Serial LCD Display, 4x20
// Manufacturer Part Number: NHD-0420D3Z-FL-GBW-V3
//--------------------------------------------------------------------------
void lcdReportSelf() {
     lcdDisplayFirmwareVersion(); delay(2500);
     lcdDisplayBaudRate();        delay(2500);
     lcdDisplayI2CAddress();      delay(2500);
} // end lcdReportSelf()

void lcdClearLine(int line) {
     if (line < 1) line = 1;
     if (line > 4) line = 4;
     byte charpos;
     switch (line) {
            case 1: charpos = 0;  break;
            case 2: charpos = 64; break;
            case 3: charpos = 20; break;
            case 4: charpos = 84; break;
     }
     // clear the contents of the specified line
     lcdSetCursor(charpos); lcd.write("                    ");
} // end lcdClearLine()

void lcdWriteLine(int line, char* text) {
     if (line < 1) line = 1;
     if (line > 4) line = 4;
     byte charpos;
     switch (line) {
            case 1: charpos = 0;  break;
            case 2: charpos = 64; break;
            case 3: charpos = 20; break;
            case 4: charpos = 84; break;
     }
     // clear the contents of the specified line
     lcdSetCursor(charpos); lcd.write("                    ");
     // write the specified text
     lcdSetCursor(charpos); lcd.write(text);
} // end lcdWriteLine()

void lcdPrintAt(int charpos, char* text) {
     lcdSetCursor(charpos); lcd.write(text);      
} // end lcdPrintAt()

// LCD Core Functions
void lcdDisplayOn()                        { lcd.write(254); lcd.write(65); delay(1);                         }
void lcdDisplayOff()                       { lcd.write(254); lcd.write(66); delay(1);                         }
void lcdSetCursor(byte position)           { lcd.write(254); lcd.write(69); lcd.write(position); delay(1);    }
void lcdCursorHome()                       { lcd.write(254); lcd.write(70); delay(2);                         }
void lcdUnderlineCursorOn()                { lcd.write(254); lcd.write(71); delay(2);                         }
void lcdUnderlineCursorOff()               { lcd.write(254); lcd.write(72); delay(2);                         }
void lcdCursorLeft()                       { lcd.write(254); lcd.write(73); delay(1);                         }
void lcdCursorRight()                      { lcd.write(254); lcd.write(74); delay(1);                         }
void lcdBlinkingCursorOn()                 { lcd.write(254); lcd.write(75); delay(1);                         }
void lcdBlinkingCursorOff()                { lcd.write(254); lcd.write(76); delay(1);                         }
void lcdBackspace()                        { lcd.write(254); lcd.write(78); delay(1);                         }
void lcdClearDisplay()                     { lcd.write(254); lcd.write(81); delay(2);                         }
void lcdSetContrast(byte value)            {
     if (value < 1) value = 1;
     if (value > 50) value = 50;
     lcd.write(254); lcd.write(82); lcd.write(value); delay(1);       }
void lcdSetBacklightBrightness(byte value) { lcd.write(254); lcd.write(83); lcd.write(value); delay(1);       }
void lcdMoveDisplayLeft()                  { lcd.write(254); lcd.write(85); delay(1);                         }
void lcdMoveDisplayRight()                 { lcd.write(254); lcd.write(86); delay(1);                         }
void lcdChangeBaudRate(byte newbaudrate)   { lcd.write(254); lcd.write(97); lcd.write(newbaudrate); delay(3); }
void lcdChangeI2CAddress(byte newaddress)  { lcd.write(254); lcd.write(98); lcd.write(newaddress); delay(3);  }
void lcdDisplayFirmwareVersion()           { lcd.write(254); lcd.write(112); delay(4);                        }
void lcdDisplayBaudRate()                  { lcd.write(254); lcd.write(113); delay(10);                       }
void lcdDisplayI2CAddress()                { lcd.write(254); lcd.write(114); delay(4);                        }

