//
// Created by curiosul on 1/14/19.
//

#include "I2C.h"

I2cMaster::I2cMaster(const char *DeviceName)
{
	if( (hFile = open(DeviceName, O_RDWR)) < 0 )
	{
		throw Exception("I2C::open(\"" + std::string(DeviceName) + "\")" , strerror(errno));
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
		std::ostringstream err_func;
		err_func << "I2C::ioctl(\"" << hFile << "\", I2C_SLAVE, " << std::hex << "0x" << SlaveAddress << ")";
		throw Exception(err_func.str(), strerror(errno));
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
		std::ostringstream err_func;
		err_func << "I2C::write(*data, " <<  "len=" << len << ")";
		throw Exception(err_func.str(), strerror(errno));
	}
	return write_bytes;
}

int I2cMaster::Read(uint8_t *recv_buffer, uint16_t max_len)
{
	int recv_bytes;
	
	recv_bytes = (int) read(hFile, recv_buffer, max_len);
	if( recv_bytes <= 0 )
	{
		std::ostringstream err_func;
		err_func << "I2C::write(*data, " <<  "max_len=" << max_len << ")";
		throw Exception(err_func.str(), strerror(errno));
	}
	
	return (uint16_t) recv_bytes;
}
