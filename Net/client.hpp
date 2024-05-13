#include "include/HTTPRequest.hpp"
#include<iostream>
#include "string.h"
using namespace std;

namespace GitAne{

class NetClient {
    public:
        string server_url;
        string token;
        NetClient(string s,string e, string m, bool reg);
        unordered_map<std::string, std::string> parseHTTPResponse(const std::string& httpResponse);
        unordered_map<std::string, std::string> body_to_args(const std::string& body);
        std::unordered_map<std::string, std::string> sendFile(string proj_name, string file_name, string file_content);
        std::unordered_map<std::string, std::string> receiveFile(string proj_name, string file_name);
    private:
        
        string email;
    
    

};

}

