#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>  

#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class UDPConnection
{
public:
	UDPConnection(u_short port, const char* bind_address = "0.0.0.0");
	bool initServer();
	int sendData(struct sockaddr_in target, std::string data);
	bool receiveData();
protected:
	SOCKET _serverSocket;
	struct sockaddr_in _serverAddr;
	WSADATA _wsaData;
	std::ofstream _logFile;
	u_short _port;
	std::string _ip;
	bool _isValid;
private:
	void log(std::string dataIn);
};
