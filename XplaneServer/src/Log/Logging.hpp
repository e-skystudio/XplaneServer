#pragma once

#include <fstream>
#include <ostream>
#include <string>
#include <ctime>

class Logging
{
public:
	Logging();
	Logging(std::string basename);

	void addToFile(std::string data);
	void operator<<(std::string data);
protected:
	std::ofstream _file;
private:
	std::string get_time_stamp();
};

