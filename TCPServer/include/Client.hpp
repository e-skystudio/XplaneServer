#pragma once

#include <winsock2.h>
#include <vector>
#include <string>

#define MAX_BUFFER_SIZE 2048
class Client
{
public:
	Client(SOCKET socket);
	SOCKET sock;

	int sendTCPData(std::string data);
	int receiveTCPData();
	std::string getAndEmptyBuffer();
protected:
	char _buffer[MAX_BUFFER_SIZE];
	int _dataInBuffer;
};

typedef std::vector<Client> ClientsVec;