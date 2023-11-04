#include "LibBipper.h"


Bipper::Bipper(int _pin)
{
	pin = _pin;
	pinMode(pin, OUTPUT); analogWrite(pin, LOW);	// Sortie buzzer
	period = 0;
	time = 0;
	isBuzzing = false;

}

Bipper::~Bipper()
{
	pinMode(pin, OUTPUT); analogWrite(pin, LOW);
	period = 0;
	time = 0;
	isBuzzing = false;
}

void Bipper::update()
{
	if (isBuzzing)
	{
		if (time + period < millis())
		{
			analogWrite(pin, LOW);
			period = 0;
			time = 0;
			isBuzzing = false;
		}
	}
}

void Bipper::start(uint32_t _period)
{
	period = _period;
	analogWrite(pin, 255);
	time = millis();
	isBuzzing = true;
}

void Bipper::start(uint32_t _period, uint32_t _sound)
{
	period = _period;
	analogWrite(pin, _sound);
	time = millis();
	isBuzzing = true;
}