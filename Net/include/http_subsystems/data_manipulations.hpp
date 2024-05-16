#ifndef base
#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <curl/curl.h>
#include<string>
#include<openssl/sha.h>
#include <map>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include<iomanip>
#endif 

namespace GitAne::Net {
     std::string extractGitParams(const std::string& input);
      std::map<std::string, std::string> parseQueryString(const std::string& queryString);

      std::string url_encode(const std::string& decoded);

        std::string url_decode(const std::string& encoded);
        std::string sha1(const std::string &input);

}