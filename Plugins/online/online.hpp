#include "../../Console/commands.hpp"
#include "../../SHA-warma/shawarma.hpp"
#include "../../Git/classes.hpp"
#include<iostream>
#include <vector>
#include<string>
#include"../../Net/client.hpp"

namespace GitAne{

    namespace Online{
        void connect(vector<string> arg);
        void add(vector<string> arg);
        void remove_server(vector<string> arg);
        void online_plugin_loader();
    }


}