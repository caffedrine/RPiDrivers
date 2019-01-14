#ifndef DRIVERSCONTAINER_COMMON_H
#define DRIVERSCONTAINER_COMMON_H

#include <mutex>
#include <condition_variable>

#include "utils/time_utils.h"
#include "Config.h"
#include "../../Shared/packet.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

/* Global variable to store latest received ACK from client */
uint64_t g_TcpRecvLastMillis = 0;

/* General time used in waiting States */
TimeUtils::Timer g_WaitTimer;

/* Program states */
class States
{
public:
	enum
	{
		STANDBY,
		WAIT_RESET,
		WAIT_MOVETO,
		WAIT_CUT,
		WAIT_CUTTER_INIT,
		WAIT_LOCK,
		WAIT_UNLOCK,
		EMERGENCY_STOP,
		
		STATES_NO
	};
	
	typedef struct State
	{
		int Val;
		const char *description;
	}state_t;
	
	/* Store current and previous state */
	state_t Current, Previous;
	/* Description of each state */
	const char *states_description[STATES_NO] =
			{
					states_description[States::STANDBY] 		= "[STANDBY] Waiting for command",
					states_description[States::WAIT_RESET] 		= "[WAIT_RESET] Waiting for motors reinitialization",
					states_description[States::WAIT_MOVETO] 	= "[WAIT_MOVETO] Moving to target position",
					states_description[States::WAIT_CUT] 		= "[WAIT_CUT] Waiting to cut the curtain",
					states_description[States::WAIT_CUTTER_INIT]= "[WAIT_CUTTER_INIT] Waiting for cutter to come back",
					states_description[States::WAIT_LOCK] 		= "[WAIT_LOCK] Waiting electrovalves to lock curtain",
					states_description[States::WAIT_UNLOCK] 	= "[WAIT_UNLOCK] Wait for electrovalves to unlock curtain",
					states_description[States::EMERGENCY_STOP] 	= "[EMERGENCY_STOP] Emergency stop triggered",
			};
	
	States()
	{
		Current.Val = 0;
		Previous.Val = 0;
		Set(States::STANDBY);
	}
	
	void Set(int new_state)
	{
		/* Reset timer to have timestamp when entered on current state */
		g_WaitTimer.Restart();
		
		Previous.Val = Current.Val;
		Previous.description = states_description[Current.Val];
		
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			Current.Val = new_state;
			Current.description = states_description[Current.Val];
		}
		mlock.unlock();     // unlock before notificiation to minimize mutex context
		cond_.notify_one(); // notify one waiting thread
		
		/* Print current state */
		console->info( states_description[Current.Val] );
	}

private:
	/* Thread safe read/write positions */
	std::mutex mutex_;
	std::condition_variable cond_;
};

States g_State;

/* Flag to inform the user that a reset is required */
bool g_ResetRequired = true;

/* Program state */
bool _ProgramContinue = true;


#endif //DRIVERSCONTAINER_COMMON_H
