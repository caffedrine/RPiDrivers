//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
#define RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorVerticalMaster : public PushButton
{
public:
	explicit SensorVerticalMaster() : PushButton(SENSOR_INIT_VERTICAL_MASTER_GPIO, BUTTONS_DBOUNCING_TIME_MS)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorVerticalMaster g_SensorVerticalMaster;



#endif //RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
