#pragma once


#include <json/json.h>
#include "../Networking/TCPServer.hpp"
#include "Dataref.hpp"

extern DatarefMap datarefMap;
extern TimedDatarefMap timedDatarefMap;

bool parseJson(std::string dataIn, Json::Value& json);
int RegisterDataref_Callback(std::string dataIn, Client& emiter);
int SetDatarefValue_Callback(std::string dataIn, Client& emiter);
int GetDatarefValue_Callback(std::string dataIn, Client& emiter);
