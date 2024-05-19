#include "../../Console/commands.hpp"
#include "../../Net/client.hpp"
#include "gitting.hpp"
#include<iostream>
#include <vector>
#include<string>


namespace GitAne{
    namespace Client{
        void connect(vector<string> arg);
        void add(vector<string> arg);
        void remove_server(vector<string> arg);
        std::pair<std::string, std::string> url_and_token();
        void client_plugin_loader();
        
    }
}
