#include "..\include\Client.hpp"

Client::Client(SOCKET socket)
{
	sock = socket;
	dataInBuffer = 0;
	memset(buffer, 0x00, MAX_BUFFER_SIZE);
}
