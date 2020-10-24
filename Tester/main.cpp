#include <iostream>
#include <WinSock2.h>

#include <UDPConnection.h>


int main(int argc, char* argv[])
{
	UDPConnection conn = UDPConnection(5055, "127.0.0.1");
	std::cout << conn.initServer();

	struct sockaddr_in broadcast_address;
	broadcast_address.sin_family = AF_INET;
	broadcast_address.sin_port = 5506;
	inet_pton(AF_INET, "127.0.0.1", &broadcast_address.sin_addr.s_addr);

	for(int i(0); i < 100; i += 1)
	{
		conn.sendData(broadcast_address, "TEST MESSAGE" + std::to_string(i));
		Sleep(1000);
	}

	return 0;
}