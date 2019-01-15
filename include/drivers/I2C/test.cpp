#ifdef I2C_TEST

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

std::string to_hex(const uint8_t *bytes, int len)
{
	std::ostringstream ss;
	for(int i = 0; i < len; i++)
		ss << "0x" << std::hex << (int)bytes[i] << " ";
	return ss.str();
}

int main()
{
	setup_logger();
	
	logger->info("Opening i2C device...");
	I2cMaster i2c("/dev/i2c-1");
	logger->info("Starting session...");
	i2c.StartSession(0x50);
	uint16_t reg_addr = 0x10;
	
	while(true)
	{
		try
		{
			uint8_t payload[] = {0b01000000, 0x62, 0x63, 0x64};
			uint16_t payload_len = (uint16_t)sizeof(payload) / sizeof(uint8_t);
			int WriteBytes = i2c.Write(payload, payload_len, reg_addr);
			logger->info("SEND ({0} bytes): {1}", WriteBytes, to_hex(payload, WriteBytes) );
			
			/* read data back */
			uint8_t recvBuffer[8];
			int ReadBytes = i2c.Read(recvBuffer, 8, reg_addr);
			logger->info("READ({0} bytes): {1}", ReadBytes, to_hex(recvBuffer, ReadBytes));
		}
		catch( Exception &e )
		{
			logger->error(e.what());
		}
		
		TimeUtils::SleepMs(1000);
	}
}

#endif