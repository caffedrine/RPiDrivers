//
// Created by curiosul on 11/15/18.
//

#ifndef RPIFIRMWARE_CUTTERDC_H
#define RPIFIRMWARE_CUTTERDC_H

#include "Config.h"
#include "drivers/DcMotor.h"

class CutterDC : public DcMotor
{
public:
	CutterDC() : DcMotor(MOTOR_CUTTER_PWM_GPIO, MOTOR_CUTTER_DIR_GPIO)
	{
		DcMotor::PwmConfig(DC_MOTORS_FREQ_HZ, DC_CUTTER_MOTOR_SPEED_CUT);
	}
private:
};

CutterDC g_CutterDC;

#endif //RPIFIRMWARE_CUTTERDC_H
