//
// Created by curiosul on 11/15/18.
//

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <stdint.h>

#include "GpioPooling.h"


class Encoder : public GpioPooling
{
	enum class EncoderState
	{
		HIGH = 1,
		LOW = 0
	};
	
public:
	uint64_t Steps = 0;
	EncoderState  State;
	
	Encoder(uint8_t gpio_pin);
	Encoder(uint8_t gpio_pin, uint32_t denounce_ms);
	void Reset();
	void ReversePolarity();
	
protected:
	uint8_t gpio_pin;
	void onGpioStateChanged(LogicalLevel newState) override;
	
protected:
	virtual void onStep() {};
};


#endif //_ENCODER_H_
