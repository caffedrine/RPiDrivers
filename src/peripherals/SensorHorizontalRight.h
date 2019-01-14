//
// Created by curiosul on 21.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTALRIGHT_H
#define RPIFIRMWARE_SENSORHORIZONTALRIGHT_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorHorizontalRight : public PushButton
{
public:
	explicit SensorHorizontalRight() : PushButton(SENSOR_INIT_HORIZONTAL_RIGHT_GPIO, BUTTONS_DBOUNCING_TIME_MS)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorHorizontalRight g_SensorHorizontalRight;

#endif //RPIFIRMWARE_SENSORHORIZONTALRIGHT_H
