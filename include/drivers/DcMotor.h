//
// Created by curiosul on 11/20/18.
//

#ifndef _DCMOTOR_H
#define _DCMOTOR_H

#include <stdint.h>
#include "hal.h"

enum class MotorDcDirection
{
	FORWARD = 1,
	BACKWARD= 0
};

enum class MotorDcState
{
	STOPPED = 0,
	RUNNING = 1
};

class DcMotor
{
public:
	MotorDcState CurrentState;
	MotorDcDirection CurrentDirection;
	
	DcMotor(uint8_t PulsePin);
	DcMotor(uint8_t PulsePin, uint8_t Direction);
	DcMotor(uint8_t PulsePin, uint8_t Direction, uint8_t Enable);
	
	void Run();
	void Stop();
	void Enable();
	void Disable();
	void SetSpeed(uint8_t speed);
	void SetDirection(MotorDcDirection new_direction);
	void ReverseDirection();
	void SetEnableReversed(bool enb_reversed);
	
	bool IsRunning();
	long PwmConfig(unsigned FrequencyHz, uint8_t u8Duty);
	
private:
	void Init();
	bool EnbReversed = false;
	unsigned PwmFreqHz = 1000;
	uint8_t PwmDuty = 128;	// 50% default pwm duty
	uint8_t GpioPulse = 0, GpioDirection = 0, GpioEnable = 0;
};

#endif //_DCMOTOR_H
