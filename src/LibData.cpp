#include "LibData.h"

Data::Data(String _letter, int _min, int _max, float _val = 0)
{
	letter = _letter;
	val_max = _max;
	val_min = _min;
	val = constrain(_val, _min, _max);
}

Data::Data()
{
}
Data::~Data()
{
}

void Data::add(float _val)
{
	val += _val;
	val = constrain(val, val_min, val_max);
}

void Data::set(float _val)
{
	val = constrain(_val, val_min, val_max);
}