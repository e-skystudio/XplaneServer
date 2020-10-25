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

typedef int (*ServerCallbacks)(std::string dataIn, Client &emiter);

class TCPServer
{
public:
	TCPServer();
	TCPServer(u_short port, const char* bind_address = "0.0.0.0");
	void setBindAddress(u_short port, const char* bind_address = "0.0.0.0");
	bool initServer();
	int broadcastData(std::string data);
	void receiveData();
	int listenForClient();
	int connectedClients();
	void bindCallback(std::string input, ServerCallbacks callback);
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
	std::map<std::string, ServerCallbacks> _callbacks;
	int parseReceivedData(Client& cli, std::string data);
	std::string getOpsFromJson(std::string input);
	std::vector<std::string> splitCommand(std::string input);
};

