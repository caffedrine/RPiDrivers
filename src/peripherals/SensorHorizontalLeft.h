//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTLNASTER_H
#define RPIFIRMWARE_SENSORHORIZONTLNASTER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorHorizontalLeft : public PushButton
{
public:
	explicit SensorHorizontalLeft() : PushButton(SENSOR_INIT_HORIZONTAL_LEFT_GPIO, BUTTONS_DBOUNCING_TIME_MS)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorHorizontalLeft g_SensorHorizontalLeft;


#endif //RPIFIRMWARE_SENSORHORIZONTLNASTER_H
