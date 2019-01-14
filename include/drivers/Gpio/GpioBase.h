//
// Created by curiosul on 08.12.18.
//

#ifndef RPIFIRMWARE_GPIOBASE_H
#define RPIFIRMWARE_GPIOBASE_H

#include "Hal.h"

class GpioBase
{
public:
	explicit GpioBase(int gpio_pin);
	
	LogicalLevel Read();
	void Write(LogicalLevel new_level);
	void SetMode(PinMode new_mode);
	void SetPullState(PullState new_pull_state);
	
private:
	void Init();
	
protected:
	int Pin;
};


#endif //RPIFIRMWARE_GPIOBASE_H
