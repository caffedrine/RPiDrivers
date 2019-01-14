#include <iostream>
#include "Stepper.h"

Stepper::Stepper(uint8_t Pulse)
{
	this->PulsePin = Pulse;
	this->Init();
}

Stepper::Stepper(uint8_t Pulse, uint8_t Direction)
{
	this->PulsePin = Pulse;
	this->DirectionPin = Direction;
	this->Init();
}

Stepper::Stepper(uint8_t Pulse, uint8_t Direction, uint8_t Enable)
{
	this->PulsePin = Pulse;
	this->DirectionPin = Direction;
	this->EnablePin = Enable;
	this->Init();
}

Stepper::~Stepper()
{
	this->Stop();
}

void Stepper::Init()
{
	/* Init GPIO pins */
	Vfb_GpioInitialise();
	
	Vfb_SetPinMode( this->PulsePin, PinMode::OUTPUT);
	
	if(this->EnablePin > 0)
	{
		Vfb_SetPinMode(this->EnablePin, PinMode::OUTPUT);
		Vfb_WriteGpio(this->EnablePin, LogicalLevel::LOW);
	}
	
	if(this->DirectionPin > 0)
		Vfb_SetPinMode(this->DirectionPin, PinMode::OUTPUT);
	
	/* Feedback function callback */
	Vfb_SetGpioCallbackFunc(this->PulsePin, static_internal_step_callback, this);
}

long Stepper::PwmConfig(unsigned Frequency, uint8_t DutyProcents)
{
	this->PwmFreq = Frequency;
	this->PwmDutyProcents = DutyProcents;
	
	/* Set up pwm frequeny */
	long result = Vfb_InitPwm(this->PulsePin, this->PwmFreq);
	/* Stop motor until next command for safety reasons */
	Vfb_PwmOut(this->PulsePin, 0 );
	
	return result;
}

long Stepper::map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Stepper::Stop()
{
	this->LastStepsToDo = this->StepsToDo;
	this->LastStepsDone = this->StepsDone;
	
	this->StepsToDo = 0;
	this->StepsDone = 0;
	
	if(!this->IsRunning())
		return;
	this->SetState(StepperState::STOPPED);
	Vfb_PwmOut(this->PulsePin, 0);
	
	if(this->EnablePin > 0)
		Vfb_WriteGpio(this->EnablePin, LogicalLevel::LOW);
}

void Stepper::Run()
{
	this->SetState(StepperState::RUNNING);
	this->StepsToDo = 0;
	this->StepsDone = 0;
	Vfb_PwmOut(this->PulsePin, (uint8_t)map(this->PwmDutyProcents, 0, 100, 0, 255) );
	
	if(this->EnablePin > 0)
		Vfb_WriteGpio(this->EnablePin, 1);
}

void Stepper::SetDirection(StepperDirection NewDirection)
{
	if(this->CurrentDirection == NewDirection)
		return;
	
	this->CurrentDirection = NewDirection;
	
	if(this->DirectionPin > 0)
		Vfb_WriteGpio(this->DirectionPin, (LogicalLevel)NewDirection);
}

void Stepper::SetState(StepperState _currentState)
{
	this->LastState = this->CurrentState;
	this->CurrentState = _currentState;
}

bool Stepper::IsRunning()
{
	return !(CurrentState == StepperState::STOPPED);
}

void Stepper::RunSteps(long steps)
{
	this->SetState(StepperState::RUNNING_STEPS);
	this->StepsToDo = steps;
	this->StepsDone = 0;
	Vfb_PwmOut(this->PulsePin, (uint8_t)map(this->PwmDutyProcents, 0, 100, 0, 255) );
	
	if(this->EnablePin > 0)
		Vfb_WriteGpio(this->EnablePin, 1);
}

void Stepper::static_internal_step_callback(int pin, int level, uint32_t tick, void *userdata)
{
	reinterpret_cast<Stepper*>(userdata)->internal_step_callback(pin, level, tick);
}

void Stepper::internal_step_callback(int pin, int NewLevel, uint32_t CurrentTick)
{
	/* Not our interrupt? */
	if( NewLevel == 2  || pin != this->PulsePin)
		return;
	
	/* Count only high pulses */
	if( NewLevel == 0 )
		return;
	
	/* Count only when RunSteps() was called */
	if(this->CurrentState != StepperState::RUNNING_STEPS)
		return;
	
	/* At this point one step was made */
	StepsDone++;
	
//	static uint32_t  last_tick = 0;
//	std::cout << "Step done in: " << (CurrentTick - last_tick) << " us" << std::endl;
//	last_tick = CurrentTick;
	
	if( this->StepsToDo > 0 && this->StepsDone == StepsToDo)
	{
		Vfb_PwmOut(this->PulsePin, 0);
		SetState(StepperState::STOPPED);
		this->LastStepsToDo = this->StepsToDo;
		this->LastStepsDone = this->StepsDone;
		
		OnStepsDone();
	}
}

void Stepper::SetStepsDoneCallback(steps_finished_t f)
{
	this->StepsDoneCb = f;
}

void Stepper::OnStepsDone()
{
	
	
	if(StepsDoneCb > 0)
		StepsDoneCb(this->StepsDone);
}
