#include <iostream>
#include <regex>
#include <TCPServer.hpp>
#include <UDPEmiter.h>
#include <Logging.hpp>
Logging loger;
#include "nlohmann/json.hpp"
using json = nlohmann::json;

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

int main(int argc, char* argv[])
{
    std::string value = "[0,750,0,750,0,0,0,0,0]";
    value = trim(value);
    json j = json::parse(value);
    for (int i(0); i < j.size(); i++)
    {
        std::cout << j[i].get<float>() << "\n";
    }
    return 0;
}

