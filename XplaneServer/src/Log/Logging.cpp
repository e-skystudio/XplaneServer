#include "Logging.hpp"

Logging::Logging()
{
}

Logging::Logging(std::string basename)
{
	_file.open(basename + ".log", std::ios::out | std::ios::app);
}

void Logging::addToFile(std::string data)
{
    _file << get_time_stamp() << "\t" << data << "\n" << std::flush;
}

void Logging::operator<<(std::string data)
{
    addToFile(data);
}

std::string Logging::get_time_stamp()
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::string timestamp = std::to_string(now->tm_year + 1900) + "/" +
        std::to_string(now->tm_mon) + "/" +
        std::to_string(now->tm_mday) + " " +
        std::to_string(now->tm_hour) + " " +
        std::to_string(now->tm_hour) + ":" +
        std::to_string(now->tm_min) + ":" +
        std::to_string(now->tm_sec);
    return timestamp;
}
