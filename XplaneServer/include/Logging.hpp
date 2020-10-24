#pragma once

#include <iostream>
#include <chrono>
#include <ctime>  
#include <fstream>
#include <string>

class Logging
{
public:
	Logging(std::string base_name);
	void AddToFile(std::string message);
protected:
	std::string _filepath;
	std::string get_time_stamp();
};