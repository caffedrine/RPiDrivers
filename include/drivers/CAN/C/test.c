#ifdef SOCKET_CAN_TEST

#include <stdio.h>
#include <stdbool.h>
#include <linux/can.h>
#include <string.h>

#include "C_SocketCAN.h"

#define CAN_IF	"can0"

int main()
{
	
	int sockfd = CAN_Open(CAN_IF);
	if(sockfd < 0) return 1;
	printf("Open %s: %d\n", CAN_IF, sockfd);
	
	//CAN_SetSocketBlockingEnabled(sockfd, 0);
	
	struct can_frame recvFrame, sendFrame;
	
	int i = 0;
	while( true )
	{
		if( CAN_IsAvailable(sockfd) > 0 )
		{
			CAN_ReadFrame(sockfd, &recvFrame);
			printf("%d. RECV: 0x%02x # 0x%02x # %s \n", i++, recvFrame.can_id, recvFrame.can_dlc, recvFrame.data);

			sendFrame = recvFrame;
			CAN_WriteFrame(sockfd, &sendFrame);
			printf("%d. SEND: 0x%02x # 0x%02x # %s \n", i++, sendFrame.can_id, sendFrame.can_dlc, sendFrame.data);
 		}
	}
	
	return 0;
}

#endif