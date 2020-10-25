#include <TCPServer.hpp>

#include "./Datas/Callbacks.hpp"
#include "./Datas/Dataref.hpp"

#include <XPLMProcessing.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <nlohmann/json.hpp>

#include <map>

using json = nlohmann::json;

TCPServer server;
float InitCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
float MainCallBack(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
extern std::map<std::string, Dataref*> DatarefMap;

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	strcpy(outName, "eSkyAirways-Simulator Connector");
	strcpy(outSig, "eskystudio.eskyAirways.simulator_connector");
	strcpy(outDesc, "Link X-Plane to eSkyAirways");
	
	
	XPLMRegisterFlightLoopCallback(InitCallback, -1.0f  /*wait for the first frame*/, NULL);
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam)
{
}

float InitCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	server = TCPServer(5505);
	server.bindCallback("REGISTER_DATAREF", RegisterDataref_Callback);
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
		std::string data = "";
		for (auto& kv : DatarefMap)
		{
			json dataref;
			dataref["Name"] = kv.first;
			dataref["Type"] = kv.second->GetDataType();
			dataref["Value"] = kv.second->GetValue();

			//data += kv.first + " : " + kv.second->GetValue() + "\n";
			j.push_back(dataref);
		}
		server.broadcastData(j.dump());
	}
	return 0.5f;
}
