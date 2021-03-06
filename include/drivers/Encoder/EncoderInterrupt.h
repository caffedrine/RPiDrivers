//
// Created by curiosul on 12.12.18.
//

#ifndef _ENCODERINTERRUPT_H
#define _ENCODERINTERRUPT_H

#include "Hal.h"
#include "Gpio/GpioInterrupt.h"

class EncoderInterrupt : public GpioInterrupt
{
	enum class EncoderState
	{
		HIGH = 1,
		LOW = 0
	};

public:
	uint64_t Steps = 0;
	EncoderState  State;
	
	explicit EncoderInterrupt(uint8_t gpio_pin);
	EncoderInterrupt(uint8_t gpio_pin, uint32_t denounce_us);
	void Reset();
	void ReversePolarity();

protected:
	uint8_t gpio_pin;
	void onGpioStateChanged(LogicalLevel newState) override;

protected:
	virtual void onStep() {};
};


#endif //_ENCODERINTERRUPT_H
