#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>

#include <drivers/Hal.h>
#include <drivers/I2C/I2C.h>
#include <utils/time_utils.h>

/* Logger instance - multi-threaded */
spdlog::logger *logger;
void setup_logger()
{
	try
	{
		auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
		auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs.log", 1024*1024*20, 10);
		
		logger = new spdlog::logger("Logger", {stdout_sink, rotating_sink});
		logger->set_level(spdlog::level::debug);
		
		logger->info("---Application logs enabled---");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
	}
}

class m24512 : private I2cMaster
{
public:
	m24512(const char *I2cDev, uint8_t DeviceAddress) : I2cMaster(I2cDev), _DeviceAddress(DeviceAddress)
	{
		this->SetI2cDeviceAddress(_DeviceAddress);
	}
	
	
	void SetI2cDeviceAddress(uint8_t I2cDeviceAddress)
	{
		this->_DeviceAddress = I2cDeviceAddress;
		I2cMaster::StartSession(this->_DeviceAddress);
	}
	
	int ReadRegister(uint8_t *recv_buffer, uint16_t max_len, uint16_t reg_addr)
	{
		/* read data back */
		int ReadBytes = I2cMaster::Read(recv_buffer, 8, reg_addr);
		logger->info("READ({0} bytes, addr 0x{1:x}): {2}", ReadBytes, reg_addr, to_hex(recv_buffer, ReadBytes));
	}
	
	int WriteRegister(uint8_t *data, uint16_t len,  uint16_t reg_addr)
	{
		int WriteBytes = I2cMaster::Write(data, len, reg_addr);
		logger->info("WRITE ({0} bytes, addr: 0x{1:x}): {2}", WriteBytes, reg_addr, to_hex(data, WriteBytes) );
	}
	
	static std::string to_hex(const uint8_t *bytes, int len)
	{
		std::ostringstream ss;
		for(int i = 0; i < len; i++)
			ss << "0x" << std::hex << (int)bytes[i] << " ";
		return ss.str();
	}
private:
	uint8_t _DeviceAddress;
};


int main()
{
	setup_logger();
	
	m24512 eeprom("/dev/i2c-1", 0x50);
	logger->info("Connected to EEPROM I2C on address 0x50");
	
	uint16_t reg_addr = 0x10;
	
	while(true)
	{
		try
		{
			/* Write */
			uint8_t payload[] = {0b01000000, 0x62, 0x63, 0x64};
			uint16_t payload_len = (uint16_t)sizeof(payload) / sizeof(uint8_t);
			eeprom.WriteRegister(payload, payload_len, reg_addr);
		
			/* Read */
			uint8_t recvBuffer[8];
			eeprom.ReadRegister(recvBuffer, 8, reg_addr);
		}
		catch( Exception &e )
		{
			logger->error(e.what());
		}
		
		TimeUtils::SleepMs(1000);
	}
}
