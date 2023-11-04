#ifndef _LIBENCODER_H
#define _LIBENCODER_H

#include <Arduino.h>

enum e_turnVal { NoTurn, Up, Down };
enum e_pushVal { NoPush, Push };

class Encoder
{
public:
	//Encoder(uint32_t, uint32_t, uint32_t);
	//~Encoder();
	static void init(uint32_t, uint32_t, uint32_t);

	static e_turnVal check();

	static void interrupt();

	
private:
	static uint32_t pinCLK;
	static uint32_t pinDT;
	static uint32_t pinPush;

	static boolean turnDetected;
	static boolean upDirectionDetected;
	static bool isPush;
};

//Encoder::Encoder(uint32_t _pinCLK, uint32_t _pinDT, uint32_t _pinPush)
void Encoder::init(uint32_t _pinCLK, uint32_t _pinDT, uint32_t _pinPush)
{
	uint32_t pinCLK = _pinCLK;
	uint32_t pinDT = _pinDT;
	uint32_t pinPush = _pinPush;

	pinMode(pinCLK, INPUT_PULLUP);										// rotary encoder CLK
	pinMode(pinDT, INPUT_PULLUP);										// rotary encoder DT
	pinMode(pinPush, INPUT); digitalWrite(pinPush, HIGH);		//PUSH BUTTON ENCODER 

	attachInterrupt(pinCLK, Encoder::interrupt, RISING); // interruption pour lire la rotation de l'encoder rotatif

	isPush = true; // variable pour supprimer les rebonds

}
/*
Encoder::~Encoder()
{
}
*/
e_turnVal Encoder::check()
{

}

void Encoder::interrupt()
{
	if (digitalRead(Encoder::pinCLK))
		upDirectionDetected = !digitalRead(Encoder::pinDT);
	else
		upDirectionDetected = digitalRead(Encoder::pinDT);
	Encoder::turnDetected = true;
}

#endif