#pragma once
#include <string>
#include <map>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>

#include <Logging.hpp>


class Dataref
{
public:
	static Logging log;
public:
	Dataref();
	Dataref(std::string link, XPLMDataTypeID dataType);
	Dataref(const Dataref& b);
	~Dataref();

	std::string GetValue();
	std::string GetDataType();
	bool SetValue(std::string value, bool forceReadOnly = false);
	bool IsReadOnly(bool ForceRecheck = false);

private:
	bool _isReadOnly;
	bool _isValid;
	std::string _link;
	XPLMDataTypeID _dataType;
	XPLMDataRef _dataref;
private:
	void XPLANELogException(const char* previousMessage, const std::exception& e);
};

