#ifndef _LIBBIPPER_H
#define _LIBBIPPER_H

#include <Arduino.h>

class Bipper
{
public:
	
	Bipper(int);
	~Bipper();
	void update();
	void start(uint32_t);
	void start(uint32_t, uint32_t);

private:
	int pin;			//the pin where the bipper is plug
	uint32_t period;
	uint32_t time;		
	bool isBuzzing;		//true if sound
};

#endif