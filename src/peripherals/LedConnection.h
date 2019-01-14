//
// Created by curiosul on 11/15/18.
//

#ifndef RPIFIRMWARE_LED_H
#define RPIFIRMWARE_LED_H

#include "Config.h"
#include <drivers/Led.h>

class LedConnection : public Led
{
public:
	LedConnection() : Led(LED_CONNECTION_GPIO)
	{
	
	}
private:
};

LedConnection g_LedConnection;

#endif //RPIFIRMWARE_LED_H
