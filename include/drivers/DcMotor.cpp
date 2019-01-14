//
// Created by curiosul on 11/20/18.
//

#include "DcMotor.h"

DcMotor::DcMotor(uint8_t Pulse)
{
	this->GpioPulse = Pulse;
	Init();
}

DcMotor::DcMotor(uint8_t Pulse, uint8_t Direction)
{
	this->GpioPulse = Pulse;
	this->GpioDirection = Direction;
	this->Init();
}

DcMotor::DcMotor(uint8_t Pulse, uint8_t Direction, uint8_t Enable)
{
	this->GpioPulse = Pulse;
	this->GpioDirection = Direction;
	this->GpioEnable = Enable;
	this->Init();
}

void DcMotor::Init()
{
	/* Init GPIO pins */
	Vfb_GpioInitialise();
	Vfb_SetPinMode( this->GpioPulse, PinMode::OUTPUT);
	
	if(this->GpioEnable > 0)
	{
		Vfb_SetPinMode(this->GpioEnable, PinMode::OUTPUT);
		this->Disable();
	}
	
	if(this->GpioDirection > 0)
		Vfb_SetPinMode(this->GpioDirection, PinMode::OUTPUT);
	
	/* Default PWM */
	this->PwmConfig(this->PwmFreqHz, this->PwmDuty);
}

bool DcMotor::IsRunning()
{
	return !(CurrentState == MotorDcState ::STOPPED);
}

long DcMotor::PwmConfig(unsigned freq, uint8_t u8Duty)
{
	this->PwmDuty = u8Duty;
	this->PwmFreqHz = freq;
	
	/* Set Freq */
	long result = Vfb_InitPwm(this->GpioPulse, this->PwmFreqHz);
	/* Stop motor until Run() */
	Vfb_PwmOut(this->GpioPulse, 0 );
	this->CurrentState = MotorDcState::STOPPED;
	
	/* return actual freq */
	return  result;
}

void DcMotor::SetSpeed(uint8_t speed)
{
	if(PwmDuty == speed)
		return;
	
	this->PwmDuty = speed;

	if(this->CurrentState == MotorDcState::RUNNING)
		Vfb_PwmOut(this->GpioPulse, this->PwmDuty);
	else
		Vfb_PwmOut(this->GpioPulse, 0);
}

void DcMotor::Stop()
{
	if(!this->IsRunning())
		return;
	
	/* Reverse direction to brake */
//	if(this->GpioDirection > 0)
//		this->SetDirection( (CurrentDirection==MotorDcDirection::FORWARD)?MotorDcDirection::BACKWARD:MotorDcDirection::FORWARD );
	
	this->CurrentState = MotorDcState::STOPPED;
	Vfb_PwmOut(this->GpioPulse, 0);
	
	this->Disable();
	
	/* Reverse direction to initial */
//	if(this->GpioDirection > 0)
//		this->SetDirection( (CurrentDirection==MotorDcDirection::FORWARD)?MotorDcDirection::BACKWARD:MotorDcDirection::FORWARD );
}

void DcMotor::SetDirection(MotorDcDirection new_direction)
{
	if(CurrentDirection == new_direction)
		return;
	
	if(this->GpioDirection > 0)
	{
		CurrentDirection = new_direction;
		if( new_direction == MotorDcDirection::FORWARD )
			Vfb_WriteGpio(this->GpioDirection, LogicalLevel::HIGH);
		else
			Vfb_WriteGpio(this->GpioDirection, LogicalLevel::LOW);
	}
}

void DcMotor::Run()
{
	if(this->IsRunning())
		return;
	
	this->CurrentState = MotorDcState::RUNNING;
	
	/* Provide PWM to the output */
	Vfb_PwmOut(this->GpioPulse, this->PwmDuty);
	
	this->Enable();
}

void DcMotor::Enable()
{
	if(this->GpioEnable > 0)
		Vfb_WriteGpio(this->GpioEnable, (!this->EnbReversed)?LogicalLevel::HIGH:LogicalLevel::LOW);
}

void DcMotor::Disable()
{
	if(this->GpioEnable > 0)
		Vfb_WriteGpio(this->GpioEnable, (this->EnbReversed)?LogicalLevel::HIGH:LogicalLevel::LOW);
}

void DcMotor::SetEnableReversed(bool enb_reversed)
{
	this->EnbReversed = enb_reversed;
}

void DcMotor::ReverseDirection()
{
	if(CurrentDirection == MotorDcDirection::BACKWARD)
	{
		this->SetDirection(MotorDcDirection::FORWARD);
		this->CurrentDirection = MotorDcDirection::FORWARD;
	}
	else
	{
		this->SetDirection(MotorDcDirection::BACKWARD);
		this->CurrentDirection = MotorDcDirection::BACKWARD;
	}
}



