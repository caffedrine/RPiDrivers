//
// Created by curiosul on 1/14/19.
//


#include "I2C.h"

I2cMaster::I2cMaster(const char *DeviceName)
{
	if ((hFile = open(DeviceName, O_RDWR)) < 0)
	{
		throw Exception("open()", strerror(errno));
	}
}

void I2cMaster::Close()
{
	close(hFile);
}

I2cMaster::~I2cMaster()
{
	this->Close();
}
