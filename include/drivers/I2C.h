//
// Created by curiosul on 1/14/19.
//

#ifndef _I2C_H
#define _I2C_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "Exception.h"

class I2cMaster
{
public:
	explicit I2cMaster(const char *DeviceName);
	~I2cMaster();
	
	void Close();
	
private:
	int hFile = 0;
};


#endif //RPIDRIVERS_I2C_H
