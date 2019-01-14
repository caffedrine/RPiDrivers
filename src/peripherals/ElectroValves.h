//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_ELECTROVALVES_H
#define RPIFIRMWARE_ELECTROVALVES_H

#include "Config.h"
#include "drivers/Relay.h"

class ElectroValves : private Relay
{
public:
	ElectroValves() : Relay(RELAY_ELECTROVALVES_GPIO)
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

ElectroValves g_ElectroValves;

#endif //RPIFIRMWARE_ELECTROVALVES_H
