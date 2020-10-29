#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Client.hpp"
#include <Logging.hpp>

#pragma comment(lib, "Ws2_32.lib")


class UDPEmiter
{
public:
	UDPEmiter();
	UDPEmiter(u_short port, const char* bind_address = "0.0.0.0");
	void setBindAddress(u_short port, const char* bind_address = "0.0.0.0");
	bool initServer();
	int broadcastData(std::string data);
protected:
	SOCKET _serverSocket;
	struct sockaddr_in _serverAddr;
	struct sockaddr_in _broadcastse;
	WSADATA _wsaData;
	u_short _port;
	std::string _ip;
	bool _isValid;
	Logging _log;
};

