#include "include/HTTPRequest.hpp"
#include<iostream>
#include "string.h"
using namespace std;

namespace GitAne{

class NetClient {
    public:
        string server_url;
        NetClient(string s,string e, string m){};
        std::unordered_map<std::string, std::string> sendFile(string proj_name, string file_name, string file_content);
        std::unordered_map<std::string, std::string> receiveFile(string proj_name, string file_name);
    private:
        string token;
        string email;
    
    

};

}

