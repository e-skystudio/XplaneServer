#include "Callbacks.hpp"

std::map<std::string, Dataref*> datarefMap = std::map<std::string, Dataref*>();

bool parseJson(std::string dataIn, Json::Value& json)
{
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    std::string errors;

    return reader->parse(dataIn.c_str(), dataIn.c_str() + dataIn.size(), &json, &errors);
}

int RegisterDataref_Callback(std::string dataIn, Client& emiter)
{
    Json::Value json;
    if (!parseJson(dataIn, json))
    {
        emiter.sendTCPData("INVALID JSON");
        return -2;
    }

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
    datarefMap.emplace(json["Name"].asString(), d);

    emiter.sendTCPData("SUCESS"); //DEBUG to be change by json;

    return 0;
}

int SetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    Json::Value json;
    if (!parseJson(dataIn, json))
    {
        emiter.sendTCPData("INVALID JSON");
        return -2;
    }
    Dataref* selDR = nullptr;
    auto datarefIterator = datarefMap.find(json["Name"].asString());
    if (datarefIterator == datarefMap.end())
    {
        emiter.sendTCPData("DATAREF NOT FOUND, PLEASE REGISTER IT");
    }

    selDR = (*datarefIterator).second;
    if (selDR->IsReadOnly())
    {
        emiter.sendTCPData("SELECTED DATAREF IS READONLY; CHECK IF ANY OVERRIDE IS REQUIRED BEFORE TRYING TO MODIFY IT");
        return -3;
    }

    bool forceHoldValue = 

    return 0;
}

int GetDatarefValue_Callback(std::string dataIn, Client& emiter)
{
    return 0;
}
