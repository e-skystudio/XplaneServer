#include "..\include\UDPEmiter.h"

UDPEmiter::UDPEmiter() : _port(0), _ip("0.0.0.0"), _wsaData({0}), _serverAddr({0}), _isValid(false), _log("UPD_SERVER")
{
}

UDPEmiter::UDPEmiter(u_short port, const char* bind_address)
{
	UDPEmiter();
	_port = port;
	_ip = bind_address;
}

void UDPEmiter::setBindAddress(u_short port, const char* bind_address)
{
	_port = port;
	_ip = bind_address;
}

bool UDPEmiter::initServer()
{
	int res = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (res != NO_ERROR)
	{
		_log.addToFile("UDPConnection WSA Startup Failed with error : " + std::to_string(res));
		return _isValid;
	}
#ifdef _DEBUG
	_log.addToFile("WSA Startup Sucessfull !");
#endif // _DEBUG
	_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (_serverSocket == INVALID_SOCKET) {
		_log.addToFile("socket creation failed with error" + std::to_string(WSAGetLastError()));
		return _isValid;
	}
#ifdef _DEBUG
	_log.addToFile("socket creation Sucessfull !");
#endif // _DEBUG
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &_serverAddr.sin_addr.s_addr);
#ifdef _DEBUG
	_log.addToFile("bind on port is Sucessfull !");
#endif // _DEBUG
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	if (setsockopt(_serverSocket, SOL_SOCKET,  SO_BROADCAST, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
	{
		_log.addToFile("setsockopt() failed with error" + WSAGetLastError());
		return 1;
	}
	else
		_log.addToFile("ioctlsocket() is OK!");
	_log.addToFile("UDP Connection initalization sucessful on " + std::to_string(_port));
	_isValid = true;
	return _isValid;
}

int UDPEmiter::broadcastData(std::string data)
{
	int res = sendto(_serverSocket, data.c_str(), (int)data.length(), 0, (struct sockaddr*)&_serverAddr, (int)sizeof(_serverAddr));
	return res;
}
