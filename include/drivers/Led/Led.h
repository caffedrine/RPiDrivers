//
// Created by curiosul on 11/20/18.
//

#ifndef _LED_H
#define _LED_H

#include <utils/time_utils.h>
#include "GpioBase.h"

class Led : public GpioBase
{
	enum class LedState
	{
		ON = 1,
		OFF = 0
	};
	
public:
	LedState State;
	
	Led(uint8_t _gpio_pin);
	Led(uint8_t _gpio_pin, uint64_t AutoTurnOffMillis);
	
	void SetReversedPolarity(bool reversed);
	void On();
	void Off();
	void Tick();
	
private:
	uint64_t LedOnTimestamp = 0;
	uint64_t AutoTurnOffInterval = 0;
	TimeUtils::Timer timer;
	bool IsReversed = false;
};


#endif //_LED_H
