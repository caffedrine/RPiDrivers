#include <linux/can.h>
#include <linux/spi/spidev.h>
#include <linux/can/raw.h>
#include <thread>
#include <stdexcept>

#include "SocketCAN.h"

SocketCAN::SocketCAN(const char *_if_name)
{
	this->if_name = _if_name;
	this->InitSocket(this->if_name);
}

void SocketCAN::InitSocket(const char *if_name)
{
	if( (sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 )
	{
		this->IsConnected = false;
		throw Exception("SocketCAN::InitSocket", strerror(errno));
	}
	
	/* Check whether interface exists or not */
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex)
	{
		this->IsConnected = false;
		perror("if_nametoindex");
		throw std::exception_ptr();
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if( bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))  == -1)
	{
		this->IsConnected = false;
		throw Exception("SocketCAN::bind", strerror(errno));

	}
	
	this->IsConnected = true;
	
	/* Launch listener thread */
	this->ListenerThread = std::thread([this](){ FramesListener(); });
	this->ListenerThread.detach();
}

int SocketCAN::IsAvailable()
{
	return RecvBuffer.size();
}

ssize_t SocketCAN::WriteFrame(can_frame *SendFrame)
{
	ssize_t sendBytes = write(this->sockfd, SendFrame, sizeof(struct can_frame));
	if( sendBytes < 0 )
	{
		this->IsConnected  = false;
		throw Exception("SocketCAN::write", strerror(errno));
	}
	if( sendBytes < sizeof(struct can_frame) )
	{
		throw Exception("SocketCAN::write", strerror(errno));
	}
	return sendBytes;
	//cout <<  i++ << ". SEND: 0x" << std::hex << recvFrame.can_id << " # 0x" << std::hex << ((int)recvFrame.can_dlc) << " # " << std::hex << recvFrame.data << endl;
}

can_frame SocketCAN::ReadFrame()
{
	can_frame f  = RecvBuffer.front();
	RecvBuffer.pop_front();
	return f;
}

void SocketCAN::Reconnect()
{
	this->InitSocket(this->if_name);
}

void SocketCAN::FramesListener()
{
	struct can_frame RecvFrame;
	ssize_t recvBytes;
	while( true )
	{
		recvBytes = read(this->sockfd, &RecvFrame, sizeof(struct can_frame));
		if( recvBytes < 0 )
		{
			this->IsConnected = false;
			throw Exception("SocketCAN::read", strerror(errno));
		}
		if( recvBytes < sizeof(struct can_frame) )
		{
			throw Exception("SocketCAN::read()", strerror(errno));
		}
		this->RecvBuffer.push_back(RecvFrame);
		//cout <<  i++ << ". RECV: 0x" << std::hex << recvFrame.can_id << " # 0x" <<  std::hex << ((int)recvFrame.can_dlc) << " # " << std::hex << recvFrame.data << endl;
		
		//std::this_thread::sleep_for( std::chrono::microseconds(500) );
	}
}

