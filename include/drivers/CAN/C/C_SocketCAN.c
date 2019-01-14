#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>
#include <bits/ioctls.h>
#include <linux/can.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include "C_SocketCAN.h"

#define EXPORT __attribute__((visibility("default")))

EXPORT int CAN_Open(const char *if_name)
{
	int sockfd;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	if( (sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 )
	{
		perror("socket");
		return -1;
	}
	
	/* Check whether interface exists or not */
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if( !ifr.ifr_ifindex )
	{
		perror("if_nametoindex");
		return -1;
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if( bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1 )
	{
		perror("binding");
		return -1;
	}
	return sockfd;
}

EXPORT int CAN_ReadFrame(int sockfd, struct can_frame *RecvFrame)
{
	ssize_t recvBytes = read(sockfd, RecvFrame, sizeof(struct can_frame));
	if( recvBytes < 0 )
	{
		perror("can raw socket read");
		return -1;
	}
	if( recvBytes < sizeof(struct can_frame) )
	{
		fprintf(stderr, "read: incomplete CAN frame\n");
		return -1;
	}
}

EXPORT int CAN_WriteFrame(int sockfd, struct can_frame *SendFrame)
{
	ssize_t sendBytes = write(sockfd, SendFrame, sizeof(struct can_frame));
	if( sendBytes < 0 )
	{
		perror("can raw socket send");
		return -1;
	}
	if( sendBytes < sizeof(struct can_frame) )
	{
		fprintf(stderr, "send: incomplete can frame\n");
		return -1;
	}
	return (int) sendBytes;
}

EXPORT int CAN_IsAvailable(int sockfd)
{
	fd_set sockset;
	FD_ZERO(&sockset);
	FD_SET(sockfd, &sockset);
	int result = select(sockfd + 1, &sockset, NULL, NULL, NULL);
	if (result == 1)
	{
		// The socket has data. For good measure, it's not a bad idea to test further
		if (FD_ISSET(sockfd, &sockset) )
		{
			///TODO: Investigate
//			int availableBytes = 0;
//			ioctl(sockfd, FIONREAD, &availableBytes);
//			if( availableBytes < sizeof(struct can_frame) )
//				return 0;
//			return (availableBytes / sizeof(struct can_frame));
			return 1;
		}
	}
	return 0;
}

EXPORT void CAN_Close(int sockfd)
{
	close(sockfd);
}

EXPORT int CAN_SetSocketBlockingEnabled(int fd, int blocking)
{
	if (fd < 0) return -1;

	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) return -1;
	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(fd, F_SETFL, flags) == 0) ? 0 : -1;
}

