#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include "TCPServer.hpp"


int main(int argc, char* argv[])
{
	TCPServer serv = TCPServer(5505, "0.0.0.0");
	serv.initServer();
	while (1)
	{
		serv.listenForClient();
		if (serv.connectedClients() > 0)
		{
			serv.broadcastData("Good news everyone\n");
		}
		Sleep(1000);
	}
	

	return 0;
}