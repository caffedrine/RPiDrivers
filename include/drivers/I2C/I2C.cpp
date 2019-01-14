//
// Created by curiosul on 1/14/19.
//

#include "I2C.h"

I2cMaster::I2cMaster(const char *DeviceName)
{
	if( (hFile = open(DeviceName, O_RDWR)) < 0 )
	{
		throw Exception("open", strerror(errno));
	}
}

I2cMaster::~I2cMaster()
{
	if( hFile < 0 )
		close(hFile);
}

void I2cMaster::StartSession(int SlaveAddress)
{
	this->StopSession();
	if( ioctl(hFile, I2C_SLAVE, SlaveAddress) < 0 )
	{
		throw Exception("ioctl", strerror(errno));
	}
}

void I2cMaster::StopSession()
{

}

int I2cMaster::Write(uint8_t *data, uint16_t len)
{
	int write_bytes;
	write_bytes = (int) write(hFile, data, len);
	
	if( write_bytes != len )
	{
		throw Exception("write", strerror(errno));
	}
	return write_bytes;
}

int I2cMaster::Read(uint8_t *recv_buffer, uint16_t max_len)
{
	int recv_bytes;
	
	recv_bytes = (int) read(hFile, recv_buffer, max_len);
	if( recv_bytes <= 0 )
	{
		throw Exception("read", strerror(errno));
	}
	
	return (uint16_t) recv_bytes;
}
