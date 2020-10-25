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
    }
    if (j.contains("Name"))
    {
        name = j["Name"];
    }
    if (j.contains("Link"))
    {
        link = j["Link"];
    }
    std::string output = "Type : " + typeStr + " Name : " + name + " Link : " + link + "\n";
    logger.addToFile("Type : " + typeStr);
    logger.addToFile("Name : " + name);
    logger.addToFile("Link : " + link);
    Dataref* d = new Dataref(link, type);
    DatarefMap.emplace(name, d);
    emiter.sendTCPData(output);
    return 0;
}

int SetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    return 0;
}

int GetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    return 0;
}
