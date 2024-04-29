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
    std::string buildReceiveFileResponse(std::string method, std::string args);
    std::string buildSendFileResponse(std::string method, std::string args);
    std::string buildLaMuleResponse(std::string method, std::string args);
}