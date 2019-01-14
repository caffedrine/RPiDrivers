#ifndef _GPIO_H
#define _GPIO_H

#include <iostream>
#include "Hal.h"
#include "GpioBase.h"

typedef void (*level_changed_cb_t)(LogicalLevel);

/**
 * Not to be used with mechanical input components which are noisy and needs debounce.
 * Used as it's best when input signal does not need software debouncing.
 */

class GpioInterrupt : public GpioBase
{
public:
	int Pin;
	bool ReversedPolarity = false;
	int DebounceTimeUs = 0;
	LogicalLevel CurrentState, PreviousState;
	
	explicit GpioInterrupt(int GpioPin);
	GpioInterrupt(int GpioPin, int DebounceTimeMicroseconds);
	~GpioInterrupt();
	
	void SetReversedPolarity(bool reversed);
	void SetStateChangedCallback( level_changed_cb_t );
	
private:
	/* State changed callback function*/
	level_changed_cb_t  LevelChangedCbFunc = NULL;
	
	/* Used to make callback function available inside class*/
	static void static_internal_gpio_callback(int pin, int level, uint32_t tick, void* userdata);
	void internal_gpio_callback(int pin, int NewLevel, uint32_t CurrentTick);
	
	void Init();

protected:
	virtual void onGpioStateChanged(LogicalLevel newState);
};


#endif //_GPIO_H
