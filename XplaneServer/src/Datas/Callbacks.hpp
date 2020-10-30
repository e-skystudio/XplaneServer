#pragma once


//#include <json/json.h>
#include <nlohmann/json.hpp>
#include <TCPServer.hpp>
#include "Dataref.hpp"
#include <Logging.hpp>
#include <XPLMNavigation.h>
#include <XPLMPlanes.h>

extern Logging logger;
extern std::map<std::string, Dataref*> DatarefMap;

int RegisterDataref_Callback(std::string dataIn, Client& emiter);
int SetDatarefValue_Callback(std::string dataIn, Client& emiter);
int GetDatarefValue_Callback(std::string dataIn, Client& emiter);
int SetActivePause_Callback(std::string dataIn, Client& emiter);
int RepositionAircraft_Callback(std::string dataIn, Client& emiter);
int GetAirport_Callback(std::string dataIn, Client& emiter);