#include <TCPServer.hpp>

#include "./Datas/Callbacks.hpp"
#include "./Datas/Dataref.hpp"

#include <XPLMProcessing.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMPlugin.h>
#include <nlohmann/json.hpp>

#include <map>

using json = nlohmann::json;

XPLMDataRef HeightDataref = XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");
XPLMDataRef GroundDataref = XPLMFindDataRef("sim/flightmodel/failures/onground_any");
bool Airborn = false;

TCPServer server;
float InitCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
float MainCallBack(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
float PerFrameCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
extern std::map<std::string, Dataref*> DatarefMap;

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	strcpy(outName, "eSkyAirways-Simulator A320");
	strcpy(outSig, "eskystudio.XPlane.A320");
	strcpy(outDesc, "X-Plane A320 Interface");
	
	
	XPLMRegisterFlightLoopCallback(InitCallback, -1.0f  /*wait for the first frame*/, NULL);
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
}

PLUGIN_API void XPluginDisable(void)
{
	if (server.connectedClients() > 0)
	{
		json j;
		j["Ops"] = "SpecialMessage";
		j["Content"] = "Stopping";
		server.broadcastData(j.dump());
	}
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam)
{
	if (inMessage == XPLM_MSG_PLANE_CRASHED)
	{
		if (server.connectedClients() > 0)
		{
			json j;
			j["Ops"] = "SpecialMessage";
			j["Content"] = "Crashed";
			server.broadcastData(j.dump());
		}
	}
}

float InitCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	server = TCPServer(5505);
	server.bindCallback("REGISTER_DATAREF", RegisterDataref_Callback);
	server.bindCallback("SET_DATAREF", SetDatarefValue_Callback);
	server.bindCallback("GET_AIRPORT", GetAirport_Callback);
	if (!server.initServer())
	{
		XPLMSpeakString("UNABLE TO START THE SERVER");
	}
	else {
		XPLMRegisterFlightLoopCallback(MainCallBack, -1.0f  /*wait for the first frame*/, NULL);
	}
	return 0.0f;
}

float MainCallBack(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	server.listenForClient();
	server.receiveData();
	if (server.connectedClients() > 0)
	{
		json j;
		j["Ops"] = "PositionReport";
		j["Datarefs"] = json::array();
		std::string data = "";
		for (auto& kv : DatarefMap)
		{
			json dataref;
			dataref["Name"] = kv.first;
			dataref["Value"] = kv.second->GetValue();
			j["Datarefs"].push_back(dataref);
		}
		server.broadcastData(j.dump());
	}
	return 0.25f;
}

float PerFrameCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	if (XPLMGetDataf(HeightDataref) >= 999.0f && XPLMGetDataf(HeightDataref) <= 1001.0f)
	{
		if (server.connectedClients() > 0)
		{
			json j;
			j["Ops"] = "PositionReport";
			j["Value"] = "1000ft";
			j["Datarefs"] = json::array();
			std::string data = "";
			for (auto& kv : DatarefMap)
			{
				json dataref;
				dataref["Name"] = kv.first;
				dataref["Value"] = kv.second->GetValue();
				j["Datarefs"].push_back(dataref);
			}
			server.broadcastData(j.dump());
		}
	}

	if (XPLMGetDataf(HeightDataref) >= 499.0f && XPLMGetDataf(HeightDataref) <= 501.0f)
	{
		if (server.connectedClients() > 0)
		{
			json j;
			j["Ops"] = "SpecialReport";
			j["Value"] = "500ft";
			j["Datarefs"] = json::array();
			std::string data = "";
			for (auto& kv : DatarefMap)
			{
				json dataref;
				dataref["Name"] = kv.first;
				dataref["Value"] = kv.second->GetValue();
				j["Datarefs"].push_back(dataref);
			}
			server.broadcastData(j.dump());
		}
	}

	if (XPLMGetDatai(GroundDataref) == 1 && Airborn)
	{
		Airborn = false;
		if (server.connectedClients() > 0)
		{
			json j;
			j["Ops"] = "SpecialReport";
			j["Value"] = "Landing";
			j["Datarefs"] = json::array();
			std::string data = "";
			for (auto& kv : DatarefMap)
			{
				json dataref;
				dataref["Name"] = kv.first;
				dataref["Value"] = kv.second->GetValue();
				j["Datarefs"].push_back(dataref);
			}
			server.broadcastData(j.dump());
		}
	}
	
	if (XPLMGetDatai(GroundDataref) == 0 && !Airborn)
	{
		Airborn = true;
	}
	return -1.0f;
}