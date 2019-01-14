#ifndef _PUSHBUTTON_H
#define _PUSHBUTTON_H

#include <iostream>
#include <cstdint>
#include "hal.h"
#include "GpioPooling.h"

enum class PushButtonState
{
    DOWN = 1,
    UP = 0
};

typedef void (*state_changed_cb_t)(PushButtonState);

class PushButton : public GpioPooling
{
public:
    int GpioPin;
    bool ReversedPolarity = false;
    PushButtonState CurrentState = PushButtonState::UP, PreviousState = PushButtonState::UP;
	
    PushButton(int GpioPin);
    PushButton(int GpioPin, int DebounceTimeMillis );
    ~PushButton();
    
    PushButtonState ReadState();
    void SetReversedPolarity(bool reveresed);
    void SetStateChangedCallback( state_changed_cb_t );
    
    virtual void OnStateChanged(PushButtonState state);
    
private:
	/* State changed callback function*/
	state_changed_cb_t StateChangedCbFunc = NULL;
	void onGpioStateChanged(LogicalLevel newState) override;
	
	void Init();
	inline PushButtonState LogicalLevel2State(LogicalLevel logical_level);
};

#endif // _PUSHBUTTON_H
