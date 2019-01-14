#ifndef _STEPPER_H
#define _STEPPER_H

#include <cstdint>
#include "Hal.h"

enum class StepperDirection
{
	FORWARD = 1,
	BACKWARD= 0
};

enum class StepperState
{
	STOPPED,
	RUNNING,
	RUNNING_STEPS
};

typedef void (*step_cb_t)(void);
typedef void (*steps_finished_t)(long);

class Stepper
{
public:
	Stepper(uint8_t Pulse);
	Stepper(uint8_t Pulse, uint8_t Direction);
	Stepper(uint8_t Pulse, uint8_t Direction, uint8_t Enable);
	~Stepper();
	
	long StepsToDo = 0, StepsDone = 0;
	long LastStepsToDo = 0, LastStepsDone = 0;
	StepperDirection CurrentDirection = StepperDirection ::FORWARD;
	
	void Stop();
	void Run();
	void RunSteps(long steps);
	void SetDirection(StepperDirection NewDirection);
	bool IsRunning();
	long PwmConfig(unsigned Frequency, uint8_t DutyProcents);
	void SetStepsDoneCallback(steps_finished_t);
	virtual void OnStepsDone();
	
private:
	uint8_t PulsePin, DirectionPin = 0, EnablePin = 0;
	unsigned PwmFreq = 1000;
	uint8_t PwmDutyProcents = 10;	// 10% default pwm duty
	
	StepperState CurrentState = StepperState::STOPPED, LastState = StepperState::STOPPED;
	void SetState(StepperState state);
	
	void Init();
	static inline long map(long x, long in_min, long in_max, long out_min, long out_max);
	
	steps_finished_t StepsDoneCb;
	static void static_internal_step_callback(int pin, int level, uint32_t tick, void* userdata);
	void internal_step_callback(int pin, int NewLevel, uint32_t CurrentTick);
};


#endif //_STEPPER_H
