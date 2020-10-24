#pragma once
#include <string>
#include <map>
#include <json/json.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>

#include "../Log/Logging.hpp"


class Dataref
{
public:
	static bool ParseJSON(std::string input, Json::Value& json);

public:
	Dataref();
	Dataref(std::string link, XPLMDataTypeID dataType);
	Dataref(const Dataref& b);
	~Dataref();

	std::string GetValue();
	bool SetValue(std::string value, bool forceReadOnly = false);
	bool IsReadOnly(bool ForceRecheck = false);

private:
	bool _isReadOnly;
	bool _isValid;
	std::string _link;
	XPLMDataTypeID _dataType;
	XPLMDataRef _dataref;
	Logging _log;
private:
	void XPLANELogException(const char* previousMessage, const std::exception& e);
};
