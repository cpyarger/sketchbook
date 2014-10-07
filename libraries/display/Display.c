#include <io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include "Types.h"
#include "Display.h"


void SendLetterToDisplay(char letter)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {}
	UDR0 = letter;
	while ((UCSR1A & (1 << UDRE1)) == 0) {}
	UDR1 = letter;
	MillisecondDelay();
	MillisecondDelay();
}


void SendStringToDisplay(char * string)
{
	while (*string)
	{
		SendLetterToDisplay(*string);
		string++;
	}
}


void SendByteToDisplay(byte number)
{
	char string[4];

	utoa(number, string, 16);
	if (strlen(string) < 2)
		SendLetterToDisplay('0');
	SendStringToDisplay(string);
}


void SendNumberToDisplay(unsigned int number)
{
	char string[16];

	utoa(number, string, 10);
	SendStringToDisplay(string);
}


void SendLongToDisplay(unsigned long number)
{
	char string[16];

	ultoa(number, string, 10);
	SendStringToDisplay(string);
}


void SendTicksToDisplay(unsigned int ticks)
{
	unsigned int time;

	time = ticks/(LOOP_RATE/10);
	SendNumberToDisplay(time/10);
	SendLetterToDisplay('.');
	SendNumberToDisplay(time%10);
}


void ClearDisplay(void)
{	// 1500uS
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x51);
	MillisecondDelay();
	MillisecondDelay();
	MillisecondDelay();
}


void TurnDisplayOff(void)
{	// 100uS
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x53);
	SendLetterToDisplay(0x01);	// set brightness to 1
}


void TurnDisplayOn(void)
{	// 100uS
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x53);
	SendLetterToDisplay(0x07);	// set brightness to 7
}


void DisplayFirmwareVersion(void)
{	// 100uS
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x70);
	MillisecondDelay();
	MillisecondDelay();
	MillisecondDelay();
	MillisecondDelay();
	MillisecondDelay();
}


void SendCustomCharactersToDisplay(void)
{	// 200uS each
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(1);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(2);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(3);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(4);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(5);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(6);
	SendLetterToDisplay(0);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);

	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x54);
	SendLetterToDisplay(7);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
	SendLetterToDisplay(0xFF);
}


void MoveCursorTo(byte location)
{	// 100uS
	SendLetterToDisplay(0xFE);
	SendLetterToDisplay(0x45);
	SendLetterToDisplay(location);
}


void MoveCursorToRowColumn(byte row, byte column)
{
	byte address = 0;

	switch(row)
	{
		case 0:
			address = 0x00;
			break;
		case 1:
			address = 0x40;
			break;
		case 2:
			address = 0x14;
			break;
		case 3:
			address = 0x54;
			break;
	}
	address += column;
	MoveCursorTo(address);
}


void ShowButton(byte button, char * text)
{
	byte column = 0;

	switch(button)
	{
		case 1:
			column = 1;
			break;
		case 2:
			column = 6;
			break;
		case 3:
			column = 11;
			break;
		case 4:
			column = 16;
			break;
	}
	MoveCursorToRowColumn(3, column);
	SendStringToDisplay(text);
}


void ShowButtons(char * string1, char * string2, char * string3, char * string4)
{
	ShowButton(1, string1);
	ShowButton(2, string2);
	ShowButton(3, string3);
	ShowButton(4, string4);
}


void HideButtons(void)
{
	ShowButton(1, "    ");
	ShowButton(2, "    ");
	ShowButton(3, "    ");
	ShowButton(4, "   ");
}
