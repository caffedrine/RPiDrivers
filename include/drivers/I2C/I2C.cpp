//
// Created by curiosul on 1/14/19.
//

#include "I2C.h"

I2cMaster::I2cMaster(const char *DeviceName)
{
	if( (fd = open(DeviceName, O_RDWR)) < 0 )
	{
		throw Exception("I2C::open(\"" + std::string(DeviceName) + "\")", strerror(errno));
	}
}

I2cMaster::~I2cMaster()
{
	if( fd < 0 )
		close(fd);
}

void I2cMaster::StartSession(int SlaveAddress)
{
	this->StopSession();
	if( ioctl(fd, I2C_SLAVE, SlaveAddress) < 0 )
	{
		std::ostringstream err_func;
		err_func << "I2C::ioctl(\"" << fd << "\", I2C_SLAVE, " << std::hex << "0x" << SlaveAddress << ")";
		throw Exception(err_func.str(), strerror(errno));
	}
	
	this->SlaveAddr = SlaveAddress;
}

void I2cMaster::StopSession()
{

}

int I2cMaster::Write(uint8_t *data, uint16_t len, uint16_t WriteAddress)
{
	/* Select registers area to write to */
	this->SelectRegisterAddress(WriteAddress);
	
	int write_bytes;
	/* Write payload */
	write_bytes = (int) write(fd, data, len);
	if( write_bytes != len )
	{
		std::ostringstream err_func;
		err_func << "I2C::write(*data, " << "len=" << len << ")";
		throw Exception(err_func.str(), strerror(errno));
	}
	return write_bytes;
}

int I2cMaster::Read(uint8_t *recv_buffer, uint16_t max_len, uint16_t reg_addr)
{
	/* Select register address to read from */
	SelectRegisterAddress(reg_addr);
	
	int recv_bytes;
	recv_bytes = (int) read(fd, recv_buffer, max_len);
	if( recv_bytes <= 0 )
	{
		std::ostringstream err_func;
		err_func << "I2C::read(*data, " << "max_len=" << max_len << ")";
		throw Exception(err_func.str(), strerror(errno));
	}
	
	return (uint16_t) recv_bytes;
}

int I2cMaster::SelectRegisterAddress(uint16_t RegAddr)
{
	/* Split address */
	uint8_t MemMsb = (uint8_t)(RegAddr >> 8);
	uint8_t MemLsb = (uint8_t)(RegAddr & 0xFF);
	
	/* Select memory address to write to */
	if( (write(fd, &MemMsb, 1) != 1) || (write(fd, &MemLsb, 1) != 1) )
	{
		std::ostringstream err_func;
		err_func << "I2C::SelectRegisterAddress(\"0x" << std::hex << RegAddr << "\")";

		throw Exception(err_func.str(), strerror(errno));
	}

	return 1;
}

