#pragma once
#include <string>
#include <map>
#include <iterator>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <nlohmann/json.hpp>
#include <Logging.hpp>


class Dataref
{
public:
	static Logging log;
public:
	Dataref();
	Dataref(std::string link, XPLMDataTypeID dataType, int index=-1);
	Dataref(const Dataref& b);
	~Dataref();
	std::string GetDataType();
	std::string GetValue();
	bool SetValue(std::string value, bool forceReadOnly = false);
	bool IsReadOnly(bool ForceRecheck = false);
private:
	bool _isReadOnly;
	bool _isValid;
	std::string _suffix;
	std::string _link;
	XPLMDataTypeID _dataType;
	XPLMDataRef _dataref;
	int _index;
private:
	void XPLANELogException(const char* previousMessage, const std::exception& e);
};

