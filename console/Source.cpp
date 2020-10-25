#include <iostream>
#include <regex>

std::string get_operation(std::string input)
{
	std::regex regexPattern("[\\\'\\\"]Ops[\\\'\\\"]\\ ?:\\ ?[\\\'\\\"](\\b\\w+\\b)[\\\'\\\"]");
	std::smatch sm;

	bool res = regex_search(input, sm, regexPattern);
	if (!res)
	{
		return "";
	}
	return sm[1].str();
}

int main()
{
	std::string data = "{\"Ops\": \"REGISTER_DATAREF\", \"Name\": \"longitude\", \"Link\": \"sim/flightmodel/position/longitude\", \"Type\": \"float\"}";

	std::cout << get_operation(data) << std::endl;

	return 0;
}