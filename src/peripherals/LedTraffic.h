//
// Created by curiosul on 11/20/18.
//

#ifndef RPIFIRMWARE_LEDTRAFFIC_H
#define RPIFIRMWARE_LEDTRAFFIC_H

#include "Config.h"
#include "utils/time_utils.h"
#include <drivers/Led.h>

class LedTraffic : public Led
{
public:
	LedTraffic() : Led(LED_TRAFFIC_GPIO, 100)
	{
	}
	
private:
};

LedTraffic g_LedTraffic;

#endif //RPIFIRMWARE_LEDTRAFFIC_H
