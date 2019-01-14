//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_CUTTERRELAY_H
#define RPIFIRMWARE_CUTTERRELAY_H

#include "Config.h"
#include "drivers/Relay.h"

class Cutter : public Relay
{
public:
	Cutter() : Relay(RELAY_CUTTER_ENB_GPIO)
	{
		Relay::SetPullState(PullState::DOWN);
	}
	
	void On()
	{
		Relay::CloseCircuit();
	}
	
	void Off()
	{
		Relay::OpenCircuit();
	}
private:

};

Cutter g_Cutter;

#endif //RPIFIRMWARE_CUTTERRELAY_H
