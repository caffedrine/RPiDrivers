#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <drivers/Hal.h>
#include <drivers/I2C/I2C.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

int main()
{
	I2cMaster i2c("/dev/i2c-0");
	i2c.StartSession(0x1234);
	uint8_t payload[] = {0x61, 0x62, 0x63, 0x64};
	i2c.Write( payload, sizeof(payload)/sizeof(uint8_t));
	
	console->info("Hello world!");
}