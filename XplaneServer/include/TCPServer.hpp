#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Client.hpp"
#include "Logging.hpp"

#pragma comment(lib, "Ws2_32.lib")

class TCPServer
{
public:
	TCPServer(u_short port, const char* bind_address = "0.0.0.0");
	bool initServer();
	int broadcastData(std::string data);
	int receiveData();
	int listenForClient();
	int connectedClients();
protected:
	SOCKET _serverSocket;
	struct sockaddr_in _serverAddr;
	WSADATA _wsaData;
	std::ofstream _logFile;
	u_short _port;
	std::string _ip;
	bool _isValid;
	Logging _log;
	ClientsVec _clients;
	FD_SET _WriteSet;
	FD_SET _ReadSet;
	FD_SET _ExceptSet;

	void setUpFdSet();
};

