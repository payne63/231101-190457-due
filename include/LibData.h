#ifndef _LIBDATA_H
#define _LIBDATA_H
#include <Arduino.h>


class Data
{
public:

	Data();
	Data(String _letter, int _max, int _min, float _val);
	~Data();
	void add(float _value);
	void set(float _value);

	String letter;
	float val;
	int val_max;
	int val_min;

};

#endif