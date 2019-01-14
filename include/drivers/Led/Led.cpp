//
// Created by curiosul on 11/20/18.
//

#include "Led.h"

Led::Led(uint8_t _gpio_pin) : GpioBase(_gpio_pin)
{
	GpioBase::SetMode(PinMode::OUTPUT);
	this->Off();
}

Led::Led(uint8_t _gpio_pin, uint64_t AutoTurnOff) : GpioBase(_gpio_pin)
{
	GpioBase::SetMode(PinMode::OUTPUT);
	this->AutoTurnOffInterval = AutoTurnOff;
	this->Off();
}

void Led::SetReversedPolarity(bool reversed)
{
	this->IsReversed = reversed;
}

void Led::On()
{
	this->LedOnTimestamp = TimeUtils::millis();
	
	if(this->IsReversed)
		GpioBase::Write(LogicalLevel::LOW);
	else
		GpioBase::Write(LogicalLevel::HIGH);
	
	State = LedState::ON;
}

void Led::Off()
{
	if(this->IsReversed)
		GpioBase::Write(LogicalLevel::HIGH);
	else
		GpioBase::Write(LogicalLevel::LOW);
	
	State = LedState::OFF;
}

void Led::Tick()
{
	if(this->State == LedState::OFF)
		return;
	
	if(TimeUtils::millis() - LedOnTimestamp > AutoTurnOffInterval)
	{
		this->Off();
	}
}
