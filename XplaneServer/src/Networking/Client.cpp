#include "Client.hpp"

Client::Client(SOCKET socket)
{
	sock = socket;
	_dataInBuffer = 0;
	memset(_buffer, 0x00, MAX_BUFFER_SIZE);
}

int Client::sendTCPData(std::string data)
{
	ULONG NonBlock = 0;
	ioctlsocket(sock, FIONBIO, &NonBlock);
	int res = send(sock, data.c_str(), data.length(), 0);
	NonBlock = 1;
	ioctlsocket(sock, FIONBIO, &NonBlock);
	return res;
}

int Client::receiveTCPData()
{
	ULONG NonBlock = 1;
	ioctlsocket(sock, FIONBIO, &NonBlock);
	int res = recv(sock, _buffer, MAX_BUFFER_SIZE, 0);
	if (res == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
		{
			return 0;
		}
	}
	return res;
}

std::string Client::getAndEmptyBuffer()
{
	std::string output = std::string(_buffer);
	output = output.substr(_dataInBuffer);
	memset(_buffer, 0x00, MAX_BUFFER_SIZE);
	return output;
}
