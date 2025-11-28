#pragma once
#include "Item.h"
class Food : public Item
{
private:
	int saturation_units;
public:
	virtual int GetSaturationUnits() const = 0;
	virtual int SetSaturationUnits(int saturation_units);
};

