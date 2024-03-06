#ifndef base
#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#endif 
namespace GitAne::Net{
    std::string buildOkPage(std::string title, std::string body);
    std::string buildCreatedPage(std::string title, std::string body);
    std::string buildAcceptedPage(std::string title, std::string body);
    std::string buildBadRequestPage(std::string title, std::string body);
    std::string buildUnauthorizedRequestPage(std::string title, std::string body);
    std::string buildNotFoundPage(std::string title, std::string body);
    std::string buildInternalServerErrorPage(std::string title, std::string body);
    std::string buildNotImplementedPage(std::string title, std::string body);
}