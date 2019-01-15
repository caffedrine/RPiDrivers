//
// Created by curiosul on 1/14/19.
//

#ifndef _I2C_H_
#define _I2C_H_

#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#include "Exception/Exception.h"

class I2cMaster
{
public:
	explicit I2cMaster(const char *DeviceName);
	~I2cMaster();
	
	void StartSession(int SlaveAddress);
	void StopSession();
	
	int Write(uint8_t *data, uint16_t len, uint16_t RegAddr);
	int Read(uint8_t *recv_buffer, uint16_t max_len = 255);
	
private:
	int fd;
	int SlaveAddr;
};


#endif //_I2C_H_
