#include <utility>

//
// Created by curiosul on 10/10/18.
//

#ifndef _TCPSERVERASYNC_H
#define _TCPSERVERASYNC_H

#include <stdio.h>
#include <string.h> 	//strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 	//close
#include <arpa/inet.h> 	//close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>	//FD_SET, FD_ISSET, FD_ZERO macros
#include <thread>
#include "Exception.h"

class TcpServerAsync
{
public:
	typedef enum Status
	{
		STOPPED,
		STARTED
	}status_t;
	
	typedef struct Client
	{
		int Fd = -1;
		uint16_t Port = 0;
		char Ip[16] = {'\0'};
		struct sockaddr_in address;
		
		void Disconnect()
		{
			close(Fd);
			Fd = -1;
		}
		
		bool IsConnected()
		{
			return (Fd > 0);
		}
		
	}client_t;
	
	static const uint16_t RECV_BUFFER_SIZE = 256;
	
	TcpServerAsync(uint16_t port, uint16_t max_clients);
	~TcpServerAsync();
	
	void Start();
	void Stop();

protected:
	int Write(const client_t *client, const char *data, int len);
	virtual void ClientConnected(client_t *client);
	virtual void ClientDisconnected(const client_t *client);
	virtual void DataReceived(const client_t *client, char *data, int len);
	virtual void DataSend(const client_t *client, const char *data, int bytesSend);
	
private:
	client_t *ClientsArray;
	int  ServerSocket, NewSocket;
	uint16_t ServerPort, MaxClients;
	status_t Status = STOPPED;
	
	//set of socket descriptors
	fd_set readfds;
	int max_sd, activity;
	
	ssize_t Readbytes = 0;
	char RecvBuffer[RECV_BUFFER_SIZE] = {'\0'};
	
	std::thread BackgroundWorker;
	void BackgroundWork();
};


#endif //_TCPSERVERASYNC_H
