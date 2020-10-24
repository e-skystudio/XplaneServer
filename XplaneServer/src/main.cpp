#include "Networking/TCPServer.hpp"
#include <map>
#include "./Datas/Dataref.hpp"

#include <XPLMProcessing.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>

std::map<std::string, Dataref*> DatarefMap = std::map<std::string, Dataref*>();
TCPServer server;

float MainCallBack(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	strcpy(outName, "eSkyAirways-Simulator Connector");
	strcpy(outSig, "eskystudio.eskyAirways.simulator_connector");
	strcpy(outDesc, "Link X-Plane to eSkyAirways");
	server = TCPServer(5505);
	if (!server.initServer())
	{
		XPLMSpeakString("UNABLE TO START THE SERVER");
	}

	DatarefMap.emplace("Latitude", new Dataref("sim/flightmodel/position/latitude", xplmType_Float));
	DatarefMap.emplace("Longitude", new Dataref("sim/flightmodel/position/longitude", xplmType_Float));
	DatarefMap.emplace("Elevation", new Dataref("sim/flightmodel/position/elevation", xplmType_Float));
	XPLMRegisterFlightLoopCallback(MainCallBack, 0.5f, NULL);


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

float MainCallBack(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	server.listenForClient();
	if (server.connectedClients() > 0)
	{
		std::string data = "Latitude : " + DatarefMap["Latitude"]->GetValue() + "\n";
		data += "Longitude : " + DatarefMap["Longitude"]->GetValue() + "\n";
		data += "Elevation : " + DatarefMap["Elevation"]->GetValue() + "\n\n";
		server.broadcastData(data);
	}
	return 0.5f;
}
