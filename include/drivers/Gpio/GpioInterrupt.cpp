#include "GpioInterrupt.h"

GpioInterrupt::GpioInterrupt(int GpioPin) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->Init();
}

GpioInterrupt::GpioInterrupt(int GpioPin, int DebounceTimeMicroseconds) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->DebounceTimeUs = DebounceTimeMicroseconds;
	this->Init();
}

GpioInterrupt::~GpioInterrupt()
{

}

void GpioInterrupt::Init()
{
	/* Init GPIO pins */
	Vfb_SetGpioCallbackFunc(this->Pin, &GpioInterrupt::static_internal_gpio_callback, this );
}

void GpioInterrupt::SetReversedPolarity(bool reversed)
{
	this->ReversedPolarity = reversed;
}

void GpioInterrupt::SetStateChangedCallback(level_changed_cb_t f)
{
	this->LevelChangedCbFunc = f;
}

void GpioInterrupt::static_internal_gpio_callback(int pin, int level, uint32_t tick, void* userdata)
{
	reinterpret_cast<GpioInterrupt*>(userdata)->internal_gpio_callback(pin, level, tick);
}

void GpioInterrupt::internal_gpio_callback(int pin, int NewLevel, uint32_t CurrentTicks)
{
	if( NewLevel == 2  || pin != this->Pin)
		return;
	
	static uint32_t LastTicks = 0;
	
	if(  (LogicalLevel)NewLevel != CurrentState )
	{
		if(CurrentTicks - LastTicks >= this->DebounceTimeUs)
		{
			PreviousState = CurrentState;
			CurrentState = (LogicalLevel)NewLevel;
			
			if(this->LevelChangedCbFunc > 0)
				this->LevelChangedCbFunc(CurrentState);
			
			onGpioStateChanged(CurrentState);
			
			LastTicks = CurrentTicks;
		}
	}
}

void GpioInterrupt::onGpioStateChanged(LogicalLevel newLevel)
{

}





