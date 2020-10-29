#include "Dataref.hpp"

using json = nlohmann::json;

Logging Dataref::log = Logging("DATAREFS_MANAGER");

const char* ws = " \t\n\r\f\v\'\"";

inline std::string& rtrim(std::string& s, const char* t = ws)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws)
{
	return ltrim(rtrim(s, t), t);
}


Dataref::Dataref() : _isValid(false), _isReadOnly(true), _link(""), _dataType(xplmType_Unknown), _dataref(NULL), _index(-1)
{
}

Dataref::Dataref(std::string link, XPLMDataTypeID dataType, int index) : _link(link), _dataType(dataType), _dataref(NULL), _index(index)
{
	Dataref::log.addToFile("[NEW DATAREF] " + link);
	if (_dataType == xplmType_Unknown || link.compare("") == 0)
	{
		_isValid = false;
		_isReadOnly = true;
		_dataType = xplmType_Unknown;
		Dataref::log.addToFile("[NEW DATAREF] Wasn't created : Type or Link is empty");
		return;
	}
	_dataref = XPLMFindDataRef(_link.c_str());
	if (_dataref == NULL)
	{
		_isValid = false;
		_isReadOnly = true;
		_dataType = xplmType_Unknown;
		Dataref::log.addToFile("[NEW DATAREF] Wasn't created : Unable to find it");
		return;
	}
	_isValid = true;
	_isReadOnly = (bool)XPLMCanWriteDataRef(_dataref);
	Dataref::log.addToFile("[NEW DATAREF] Creation Sucessfull !");
	Dataref::log.addToFile("[NEW DATAREF] IsReadOnly = " + std::to_string(_isReadOnly));
}

Dataref::Dataref(const Dataref& b)
{
	_isValid = b._isValid;
	_isReadOnly = b._isReadOnly;
	_dataType = b._dataType;
	_dataref = b._dataref;
	_link = b._link;
}

Dataref::~Dataref()
{
}

std::string Dataref::GetDataType()
{
	std::string type("");
	switch (_dataType)
	{
	case xplmType_Int:
		return "Int";
	case xplmType_Float:
		return "Float";
	case xplmType_Double:
		return "Double";
	case xplmType_IntArray:
		return "IntArray";
	case xplmType_FloatArray:
		return "FloatArray";
	case xplmType_Data:
		return "Data";
	default:
		return "";
	}
}

std::string Dataref::GetValue()
{
	if (_dataref == NULL || _isValid == false || _dataType == xplmType_Unknown)
	{
		return "";
	}
	std::string val("");
	switch (_dataType)
	{
	case xplmType_Int:
		val = std::to_string(XPLMGetDatai(_dataref));
		return val;
	case xplmType_Float:
		val = std::to_string(XPLMGetDataf(_dataref));
		return val;
	case xplmType_Double:
		val = std::to_string(XPLMGetDatad(_dataref));
		return val;
	case xplmType_IntArray:
	{
		int arraySize = XPLMGetDatavi(_dataref, NULL, 0, 0);
		int* arrayVal = new int[arraySize];
		if (_index >= 0)
		{
			XPLMGetDatavi(_dataref, arrayVal, _index, 1);
			int value = arrayVal[0];
			return std::to_string(value);
		}
		XPLMGetDatavi(_dataref, arrayVal, 0, arraySize);
		json j = json::array();
		for (int i(0); i < arraySize; i++)
		{
			j.push_back(std::to_string(*(arrayVal + i)));
		}
		return j.dump();
	}
	case xplmType_FloatArray:
	{
		int arraySize = XPLMGetDatavf(_dataref, NULL, 0, 0);
		float* arrayVal = new float[arraySize];
		if (_index >= 0)
		{
			XPLMGetDatavf(_dataref, arrayVal, _index, 1);
			float value = arrayVal[0];
			return std::to_string(value);
		}
		XPLMGetDatavf(_dataref, arrayVal, 0, arraySize);
		json j = json::array();
		for (int i(0); i < arraySize; i++)
		{
			j.push_back(std::to_string(*(arrayVal + i)));
		}
		return j.dump();
	}
	case xplmType_Data:
	{
		int arraySize = XPLMGetDatab(_dataref, NULL, 0, 0);
		char* arrayVal = new char[arraySize];
		XPLMGetDatab(_dataref, arrayVal, 0, arraySize);
		return arrayVal;
	}
	default:
		break;
	}
	return "";
}

bool Dataref::SetValue(std::string value, bool forceReadOnly)
{

	Dataref::log.addToFile("[SET VALUE] of " + _link + " [STARTED]");
	if (!_isValid || _dataref == NULL || _dataType == xplmType_Unknown)
	{
		Dataref::log.addToFile("Dataref is not valid or link is null or type is not set");
		if (!_isValid)
		{
			Dataref::log.addToFile("Dataref is not valid");
		}
		if (_dataref == NULL)
		{
			Dataref::log.addToFile("Dataref is null");
		}
		if (_dataType == xplmType_Unknown)
		{
			Dataref::log.addToFile("Dataref type is unset");
		}
		return false;
	}
	if (forceReadOnly)
	{
		Dataref::log.addToFile("[SET VALUE] Updating Read Only");
		_isReadOnly = (bool)XPLMCanWriteDataRef(_dataref);
	}
	if (!_isReadOnly)
	{
		return false;
		Dataref::log.addToFile("Dataref is readonly");
	}
	Dataref::log.addToFile("[SET VALUE] Switching Type");
	switch (_dataType)
	{
	case xplmType_Int:
	{
		value = trim(value);
		Dataref::log.addToFile("[SET VALUE] Integer");
		int val = 0;
		try
		{
			val = std::stoi(value);
		}
		catch (const std::exception& e)
		{
			return false;
		}
		XPLMSetDatai(_dataref, val);
		Dataref::log.addToFile("Value = " + std::to_string(val));
		return true;
	}
	case xplmType_Float:
	{
		value = trim(value);
		Dataref::log.addToFile("[SET VALUE] Float");
		float val = 0.0f;
		Dataref::log.addToFile("Trying to parse |" + value + "|");
		try
		{
			val = std::stof(value);
		}
		catch (const std::exception& e)
		{
			Dataref::log.addToFile("[SET VALUE]Exception During parsing (stof): " + std::string(e.what()));
			Dataref::log.addToFile("[SET VALUE]Trying as int to convert to float");
			try
			{
				val = (float)std::stoi(value);
			}
			catch (const std::exception& e1)
			{
				Dataref::log.addToFile("[SET VALUE]Exception During parsing (stoi): " + std::string(e1.what()));
				return false;
			}
		}
		XPLMSetDataf(_dataref, val);
		Dataref::log.addToFile("Value = " + std::to_string(val));
		return true;
	}
	case xplmType_Double:
	{
		char* valChr = (char*)value.c_str();
		value = std::string(valChr);
		Dataref::log.addToFile("[SET VALUE] Double");
		double val = 0.0;
		try
		{
			val = std::stod(value);
		}
		catch (const std::exception& e)
		{
			return false;
		}
		XPLMSetDatad(_dataref, val);
		Dataref::log.addToFile("Value = " + std::to_string(val));
		return true;
	}
	case xplmType_IntArray:
	{
		Dataref::log.addToFile("[SET VALUE] Integer Array");
		json j = json::parse(value);
		if (_index < 0)
		{

			int arraySize = XPLMGetDatavi(_dataref, NULL, 0, 0);
			int* arrayVal = new int[arraySize];
			int maxElem = arraySize;
			if (j.size() < arraySize)
			{
				maxElem = j.size();
			}

			for (int i(0); i < maxElem; i++)
			{
				*(arrayVal + i) = std::stoi(j[i].get<std::string>());
			}
			XPLMSetDatavi(_dataref, arrayVal, 0, maxElem);
		}
		else
		{
			int val = std::stoi(j.get<std::string>());
			XPLMSetDatavi(_dataref, &val, _index, 1);
		}
		return true;
	}
	case xplmType_FloatArray:
	{
		Dataref::log.addToFile("[SET VALUE] FloatArray");
		json j = json::parse(value);
		if (_index < 0)
		{
			int arraySize = XPLMGetDatavf(_dataref, NULL, 0, 0);
			float* arrayVal = new float[arraySize];
			int maxElem = arraySize;
			if (j.size() < arraySize)
			{
				maxElem = j.size();
			}

			for (int i(0); i < maxElem; i++)
			{
				*(arrayVal + i) = std::stof(j[i].get<std::string>());
			}
			XPLMSetDatavf(_dataref, arrayVal, 0, maxElem);
		}
		else {
			float val = std::stof(j.get<std::string>());
			XPLMSetDatavf(_dataref, &val, _index, 1);
		}
		return true;
	}
	case xplmType_Data:
	{
		Dataref::log.addToFile("[SET VALUE] Data");
		return false;
	}
	default:
		return false;
	}
}

bool Dataref::IsReadOnly(bool ForceRecheck)
{
	if (ForceRecheck)
	{
		XPLMIsDataRefGood(_dataref);
	}
	return _isReadOnly;
}

void Dataref::XPLANELogException(const char* previousMessage, const std::exception& e)
{
	std::string output = std::string(previousMessage) + " " + e.what();
}