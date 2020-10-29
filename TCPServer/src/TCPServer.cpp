#include "../include/TCPServer.hpp"

TCPServer::TCPServer() : _port(0), _ip("")
{
	_log = Logging("TCP_SERVER");
	_clients = ClientsVec();
}

TCPServer::TCPServer(u_short port, const char* bind_address) : _port(port), _ip(bind_address)
{
	_log = Logging("TCP_SERVER");
	_clients = ClientsVec();
}

void TCPServer::setBindAddress(u_short port, const char* bind_address)
{
	_port = port;
	_ip = bind_address;
}

bool TCPServer::initServer()
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
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	if (bind(_serverSocket, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr))) {
		_log.addToFile("bind failed with error: " + std::to_string(WSAGetLastError()));
		return _isValid;
	}
#ifdef _DEBUG
	_log.addToFile("bind on port is Sucessfull !");
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
		_log.addToFile("ioctlsocket() failed with error" +  WSAGetLastError());
		return 1;
	}
	else
		_log.addToFile("ioctlsocket() is OK!");

	_log.addToFile("TCP Connection initalization sucessful on " + std::to_string(_port));
	_isValid = true;
	return _isValid;
}

void TCPServer::receiveData()
{
	auto it = _clients.begin();
	while (_clients.size() > 0 && it != _clients.end())
	{
		if (it->receiveTCPData() > 0)
		{
			std::string data = it->getAndEmptyBuffer();
			_log.addToFile(data);
			parseReceivedData(*it, data);
		}
		it += 1;
	}
}

int TCPServer::broadcastData(std::string data)
{
	auto it = _clients.begin();
	while(_clients.size() > 0 && it != _clients.end())
	{
		int res = it->sendTCPData(data);
		if (res == SOCKET_ERROR)
		{
			_clients.erase(it);
		}
		it++;
	}
	return (int)data.length();
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

	std::string data = "{\"Server Version\": \""+ std::to_string(PROTO_MAJ) + '_' + std::to_string(PROTO_MAJ) + "\"";
	data += "\"Connexion Status\":\"Accepted\"}";
	int res = send(client, data.c_str(), (int)data.length(), 0);
	if (res == SOCKET_ERROR)
	{
		_log.addToFile("Error in listenForClient : " + std::to_string(WSAGetLastError()));
	}
	else {
		_log.addToFile("listenForClient sended : " + std::to_string(res) + " byte(s)");
	}

	ULONG NonBlock = 1;
	if (ioctlsocket(client, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		return -2;
	}
	_clients.push_back(Client(client));
	return (int)_clients.size();
}

int TCPServer::connectedClients()
{
	return (int)_clients.size();
}

void TCPServer::bindCallback(std::string input, ServerCallbacks callback)
{
	_callbacks.emplace(input, callback);
	_log.addToFile("callback" + input + ":");
	for (auto& kv : _callbacks)
	{
		_log.addToFile("\t " + kv.first + " : [BINDED]");
	}
}

int TCPServer::parseReceivedData(Client &cli, std::string data)
{
	std::vector<std::string> commands = splitCommand(data);
	for (auto& string : commands)
	{
		_log.addToFile("Loaded : " + string);
		std::string operation = getOpsFromJson(string);
		auto it = _callbacks.find(operation);
		if (it == _callbacks.end())
		{
			_log.addToFile("Last command didn't match any callbacks");
			continue;
		}
		_log.addToFile("Executing callback" + operation + ": [STARTED]");
		int res = it->second(string, cli);
		_log.addToFile("Executing callback " + operation + ": [DONE] (Returned = " + std::to_string(res) + ")");
	}
	return -1;
}

std::string TCPServer::getOpsFromJson(std::string input)
{
	std::regex regexPattern("[\\\'\\\"]Ops[\\\'\\\"]\\ ?:\\ ?[\\\'\\\"](\\b\\w+\\b)[\\\'\\\"]");
	std::smatch sm;

	bool res = regex_search(input, sm, regexPattern);
	if (!res)
	{
		return "";
	}
	return sm[1].str();
}

std::vector<std::string> TCPServer::splitCommand(std::string input)
{
	std::string split_char = "}{";
	std::vector<std::string> datas = std::vector<std::string>();
	std::size_t split(0);
	while (split != std::string::npos)
	{
		split = input.find(split_char);
		if (split == std::string::npos)
		{
			continue;
		}
		datas.push_back(input.substr(0, split + 1));
		input = input.substr(split + 1);
	}
	datas.push_back(input);
	return datas;
}