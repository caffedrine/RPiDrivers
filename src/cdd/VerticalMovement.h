//
// Created by curiosul on 11/23/18.
//

#ifndef RPIFIRMWARE_VERTICALMOVEMENT_H
#define RPIFIRMWARE_VERTICALMOVEMENT_H

#include <mutex>
#include <condition_variable>

#include "Globals.h"
#include "drivers/Pid.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"
#include "peripherals/StepperMotor.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"

class VerticalMovement : protected MasterEncoder, protected SlaveEncoder, protected StepperMotor
{
//	 ____  _   _ ____  _     ___ ____
//	|  _ \| | | | __ )| |   |_ _/ ___|
//	| |_) | | | |  _ \| |    | | |
//	|  __/| |_| | |_) | |___ | | |___
//	|_|    \___/|____/|_____|___\____|
public:
	explicit VerticalMovement() : MasterEncoder(), SlaveEncoder(), StepperMotor()
	{
		CurrentPosition = Mm2Steps(VERTICAL_MM_OFFSET);
	}
	
	bool IsRunning()
	{
		return StepperMotor::IsRunning();
	}
	
	float GetCurrPositionMM()
	{
		return Steps2Mm(CurrentPosition);
	}
	
	void MoveToMM(int32_t mm)
	{
		MoveTo(Mm2Steps(mm));
	}
	
	void MoveTo(int32_t new_position)
	{
		if( ((int) (new_position - (int) Mm2Steps(VERTICAL_MM_OFFSET))) < 0 )
		{
			console->warn("Can't go to a negative position: {}", (((int) (new_position - (int) Mm2Steps(VERTICAL_MM_OFFSET)))));
			return;
		}
		
		console->info("[MOVETO] Start moving from {0} ({1}steps) to {2} mm ({3}steps)", CurrentPosition, Steps2Mm(CurrentPosition), Steps2Mm(new_position), new_position);
		
		if( CurrentPosition < new_position )
		{
			StepperMotor::SetDirection(StepperDirection::FORWARD);
			StepperMotor::RunSteps( new_position - CurrentPosition );
		}
		else if( CurrentPosition > new_position )
		{
			StepperMotor::SetDirection(StepperDirection::BACKWARD);
			StepperMotor::RunSteps( CurrentPosition - new_position );
		}
		
		this->CurrentPosition = new_position;
	}
	
	void MoveUp()
	{
		MoveTo(Mm2Steps(VERTICAL_MAX_POSITION_MM));
	}
	
	void MoveDown()
	{
		MoveTo(VERTICAL_MIN_POSITION);
	}
	
	void Stop()
	{
		/* Stop */
		StepperMotor::Stop();
		
		/* Steps completed so far if stop was triggered suddenly */
		auto StepsDoneSoFar = (uint32_t)( StepperMotor::LastStepsToDo - StepperMotor::LastStepsDone);
		
		if(StepperMotor::CurrentDirection == StepperDirection::FORWARD)
			CurrentPosition -= StepsDoneSoFar;
		else if(StepperMotor::CurrentDirection == StepperDirection::BACKWARD)
			CurrentPosition += StepsDoneSoFar;
		
		console->info("[STOPPED] Stepper current position: {0}mm - {1}steps", Steps2Mm(CurrentPosition), CurrentPosition);
		
	}
	
	void Reset()
	{
		if( g_SensorVerticalMaster.CurrentState != PushButtonState::DOWN )
		{
			StepperMotor::SetDirection(StepperDirection::BACKWARD);
			StepperMotor::Run();
		}
		else
		{
			console->info("Already in reset position!");
		}
		this->CurrentPosition = Mm2Steps(VERTICAL_MM_OFFSET);
	}
	
	static float Steps2Mm(int32_t steps)
	{
		if( steps == 0 )
			return 0;
		return ( (float)((float)steps*(float)MILIMETERS_PER_REVOLUTION)/(float)STEPS_PER_REVOLUTION );
	}
	
	static int32_t Mm2Steps(float mm)
	{
		if( mm == 0 )
			return 0;
		return (int32_t)( (mm*STEPS_PER_REVOLUTION)/MILIMETERS_PER_REVOLUTION );
	}

//	 ____  ____  _____     ___  _____ _____
//	|  _ \|  _ \|_ _\ \   / / \|_   _| ____|
//	| |_) | |_) || | \ \ / / _ \ | | |  _|
//	|  __/|  _ < | |  \ V / ___ \| | | |___
//	|_|   |_| \_\___|  \_/_/   \_\_| |_____|
private:
	const int32_t VERTICAL_MIN_POSITION = Mm2Steps(VERTICAL_MM_OFFSET);
	/* Store positions */
	int32_t CurrentPosition;
	
protected:
	void OnMasterEncoderStep() override
	{

	}
	
	void OnSlaveEncoderStep() override
	{

	}
	
	void StepperOnStepsDone() override
	{
		console->info("{0} ({1} mm) steps done. Current position: {2} ({3}mm)", StepperMotor::StepsDone, Steps2Mm((uint32_t)StepperMotor::StepsDone), CurrentPosition, Steps2Mm(CurrentPosition));
	}
};

VerticalMovement g_Vertical;

#endif //RPIFIRMWARE_VERTICALMOVEMENT_H
