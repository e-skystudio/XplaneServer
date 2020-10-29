#include "Callbacks.hpp"

using json = nlohmann::json;

Logging logger = Logging("CALLBACK");
std::map<std::string, Dataref*> DatarefMap = std::map<std::string, Dataref*>();

int RegisterDataref_Callback(std::string dataIn, Client& emiter)
{
    json j = json::parse(dataIn);
    std::string typeStr("");
    std::string name("");
    std::string link("");
    int index(-1);
    XPLMDataTypeID type = xplmType_Int;

    if (j.contains("Type"))
    {
        typeStr = j["Type"];
        if (typeStr == "int")
        {
            type = xplmType_Int;
        }
        else if (typeStr == "float")
        {
            type = xplmType_Float;
        }
        else if (typeStr == "double")
        {
            type = xplmType_Double;
        }
        else if (typeStr == "float_array")
        {
            type = xplmType_FloatArray;
        }
    }
    if (j.contains("Index"))
    {
        index = j["Index"];
    }
    if (j.contains("Name"))
    {
        name = j["Name"];
    }
    if (j.contains("Link"))
    {
        link = j["Link"];
    }
    json resp;
    resp["Ops"] = "Response";
    resp["Value"] = "Sucess";

    emiter.sendTCPData(resp.dump());
    logger.addToFile("Type : " + typeStr);
    logger.addToFile("Name : " + name);
    logger.addToFile("Link : " + link);
    logger.addToFile("Index : " + std::to_string(index));
    Dataref* d = new Dataref(link, type, index);
    DatarefMap.emplace(name, d);
    return 0;
}

int SetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    logger.addToFile("SET_DATAREF_VALUE Started");
    json j = json::parse(dataIn);
    Dataref selDr;
    std::string name("");
    std::string value;

    if (j.contains("Value"))
    {
        value = j["Value"].dump();
        logger.addToFile("Value = " + value);
    }

    std::map<std::string, Dataref*>::iterator it;
    if (j.contains("Name") && (it = DatarefMap.find(j["Name"])) != DatarefMap.end())
    {
        selDr = *(DatarefMap.at(j["Name"]));
        logger.addToFile("Name = " + j["Name"]);
    }

    else 
    {
        logger.addToFile("Name not set or dataref not found!");
        if (!j.contains("Link"))
        {
            return 1;
        }
        std::string link(j["Link"]);
        logger.addToFile("Link = " + link);
        if (!j.contains("Type"))
        {
            return 2;
        }
        std::string typeStr(j["Type"]);
        logger.addToFile("Type = " + typeStr);
        XPLMDataTypeID type = xplmType_Int;
        if (typeStr == "int")
        {
            type = xplmType_Int;
        }
        else if (typeStr == "float")
        {
            type = xplmType_Float;
        }
        else if (typeStr == "double")
        {
            type = xplmType_Double;
        }
        else if (typeStr == "float_array")
        {
            type = xplmType_FloatArray;
        }
        logger.addToFile("Type = " + std::to_string(type));
        int index(-1);
        if (j.contains("Index"))
        {
            index = j["Index"];
        }
        logger.addToFile("Index = " + std::to_string(index));
        selDr = Dataref(link, type, index);
    }
    logger.addToFile("Value = " + value);
    selDr.SetValue(value);
    logger.addToFile("SET_DATAREF_VALUE Stopped");
    return 0;
}

int GetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    return 0;
}

int SetActivePause_Callback(std::string dataIn, Client& emiter)
{
    logger.addToFile("SET_ACTIVE_PAUSE Started");
    Dataref physics = Dataref("sim/operation/override/override_planepath", xplmType_Int);
    json j = json::parse(dataIn);
    if (j["Status"] == "Active")
    {
        physics.SetValue("1");
    }
    else {
        physics.SetValue("0");
    }
    logger.addToFile("SET_ACTIVE_PAUSE Stopped");
    return 0;
}

int RepositionAircraft_Callback(std::string dataIn, Client& emiter)
{
    return 0;
}
