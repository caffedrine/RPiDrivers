#ifndef _C_SOCKETCAN_H
#define _C_SOCKETCAN_H

int CAN_Open(const char *if_name);
int CAN_ReadFrame(int sockfd, struct can_frame *RecvFrame);
int CAN_WriteFrame(int sockfd, struct can_frame *SendFrame);
int CAN_IsAvailable(int sockfd);
void CAN_Close(int sockfd);

int CAN_SetSocketBlockingEnabled(int fd, int blocking);

#endif //_C_SOCKETCAN_H
