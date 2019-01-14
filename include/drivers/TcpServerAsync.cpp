//
// Created by curiosul on 10/10/18.
//

#include <iostream>
#include "TcpServerAsync.h"

TcpServerAsync::TcpServerAsync(uint16_t port, uint16_t max_clients)
{
	/* Validate port */
	if( port < 1024 || port > 65535 )
	{
		throw Exception("TcpServerAsync", "The listening port must be in interval [1024-65535]");
	}
	this->ServerPort = port;
	
	/* Initialize clients array */
	this->MaxClients = max_clients;
	ClientsArray = new client_t[this->MaxClients];
	for( int i = 0; i < this->MaxClients; i++ )
		ClientsArray[i].Fd = 0;
	
	/* Now just start server */
	this->Start();
}

TcpServerAsync::~TcpServerAsync()
{
	/* Important! */
	delete[] this->ClientsArray;
}

void TcpServerAsync::Start()
{
	if( Status == STARTED )
		return;
	
	/* Create server socket handler */
	if( (this->ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
	{
		throw Exception("Start->socket", strerror(errno));
	}
	
	/* Allow multiple clients */
	int optval = 1;
	int optlen = sizeof(optval);
	if( setsockopt(this->ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0 )
	{
		throw Exception("Start()->setsockpt", strerror(errno));
	}
	
	/* Create server IP structure */
	struct sockaddr_in server;
	server.sin_family = AF_INET;                // address family. leave AF_INET
	server.sin_addr.s_addr = INADDR_ANY;        // accept connection from any if
	server.sin_port = htons(this->ServerPort);    // host-to-network short
	
	/* Bind to localhost:ServerPort */
	if( bind(this->ServerSocket, (const sockaddr *) &server, sizeof(server)) < 0 )
	{
		throw Exception("Start()->bind", strerror(errno));
	}
	
	/* Set maximum pending connecions: 3 */
	if( listen(this->ServerSocket, 3) < 0 )
	{
		perror("listen");
		throw ("Can't start listening on given port!");
	}
	
	Status = STARTED;
	
	BackgroundWorker = std::thread([this]()
								   { BackgroundWork(); });
	BackgroundWorker.detach();
}

void TcpServerAsync::Stop()
{
	if( Status == STOPPED )
		return;
	
	/* Close server socked fd */
	shutdown(this->ServerSocket, SHUT_RDWR);
	close(this->ServerSocket);
	this->ServerSocket = 0;
	Status = STOPPED;
	
	//std::cout << "Server stopped!" << std::endl;
}

int TcpServerAsync::Write(const client_t *client, const char *data, int len)
{
	int SendResult = (int) send(client->Fd, data, (size_t) len, 0);
	DataSend(client, data, SendResult);
}

void TcpServerAsync::BackgroundWork()
{
	while( true )
	{
		/* clear the socket set */
		FD_ZERO(&this->readfds);
		this->max_sd = 0;
		
		/* Add server socket socket to set only if it is open*/
		if( this->ServerSocket > 0 )
		{
			FD_SET(this->ServerSocket, &readfds);
			this->max_sd = this->ServerSocket;
		}
		
		//add child sockets to set
		for( uint16_t i = 0; i < this->MaxClients; i++ )
		{
			//if valid socket descriptor then add to read list
			if( ClientsArray[i].Fd > 0 )
				FD_SET(ClientsArray[i].Fd, &readfds);
			
			//highest file descriptor number, need it for the select function
			if( ClientsArray[i].Fd > max_sd )
				max_sd = ClientsArray[i].Fd;
		}
		
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		this->activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		
		if( (activity < 0) && (errno != EINTR) )
		{
			throw Exception("select", strerror(errno));
		}
		
		//If something happened on the master socket ,
		//then its an incoming connection
		if( FD_ISSET(this->ServerSocket, &this->readfds) )
		{
			/* Retrieve a free slot for this client */
			bool SlotsAvailable = false;
			client_t *CurrentClientSlot;
			for( int i = 0; i < this->MaxClients; i++ )
			{
				if( ClientsArray[i].Fd <= 0 )
				{
					SlotsAvailable = true;
					CurrentClientSlot = &ClientsArray[i];
					break;
				}
			}
			
			/* Refuse new connection if maximum clients slots is reached */
			if( !SlotsAvailable )
			{
				/* Accept and then close connection with a message */
				int tmp_sock = 0;
				struct sockaddr_in tmp_addr;
				socklen_t tmp_add_len = sizeof(tmp_addr);
				if( (tmp_sock = accept(this->ServerSocket, (struct sockaddr *) &tmp_addr, &tmp_add_len)) >= 0 )
				{
					char msg[] = "MAX_CONNECTION_REACHED\r\n";
					if( send(tmp_sock, msg, strlen(msg), 0) != strlen(msg) )
						throw Exception("send", strerror(errno));
					close(tmp_sock);
				}
			}
			else
			{
				int addrlen = sizeof(CurrentClientSlot->address);
				if( (CurrentClientSlot->Fd = accept(this->ServerSocket, (struct sockaddr *) &CurrentClientSlot->address, (socklen_t *) &addrlen)) < 0 )
				{
					throw Exception("accept", strerror(errno));
				}
				
				/* Set IP and port variables */
				CurrentClientSlot->Port = ntohs(CurrentClientSlot->address.sin_port);
				strcpy(CurrentClientSlot->Ip, inet_ntoa(CurrentClientSlot->address.sin_addr));
				
				/* Send a notification callback */
				ClientConnected(CurrentClientSlot);
			}
		}
		
		//else its some IO operation on some other socket
		for( int i = 0; i < MaxClients; i++ )
		{
			if( FD_ISSET(ClientsArray[i].Fd, &readfds) )
			{
				//Check if it was for closing , and also read the
				//incoming message
				if( (this->Readbytes = read(ClientsArray[i].Fd, RecvBuffer, RECV_BUFFER_SIZE)) == 0 )
				{
					//Somebody disconnected , get his details and print
					close(ClientsArray[i].Fd);
					ClientsArray[i].Fd = 0;
					/* Push a notification */
					ClientDisconnected(&ClientsArray[i]);
				}
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					RecvBuffer[this->Readbytes] = '\0';
					DataReceived(&ClientsArray[i], RecvBuffer, (int) this->Readbytes);
				}
			}
		}
	}
}

void TcpServerAsync::ClientConnected(TcpServerAsync::client_t *client)
{
	printf("[%d %s:%d] CONNECTED\n", client->Fd, client->Ip, client->Port);
}

void TcpServerAsync::ClientDisconnected(const TcpServerAsync::client_t *client)
{
	printf("[%d %s:%d] DISCONNECTED\n", client->Fd, client->Ip, client->Port);
}

void TcpServerAsync::DataReceived(const TcpServerAsync::client_t *client, char *data, int length)
{
	printf("[%d %s:%d] RECV  (%d bytes): %s\n", client->Fd, client->Ip, client->Port, length, data);
}

void TcpServerAsync::DataSend(const TcpServerAsync::client_t *client, const char *data, int bytesSend)
{
	printf("[%d %s:%d] SEND: (%d bytes): %s\n", client->Fd, client->Ip, client->Port, bytesSend, data);
}
