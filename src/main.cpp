#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <drivers/Hal.h>
#include <drivers/I2C/I2C.h>
#include <iostream>

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
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
	}
}

int main()
{
	setup_logger();
	
	logger->debug("This is a debug messge!");
	logger->info("Started!");
	try
	{
		I2cMaster i2c("/dev/i2c-0");
		i2c.StartSession(0x1234);
		uint8_t payload[] = {0x61, 0x62, 0x63, 0x64};
		i2c.Write(payload, sizeof(payload) / sizeof(uint8_t));
	}
	catch(Exception &e)
	{
		logger->error(e.what());
	}
}