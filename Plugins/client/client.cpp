#include "client.hpp"

using namespace GitAne::Console;
using namespace std;

namespace GitAne{
    namespace Client{

        //GitAne::NetClient cTest("localhost:8087","test@test.com","testest");
        void log_fun(vector<string> args){
            GitAne::NetClient cTest(args[0],args[1],args[2]);
            cTest.sendFile("caca","caca.txt","je suis un gros caca");
        }


        void client_plugin_loader(){
            initPlugin("client");
            addCommand("log",&log_fun, "log to server",3,3);
        }
    }
}

