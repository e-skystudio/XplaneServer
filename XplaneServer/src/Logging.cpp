#include "..\include\Logging.hpp"

Logging::Logging(std::string base_name)
{
	_filepath = "./" + base_name + ".log";
}

void Logging::AddToFile(std::string message)
{
	std::ofstream file;
	file.open(_filepath, std::ios::out | std::ios::app);
	file << get_time_stamp() << message << "\n";
#ifdef _DEBUG
	std::cout << get_time_stamp() << message << "\n";
#endif // _DEBUG
	file.close();
}

std::string Logging::get_time_stamp()
{
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	return std::to_string(now->tm_year + 1900) + "/" + std::to_string(now->tm_mon) + "/" +
		std::to_string(now->tm_mday) + " " + std::to_string(now->tm_hour) + ":" +
		std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
}
