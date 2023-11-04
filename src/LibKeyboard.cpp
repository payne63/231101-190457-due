#include <Keypad.h>		//Libray for the resistive keyboard
#include "LibKeyboard.h"


Keyboard::Keyboard()
{
	
	ROWS = 4; //four rows
	COLS = 3; //three columns
	rowPins[0] = A6;
	rowPins[1] = A5;
	rowPins[2] = A4;
	rowPins[3] = A3;
	colPins[0] = A2;
	colPins[1] = A1;
	colPins[2] = A0;

	hexaKeys[0][0] = '1';
	hexaKeys[0][1] = '2';
	hexaKeys[0][2] = '3';
	hexaKeys[1][0] = '4';
	hexaKeys[1][1] = '5';
	hexaKeys[1][2] = '6';
	hexaKeys[2][0] = '7';
	hexaKeys[2][1] = '8';
	hexaKeys[2][2] = '9';
	hexaKeys[3][0] = '*';
	hexaKeys[3][1] = '0';
	hexaKeys[3][2] = '#';
	
	//Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
	Keypad* keypad =new Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

}

Keyboard::~Keyboard()
{
}

e_Button Keyboard::check() 
{
    //char a = keypad.getKey();
	char customKey = keypad->getKey();
	if (customKey)					//if a button is pressed
	{
		//	|------------------------------------------------------|
		//  | ButtonMenu | ButtonStart | ButtonStop | ButtonReset  |
		//  |------------------------------------------------------|
		//  |ButtonSpindle|ButtonHomingC|?     |   ?			   |
		//  |------------------------------------------------------|
		//  |ButtonReinit|ButtonOriginX|ButtonOriginY|ButtonOriginZ|
		//  |------------------------------------------------------|
		switch (customKey)
		{
		case '*':
			return ButtonMenu;
			break;	
		case '7':
			return ButtonStart;
			break;	
		case '4':
			return ButtonStop;
			break;
		case '1':
			return ButtonReset;
			break;	
		case '0':		// demarrage ou arret de la broche
			return ButtonSpindle;
			break;
		case '8':		
			return ButtonHomingC;
			break;
		case '5':		
			break;
		case '2':		
			break;
		case '#':
			return ButtonReinit;
			break;
		case '9':
			return ButtonOriginX;
			break;
		case '6':
			return ButtonOriginY;
			break;
		case '3':
			return ButtonOriginZ;
			break;
		default:
			return ButtonNone;
			break;
		
		}
		
	}
	else return ButtonNone;
    return ButtonNone;
}

