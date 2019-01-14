#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <drivers/Hal.h>
#include <drivers/I2C.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

int main()
{

}