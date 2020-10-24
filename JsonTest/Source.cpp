#include <json/json.h>
#include <iostream>




int main()
{
    
    parseJson("{\"Ops\": \"REGISTER_DATAREF\", \"Link\": \"sim / flightmodel / position / elevation\", \"Type\": \"float\", \"Name\": \"elevation\"}", json);
    std::cout << json["Ops"] << std::endl;
    return 0;
}
