//
// Created by curiosul on 11/20/18.
//

#include "Relay.h"

Relay::Relay(uint8_t _gpio_pin) : GpioBase(_gpio_pin)
{
	GpioBase::SetMode(PinMode::OUTPUT);
	GpioBase::SetPullState(PullState::DOWN);
	
	this->OpenCircuit();
}

void Relay::SetReversedPolarity(bool reversed)
{
	this->IsReversed = reversed;
}

void Relay::CloseCircuit()
{
	if(!IsReversed)
		GpioBase::Write(LogicalLevel::HIGH);
	else
		GpioBase::Write(LogicalLevel::LOW);
}

void Relay::OpenCircuit()
{
	if(!IsReversed)
		GpioBase::Write(LogicalLevel::LOW);
	else
		GpioBase::Write(LogicalLevel::HIGH);
}
