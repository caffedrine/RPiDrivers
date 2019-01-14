//
// Created by curiosul on 08.12.18.
//

#include "GpioPooling.h"

GpioPooling::GpioPooling(int GpioPin) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->Init();
}

GpioPooling::GpioPooling(int GpioPin, int DebounceTimeMillis) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->DebounceTimeMs = DebounceTimeMillis;
	this->Init();
}

GpioPooling::~GpioPooling()
{

}

void GpioPooling::Init()
{
}

void GpioPooling::Tick()
{
	LogicalLevel CurrentRead = GpioBase::Read();
	
	if(CurrentRead != this->CurrentState)
	{
		if( timer.ElapsedMs() >= this->DebounceTimeMs )
		{
			this->PreviousState = this->CurrentState;
			this->CurrentState = CurrentRead;
			
			if( this->LevelChangedCbFunc > 0 )
				this->LevelChangedCbFunc(this->CurrentState);
			
			onGpioStateChanged(this->CurrentState);
		
			this->timer.Restart();
		}
	}
	else
	{
		this->timer.Restart();
	}
	
}

void GpioPooling::SetStateChangedCallback(level_changed_cb_t f)
{
	this->LevelChangedCbFunc = f;
}

void GpioPooling::onGpioStateChanged(LogicalLevel newState)
{

}
