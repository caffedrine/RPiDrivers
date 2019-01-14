//
// Created by curiosul on 18.12.18.
//

#ifndef RPIFIRMWARE_STEPPER_H
#define RPIFIRMWARE_STEPPER_H

#include "Config.h"
#include "drivers/Stepper.h"

class StepperMotor : public Stepper
{
public:
	StepperMotor() : Stepper(STEPPER_PWM_GPIO, STEPPER_DIR_GPIO)
	{
		Stepper::PwmConfig(STEPPER_FREQ_HZ, 10);
	}
	
	void OnStepsDone() override
	{
		StepperOnStepsDone();
	}
protected:
	virtual void StepperOnStepsDone()
	{
	
	}
};


#endif //RPIFIRMWARE_STEPPER_H
