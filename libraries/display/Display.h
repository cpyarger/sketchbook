#if !defined(DISPLAY_H)
#define DISPLAY_H 1


void SendLetterToDisplay(char letter);
void SendStringToDisplay(char * string);
void SendByteToDisplay(byte number);
void SendNumberToDisplay(unsigned int number);
void SendLongToDisplay(unsigned long number);
void SendTicksToDisplay(unsigned int ticks);
void ClearDisplay(void);
void TurnDisplayOff(void);
void TurnDisplayOn(void);
void DisplayFirmwareVersion(void);
void SendCustomCharactersToDisplay(void);
void MoveCursorTo(byte location);
void MoveCursorToRowColumn(byte row, byte column);
void ShowButton(byte button, char * text);
void ShowButtons(char * string1, char * string2, char * string3, char * string4);
void HideButtons(void);


#endif
