#ifndef _SOCKETCAN_H
#define _SOCKETCAN_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>
#include <bits/ioctls.h>
#include <linux/can.h>



/* Recv buffer */
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T> class SharedQueue
{
public:
	SharedQueue()= default;
	~SharedQueue()= default;
	
	T &front()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while( queue_.empty() )
		{
			cond_.wait(mlock);
		}
		return queue_.front();
	}
	
	void pop_front()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while( queue_.empty() )
		{
			cond_.wait(mlock);
		}
		queue_.pop_front();
	}
	
	void push_back(const T &item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push_back(item);
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
	
	void push_back(T &&item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push_back(std::move(item));
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
	
	int size()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		int size = queue_.size();
		mlock.unlock();
		return size;
	}
	
	bool empty();

private:
	std::deque<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

class SocketCAN
{
public:
	explicit SocketCAN(const char *if_name);
	~SocketCAN() = default;
	
	/* read/write methods */
	ssize_t WriteFrame(can_frame *SendFrame);
	can_frame ReadFrame();
	
	int IsAvailable();
	void Reconnect();
private:
	/* Socket methods */
	int sockfd;
	struct sockaddr_can addr;
	struct ifreq ifr;
	const char *if_name;
	
	/* Store state of can interface */
	bool IsConnected = true;
	
	/* Store received frames here */
	SharedQueue<can_frame> RecvBuffer;
	
	/* Thread used to listen for incoming frames */
	std::thread ListenerThread;
	void InitSocket(const char *if_name);
	void FramesListener();
};

#endif //_SOCKETCAN_H
