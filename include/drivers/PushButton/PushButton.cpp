#include "PushButton.h"
#include "Hal.h"

PushButton::PushButton(int _GpioPin) : GpioPooling(_GpioPin)
{
	this->GpioPin = _GpioPin;
	this->Init();
}

PushButton::PushButton(int _GpioPin, int _DebounceTimeMicroseconds) : GpioPooling(_GpioPin, _DebounceTimeMicroseconds)
{
	this->GpioPin = _GpioPin;
	this->Init();
	
	this->ReadState();
}

PushButton::~PushButton()
{

}

void PushButton::Init()
{
	GpioPooling::SetMode(PinMode::INPUT);
	GpioPooling::SetPullState(PullState::DOWN);
	this->ReadState();
}

PushButtonState PushButton::ReadState()
{
	PreviousState = CurrentState;
	CurrentState = this->LogicalLevel2State(GpioPooling::Read());
	
	if( CurrentState != PreviousState )
	{
		/* State changed callback function*/
		if(StateChangedCbFunc > 0)
			StateChangedCbFunc(CurrentState);
	}
	return CurrentState;
}

PushButtonState PushButton::LogicalLevel2State(LogicalLevel logical_level)
{
	if( this->ReversedPolarity == true )
	{
		return (PushButtonState)(!(bool)logical_level);
	}
	else
	{
		return (PushButtonState)((bool)logical_level);
	}
}

void PushButton::SetReversedPolarity(bool reveresed)
{
	this->ReversedPolarity = reveresed;
}

void PushButton::SetStateChangedCallback( state_changed_cb_t f)
{
	StateChangedCbFunc = f;
}

void PushButton::OnStateChanged(PushButtonState new_state)
{
	/* Call callback function*/
	if(this->StateChangedCbFunc > 0)
		StateChangedCbFunc(new_state);
}

void PushButton::onGpioStateChanged(LogicalLevel newState)
{
	this->PreviousState = this->CurrentState;
	this->CurrentState = LogicalLevel2State(newState);
	this->OnStateChanged( this->CurrentState );
}

