//
// Created by curiosul on 12.12.18.
//

#include "EncoderInterrupt.h"

EncoderInterrupt::EncoderInterrupt(uint8_t _gpio_pin) : GpioInterrupt(_gpio_pin)	/* 1ms debouncing */
{
	this->gpio_pin = _gpio_pin;
}

EncoderInterrupt::EncoderInterrupt(uint8_t gpio_pin, uint32_t denounce_us) : GpioInterrupt(gpio_pin, denounce_us)
{

}

void EncoderInterrupt::Reset()
{
	this->Steps = 0;
}

void EncoderInterrupt::onGpioStateChanged(LogicalLevel newState)
{
	GpioInterrupt::onGpioStateChanged(newState);
//	if( newState == (LogicalLevel) EncoderState::HIGH )
//	{
	this->Steps++;
	this->State = (EncoderState) newState;
	onStep();
//	}
}

void EncoderInterrupt::ReversePolarity()
{
	GpioInterrupt::ReversedPolarity = !GpioInterrupt::ReversedPolarity;
}
