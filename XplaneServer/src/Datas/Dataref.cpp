#include "Dataref.hpp"


bool Dataref::ParseJSON(std::string input, Json::Value& json)
{
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::string errors;

	return reader->parse(input.c_str(), input.c_str() + input.size(), &json, &errors);
}

Dataref::Dataref() : _isValid(false), _isReadOnly(true), _link(""), _dataType(xplmType_Unknown), _dataref(NULL)
{
	_log = Logging("Dataref_Manager");
}

Dataref::Dataref(std::string link, XPLMDataTypeID dataType) : _link(link), _dataType(dataType), _dataref(NULL)
{
	_log = Logging("Dataref_Manager");
	if (_dataType == xplmType_Unknown || link.compare("") == 0)
	{
		_isValid = false;
		_isReadOnly = true;
		_dataType = xplmType_Unknown;
		return;
	}
	_dataref = XPLMFindDataRef(_link.c_str());
	if (_dataref == NULL)
	{
		_isValid = false;
		_isReadOnly = true;
		_dataType = xplmType_Unknown;
		return;
	}
	_isValid = true;
	_isReadOnly = (bool)XPLMCanWriteDataRef(_dataref);
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

std::string Dataref::GetValue()
{
	if (_dataref == NULL || _isValid == false || _dataType == xplmType_Unknown)
	{
		return "";
	}
	switch (_dataType)
	{
	case xplmType_Int:
		return std::to_string(XPLMGetDatai(_dataref));
	case xplmType_Float:
		return std::to_string(XPLMGetDataf(_dataref));
	case xplmType_Double:
		return std::to_string(XPLMGetDatad(_dataref));
	case xplmType_IntArray:
	{
		int arraySize = XPLMGetDatavi(_dataref, NULL, 0, 0);
		int* arrayVal = new int[arraySize];
		XPLMGetDatavi(_dataref, arrayVal, 0, arraySize);
		std::string output = "[";
		for (int i(0); i < arraySize - 1; i++)
		{
			output += std::to_string(*(arrayVal + (i * sizeof(int)))) + ",";
		}
		output += std::to_string(*(arrayVal + (arraySize * sizeof(int)))) + "]";
		return output;
	}
	case xplmType_FloatArray:
	{
		int arraySize = XPLMGetDatavf(_dataref, NULL, 0, 0);
		float* arrayVal = new float[arraySize];
		XPLMGetDatavf(_dataref, arrayVal, 0, arraySize);
		std::string output = "[";
		for (int i(0); i < arraySize - 1; i++)
		{
			output += std::to_string(*(arrayVal + (i * sizeof(int)))) + ",";
		}
		output += std::to_string(*(arrayVal + (arraySize * sizeof(int)))) + "]";
		return output;
	}
	case xplmType_Data:
	{
		/*int arraySize = XPLMGetDatab(_dataref, NULL, 0, 0);
		float* arrayVal = new float[arraySize];
		XPLMGetDatavb(_dataref, arrayVal, 0, arraySize);
		std::string output = "[";
		for (int i(0); i < arraySize - 1; i++)
		{
			output += std::to_string(*(arrayVal + (i * sizeof(int)))) + ",";
		}
		output += std::to_string(*(arrayVal + (arraySize * sizeof(int)))) + "]";
		return output;*/
		return "";
	}
	default:
		break;
	}
	return "";
}

bool Dataref::SetValue(std::string value, bool forceReadOnly)
{
	if (!_isValid || _dataref == NULL || _dataType == xplmType_Unknown)
	{
		return false;
	}
	if (forceReadOnly)
	{
		_isReadOnly = (bool)XPLMCanWriteDataRef(_dataref);
	}
	if (!_isReadOnly)
	{
		return false;
	}

	switch (_dataType)
	{
	case xplmType_Int:
	{
		int val = 0;
		try
		{
			val = std::stoi(value);
		}
		catch (const std::exception& e)
		{
#ifdef _DEBUG
			XPLANELogException("Unable to parse value to integer", e);
#endif // _DEBUG
			return false;
		}
		XPLMSetDatai(_dataref, val);
		return true;
	}
	case xplmType_Float:
	{
		float val = 0.0f;
		try
		{
			val = std::stof(value);
		}
		catch (const std::exception& e)
		{
#ifdef _DEBUG
			XPLANELogException("Unable to parse value to float", e);
#endif // _DEBUG
			return false;
		}
		XPLMSetDataf(_dataref, val);
		return true;
	}
	case xplmType_Double:
	{
		double val = 0.0;
		try
		{
			val = std::stod(value);
		}
		catch (const std::exception& e)
		{
#ifdef _DEBUG
			XPLANELogException("Unable to parse value to double", e);
#endif // _DEBUG
			return false;
		}
		XPLMSetDatad(_dataref, val);
		return true;
	}
	case xplmType_IntArray:
	{
		int arraySize = XPLMGetDatavi(_dataref, NULL, 0, 0);
		Json::Value jParser;
		ParseJSON(value, jParser);
		Json::ValueType val = jParser.type();
		int maxSize = ((unsigned int)arraySize > jParser.size()) ? jParser.size() : arraySize;
		int* values = new int[maxSize];
		for (int i(0); i < maxSize; i++)
		{
			*(values + (i * sizeof(int))) = jParser[i].asInt();
		}
		XPLMSetDatavi(_dataref, values, 0, maxSize);
		return true;
	}
	case xplmType_FloatArray:
	{
		int arraySize = XPLMGetDatavi(_dataref, NULL, 0, 0);
		Json::Value jParser;
		ParseJSON(value, jParser);
		Json::ValueType val = jParser.type();
		int maxSize = ((unsigned int)arraySize > jParser.size()) ? jParser.size() : arraySize;
		float* values = new float[maxSize];
		for (int i(0); i < maxSize; i++)
		{
			*(values + (i * sizeof(float))) = jParser[i].asFloat();
		}
		XPLMSetDatavf(_dataref, values, 0, maxSize);
		return true;
	}
	case xplmType_Data:
	{
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