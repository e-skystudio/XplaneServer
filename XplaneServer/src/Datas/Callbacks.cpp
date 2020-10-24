#include "Callbacks.hpp"

std::map<std::string, Dataref*> DatarefMap = std::map<std::string, Dataref*>();

int RegisterDataref_Callback(std::string dataIn, Client& emiter)
{
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value json;
    std::string errors;

    reader->parse(dataIn.c_str(), dataIn.c_str() + dataIn.size(), &json, &errors);

    XPLMDataTypeID type = xplmType_Int; //DEFAULT

    if (json["Type"].asString() == "int")
    {
        type = xplmType_Int;
    }
    else if (json["Type"].asString() == "float")
    {
        type = xplmType_Float;
    }
    else if (json["Type"].asString() == "double")
    {
        type = xplmType_Double;
    }

    Dataref* d = new Dataref(json["Link"].asString(), type);
    DatarefMap.emplace(json["Name"].asString(), d);

    emiter.sendTCPData("SUCESS"); //DEBUG to be change by json;

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
