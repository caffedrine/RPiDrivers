#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

//#define PIGPIO

#ifdef PIGPIO
#include <pigpio.h>
#else
typedef void (*gpioAlertFuncEx_t)  (int event, int level, uint32_t tick, void *userdata);
#endif
//////////////////////////////
// PIN Schema: BCM/GPIO
//////////////////////////////

/** **************************************** **/

/** **************************************** **/

#define ERROR_LOG(str)	0

enum class LogicalLevel
{
    LOW = 0,
    HIGH = 1
};

enum class PinMode
{
    OUTPUT = 1,
    INPUT = 0,
    ALT0,
    ALT1,
    ALT2,
    ALT3,
    ALT4
};

enum class PullState
{
    OFF = 0,
    DOWN = 1,
    UP = 2
};

enum ReturnStatus
{
    SUCCES = 0,
    FAIL = -1
};

static inline int _initialise_gpio_interface()
{
#ifdef PIGPIO
//    for(int i = 1337; i <= 1347; i++)
//    {
//        if( gpioCfgSocketPort(i)  == 0 )
//            break;
//    }
    if(gpioInitialise() == PI_INIT_FAILED)
    	ERROR_LOG("Failed to initialise PiGPIO");
    return PI_INIT_FAILED;
#else
    return -1;
#endif
}

static inline int _set_pin_mode(int pin, PinMode mode)
{
#ifdef PIGPIO
    if(gpioSetMode(pin, (int)mode) != 0)
    {
    	//ERROR_LOG("Failed to set " << pin << " as INPUT");
        return -3;
    }
    return 0;
#else
    return -1;
#endif
}

static inline int _read_gpio(int pin)
{
#ifdef PIGPIO
    return gpioRead(pin);
#else
    return -1;
#endif
}

static inline int _write_gpio(int pin, int level)
{
#ifdef PIGPIO
    return gpioWrite(pin, level);
#else
    return -1;
#endif
}

static inline int _write_gpio(int pin, LogicalLevel level)
{
#ifdef PIGPIO
	return gpioWrite(pin, (int)level);
#else
	return -1;
#endif
}

static inline int _gpio_sleep(int microseconds)
{
#ifdef PIGPIO
    return gpioSleep(0, 0, microseconds);
#else
    return -1;
#endif
}

static inline int _set_pull_updown(int gpio, PullState updown)
{
#ifdef PIGPIO
    return gpioSetPullUpDown(gpio, (int)updown);
#else
    return -1;
#endif
}

static inline int _pwm_output(unsigned pin, unsigned u8Duty)
{
#ifdef PIGPIO
    return gpioPWM(pin, u8Duty);
#else
    return FAIL;
#endif
}

static inline int _pwm_init(unsigned pin, unsigned freq)
{
#ifdef PIGPIO
    _pwm_output(pin, 0);
    return gpioSetPWMfrequency(pin, freq);
#else
    return FAIL;
#endif
}

static inline uint32_t _get_micros()
{
#ifdef PIGPIO
	return gpioTick();
#else
	return FAIL;
#endif
}

static inline uint32_t _get_millis()
{
#ifdef PIGPIO
	return (gpioTick()/1000);
#else
	return FAIL;
#endif
}

static inline uint32_t _set_gpio_callback_function(int pin, gpioAlertFuncEx_t cbf, void *context)
{
#ifdef PIGPIO
	gpioSetAlertFuncEx(pin, cbf, context );
#else
	return FAIL;
#endif
}

#define Vfb_GpioInitialise()                _initialise_gpio_interface()
#define Vfb_SetPinMode(pin, mode)          	_set_pin_mode(pin, mode)
#define Vfb_SetPullUpDown(pin, pull)        _set_pull_updown(pin, pull)
#define Vfb_ReadGpio(arg)         			_read_gpio(arg)
#define Vfb_WriteGpio(arg1, arg2)           _write_gpio(arg1, arg2)
#define Vfb_SleepGPIO(arg1)					_gpio_sleep(arg1)

#define Vfb_InitPwm(pin, freq)				_pwm_init(pin, freq)
#define Vfb_PwmOut(pin, u8duty)             _pwm_output(pin, u8duty)

#define Vfb_GetMicros()						_get_micros()
#define Vfb_GetMillis()						_get_millis()

#define Vfb_SetGpioCallbackFunc(pin, cbf, context)	_set_gpio_callback_function(pin, cbf, context)

#endif // _HAL_H
