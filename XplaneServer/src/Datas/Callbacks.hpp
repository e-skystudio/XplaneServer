#pragma once


#include <json/json.h>
#include "../Networking/TCPServer.hpp"
#include "Dataref.hpp"

extern std::map<std::string, Dataref*> DatarefMap;

int RegisterDataref_Callback(std::string dataIn, Client& emiter);
int SetDatarefValue_Callback(std::string dataIn, Client& emiter);
int GetDatarefValue_Callback(std::string dataIn, Client& emiter);
