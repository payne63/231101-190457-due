#ifndef _LIBSPINDLE_H
#define _LIBSPINDLE_H

#include <Arduino.h>


class Spindle
{
public:
	Spindle(int);
	~Spindle();
	void start();
	void stop();
	bool isOn();
	int maxSpeed();
	void speedAdd(int);
	void speedSet(int);
	int getConsigneSpeed();

private:
	int  spindleSpeed;			//Consigne de la broche
	bool  spindleOn;				//Etat de la broche , true = tourne
	int  spindleMaxSpeedValue;	//Vitese max de la broche
};

Spindle::Spindle(int _max_spindle_speed)
{
	spindleSpeed = 1000;
	spindleOn = false;
	spindleMaxSpeedValue = _max_spindle_speed;
}

Spindle::~Spindle()
{
	spindleSpeed = 0;
	spindleOn = false;
}

void Spindle::start()
{
	spindleOn = true;
	Order::addInListOrder("M03 S" + String(spindleSpeed));
}

void Spindle::stop()
{
	spindleOn = false;
	Order::addInListOrder("M05 ");
}

bool Spindle::isOn()
{
	return spindleOn;
}

int Spindle::maxSpeed()
{
	return spindleMaxSpeedValue;
}

void Spindle::speedAdd(int _valToAdd) 
{
	spindleSpeed += _valToAdd;
	spindleSpeed = constrain(spindleSpeed, 0, spindleMaxSpeedValue);
	Order::addInListOrder("S" + String(spindleSpeed));
}

void Spindle::speedSet(int _valToSet)
{
	spindleSpeed = _valToSet;
	spindleSpeed = constrain(spindleSpeed, 0, spindleMaxSpeedValue);
	Order::addInListOrder("S" + String(spindleSpeed));
}

int Spindle::getConsigneSpeed()
{
	return spindleSpeed;
}

#endif