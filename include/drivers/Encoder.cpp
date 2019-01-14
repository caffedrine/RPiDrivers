//
// Created by curiosul on 11/15/18.
//

#include "Encoder.h"

Encoder::Encoder(uint8_t _gpio_pin) : GpioPooling(_gpio_pin)	/* 1ms debouncing */
{
	this->gpio_pin = _gpio_pin;
}

Encoder::Encoder(uint8_t gpio_pin, uint32_t denounce_ms) : GpioPooling(gpio_pin, denounce_ms)
{

}

void Encoder::Reset()
{
	this->Steps = 0;
}

void Encoder::onGpioStateChanged(LogicalLevel newState)
{
	GpioPooling::onGpioStateChanged(newState);
//	if( newState == (LogicalLevel) EncoderState::HIGH )
//	{
		this->Steps++;
		this->State = (EncoderState) newState;
		onStep();
//	}
}

void Encoder::ReversePolarity()
{
	GpioPooling::ReversedPolarity = !GpioPooling::ReversedPolarity;
}



