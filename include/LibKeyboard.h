#ifndef _LIBKEYBOARD_H
#define _LIBKEYBOARD_H
#include <Keypad.h>	


enum e_Button {
	ButtonNone,
	ButtonMenu, ButtonSpindle,  ButtonHomingC,
	ButtonOriginX, ButtonOriginY, ButtonOriginZ,
	ButtonStart, ButtonPause, ButtonStop, ButtonReset,
	ButtonReinit
};

class Keyboard
{
	public:
		Keyboard();
		~Keyboard();
		e_Button check();

	private:
		byte ROWS;
		byte COLS;
		byte rowPins[4];
		byte colPins[3];
		char hexaKeys[4][3];
		Keypad* keypad =nullptr;
};


#endif