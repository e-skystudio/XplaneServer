#pragma once

#include <winsock2.h>
#include <vector>

#define MAX_BUFFER_SIZE 2048
class Client
{
public:
	Client(SOCKET socket);
	SOCKET sock;
	char buffer[MAX_BUFFER_SIZE];
	int dataInBuffer;

};

typedef std::vector<Client> ClientsVec;