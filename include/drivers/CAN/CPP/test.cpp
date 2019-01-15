#ifdef SOCKET_CAN_TEST

#include <cstdio>
#include <iostream>
#include <cstring>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>
#include <bits/ioctls.h>
#include <unistd.h>

#include <linux/can.h>
#include <linux/spi/spidev.h>
#include <linux/can/raw.h>
#include <thread>

#define CAN_INTERFACE   "can0"

#include "SocketCAN.h"

using namespace std;

int main()
{
	SocketCAN can("can0");
	
	struct can_frame recvFrame, sendFrame;
	ssize_t recvBytes, sendBytes;
	int i = 0;
	
	bool _continue = true;
	while( true )
	{
		if( can.IsAvailable() > 0 )
		{
			recvFrame = can.ReadFrame();
			cout << (int) i++ << ". RECV: 0x" << std::hex << recvFrame.can_id << " # 0x" << std::hex << ((int) recvFrame.can_dlc) << " # " << std::hex
				 << recvFrame.data << endl;
			
			sendFrame = recvFrame;
			sendBytes = can.WriteFrame(&sendFrame);
			cout << i++ << ". SEND: 0x" << std::hex << recvFrame.can_id << " # 0x" << std::hex << ((int) recvFrame.can_dlc) << " # " << std::hex
				 << recvFrame.data << endl;
		}
		
		/** ************************************** **/
		this_thread::sleep_for(chrono::milliseconds(1));
		if(!_continue) break;
	}
	
	return 0;
}

#endif