#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>  

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Logging.hpp"

#pragma comment(lib, "Ws2_32.lib")

class TCPServer
{
public:
	TCPServer(u_short port, const char* bind_address = "0.0.0.0");
	bool initServer();
	int broadcastData();
	int receiveData();
protected:
	SOCKET _serverSocket;
	struct sockaddr_in _serverAddr;
	WSADATA _wsaData;
	std::ofstream _logFile;
	u_short _port;
	std::string _ip;
	bool _isValid;
	Logging _log;
};

