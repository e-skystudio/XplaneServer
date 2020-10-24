#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include "Networking/TCPServer.hpp"
#include <json/json.h>

int tryParseJSON(std::string text)
{
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();

	Json::Value root;
	std::string errors;

	bool parsingSuccessful = reader->parse(text.c_str(), text.c_str() + text.size(), &root, &errors);
	if (!parsingSuccessful)
	{
		std::cout << "Error parsing the string" << std::endl;
	}
	const Json::Value operation = root["Ops"];
	std::cout << operation.asString();
	return 0;
}

int RegisterDataref(std::string datain, Client& emiter)
{
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();

	Json::Value root;
	std::string errors;

	bool parsingSuccessful = reader->parse(datain.c_str(), datain.c_str() + datain.size(), &root, &errors);
	if (!parsingSuccessful)
	{
		std::cout << "Error parsing the string" << std::endl;
		return -2; //Unable to parse the string
	}

	std::cout << "Registering Dataref\n";
	std::cout << "\tName:" << root["Name"].asString() <<  "\n";
	std::cout << "\tLink:" << root["Link"].asString() <<  "\n";
	std::cout << "\tType:" << root["Type"].asString() <<  "\n";
	std::cout << "\n\n";
		
}

int main(int argc, char* argv[])
{
	TCPServer serv = TCPServer(5505, "0.0.0.0");
	serv.initServer();
	serv.bindCallback("REGISTER_DATAREF", RegisterDataref);
	while (1)
	{
		serv.listenForClient();
		if (serv.connectedClients() > 0)
		{
			serv.broadcastData("Good news everyone\n");
			serv.receiveData();
		}
		Sleep(1000);
	}
	//std::string jInput = "{\"Ops\": \"REGISTER_DATAREF\", \"Link\": \"sim/flightmodel/position/elevation\", \"Value\": \"float\", \"Name\": \"elevation\"}";
	//tryParseJSON(jInput);

	return 0;
}