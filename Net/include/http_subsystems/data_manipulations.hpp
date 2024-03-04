#ifndef base
#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#endif 

namespace GitAne::Net {
     std::string extractGitParams(const std::string& input);
      std::map<std::string, std::string> parseQueryString(const std::string& queryString);

}