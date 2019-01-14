//
// Created by curiosul on 08.12.18.
//

#include "GpioBase.h"

GpioBase::GpioBase(int gpio_pin)
{
	this->Pin = gpio_pin;
	this->Init();
}

LogicalLevel GpioBase::Read()
{
	return (LogicalLevel)Vfb_ReadGpio(this->Pin);
}

void GpioBase::Write(LogicalLevel new_level)
{
	Vfb_WriteGpio(this->Pin, new_level);
}

void GpioBase::SetMode(PinMode new_mode)
{
	Vfb_SetPinMode(this->Pin, new_mode);
}

void GpioBase::SetPullState(PullState new_pull_state)
{
	Vfb_SetPullUpDown(this->Pin, new_pull_state);
}

void GpioBase::Init()
{
	/* Init GPIO pins */
	Vfb_GpioInitialise();
	
	/* Default pin mode */
	Vfb_SetPinMode(this->Pin, PinMode::INPUT);
	Vfb_SetPullUpDown(this->Pin, PullState::UP);
}

