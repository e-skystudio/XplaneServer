#include "../include/UDPConnection.hpp"

UDPConnection::UDPConnection(u_short port, const char* bind_address): _serverSocket(INVALID_SOCKET), _serverAddr({0}),
	_wsaData({ 0 }), _port(port), _ip(bind_address), _isValid(false)
{
}

bool UDPConnection::initServer()
{
	int res = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (res != NO_ERROR)
	{
		log("UDPConnection WSA Startup Failed with error : " + std::to_string(res));
		return _isValid;
	}
#ifdef _DEBUG
	log("WSA Startup Sucessfull !");
#endif // _DEBUG

	_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (_serverSocket == INVALID_SOCKET) {
		log("socket creation failed with error" + std::to_string(WSAGetLastError()));
		return _isValid;
	}
#ifdef _DEBUG
	log("socket creation Sucessfull !");
#endif // _DEBUG
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	inet_pton(AF_INET, "127.0.0.1", &_serverAddr.sin_addr.s_addr);

	if (bind(_serverSocket, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr))) {
		log("bind failed with error: " + std::to_string(WSAGetLastError()));
		return _isValid;
	}

	log("UDP Connection initalization sucessful on " + std::to_string(_port));
	_isValid = true;
	return _isValid;
}

int UDPConnection::sendData(sockaddr_in target, std::string data)
{
	int res = sendto(_serverSocket, data.c_str(), data.length(), 0,
		             (SOCKADDR*)&target, sizeof(target));
	log("Sending " + data);
	return res;
}

void UDPConnection::log(std::string dataIn)
{
	_logFile.open("./UDP_LOG.log", std::ios::out | std::ios::app);
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	std::string time = std::to_string(now->tm_year + 1900) + "/" + std::to_string(now->tm_mon) + "/" + 
		std::to_string(now->tm_mday) + " " + std::to_string(now->tm_hour) + ":" + 
		std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);

	std::string log_string = time + " : " + dataIn + "\n";
#ifdef _DEBUG
	std::cout << log_string;
#endif // _DEBUG

	_logFile << log_string;
	_logFile.close();
}