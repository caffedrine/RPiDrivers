//
// Created by curiosul on 11/20/18.
//

#ifndef _RELAY_H
#define _RELAY_H

#include "GpioBase.h"

class Relay : public GpioBase
{
public:
	Relay(uint8_t _gpio_pin);
	void SetReversedPolarity(bool reversed);
	void CloseCircuit();
	void OpenCircuit();
private:
	bool IsReversed = false;
};

#endif //_RELAY_H
