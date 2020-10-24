#include "..\include\TCPServer.hpp"

TCPServer::TCPServer(u_short port, const char* bind_address) : _port(port), _ip(bind_address)
{
    _log = Logging("TCP_SERVER");
	_clients = ClientsVec();
}

bool TCPServer::initServer()
{
	int res = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (res != NO_ERROR)
	{
		_log.AddToFile("UDPConnection WSA Startup Failed with error : " + std::to_string(res));
		return _isValid;
	}
#ifdef _DEBUG
	_log.AddToFile("WSA Startup Sucessfull !");
#endif // _DEBUG
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverSocket == INVALID_SOCKET) {
		_log.AddToFile("socket creation failed with error" + std::to_string(WSAGetLastError()));
		return _isValid;
	}
#ifdef _DEBUG
	_log.AddToFile("socket creation Sucessfull !");
#endif // _DEBUG
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &_serverAddr.sin_addr.s_addr);

	if (bind(_serverSocket, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr))) {
		_log.AddToFile("bind failed with error: " + std::to_string(WSAGetLastError()));
		return _isValid;
	}
#ifdef _DEBUG
	_log.AddToFile("bind on port is Sucessfull !");
#endif // _DEBUG
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(_serverSocket);
		WSACleanup();
		return 1;
	}

	ULONG NonBlock = 1;

	if (ioctlsocket(_serverSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		_log.AddToFile("ioctlsocket() failed with error" +  WSAGetLastError());
		return 1;
	}
	else
		_log.AddToFile("ioctlsocket() is OK!");

	_log.AddToFile("TCP Connection initalization sucessful on " + std::to_string(_port));
	_isValid = true;
	return _isValid;
}



int TCPServer::broadcastData(std::string data)
{
	auto it = _clients.begin();
	while(_clients.size() > 0 && it != _clients.end())
	{
		ULONG NonBlock = 0;
		ioctlsocket(it->sock, FIONBIO, &NonBlock);
		int res = send(it->sock, data.c_str(), data.length(), 0);
		if (res == SOCKET_ERROR)
		{
			_log.AddToFile("Error in broadcast : " + std::to_string(WSAGetLastError()));
			_clients.erase(it);
			continue;
		}
		else {
			_log.AddToFile("broadcast sended : " + std::to_string(res) + " byte(s)");
		}
		NonBlock = 1;
		ioctlsocket(it->sock, FIONBIO, &NonBlock);
		it += 1;
	}
	return data.length();
}

int TCPServer::receiveData()
{
    return 0;
}

int TCPServer::listenForClient()
{
	listen(_serverSocket, SOMAXCONN);
	sockaddr cliAddress;
	int sizeOfCli = sizeof(cliAddress);
	SOCKET client = accept(_serverSocket, &cliAddress, &sizeOfCli);
	if (client == SOCKET_ERROR)
	{
		return 0;
	}

	std::string data = "Hello on the server\n";
	int res = send(client, data.c_str(), data.length(), 0);
	if (res == SOCKET_ERROR)
	{
		_log.AddToFile("Error in listenForClient : " + std::to_string(WSAGetLastError()));
	}
	else {
		_log.AddToFile("listenForClient sended : " + std::to_string(res) + " byte(s)");
	}

	ULONG NonBlock = 1;
	if (ioctlsocket(client, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		return -2;
	}
	_clients.push_back(Client(client));
	return _clients.size();
}

int TCPServer::connectedClients()
{
	return _clients.size();
}
