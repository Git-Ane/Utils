#ifndef base
#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#endif 
#include <iostream>
#include "../json.hpp"
#ifndef DATA_MANIP_ET_RESPONSE_BUILDER
#include "data_manipulations.hpp"
#include "response_builder.hpp"
#endif
namespace GitAne::Net
{  
    std::string buildLoginResponse(std::string method, std::string args);
    std::string buildRegisterResponse(std::string method, std::string args);

}