#include "plugin_loader.hpp"


namespace GitAne{

    void plugin_loader(){
        Joke::joke_plugin_loader();
        Debug::debug_plugin_loader();
        Merge::merge_plugin_loader();
        Client::client_plugin_loader();
        Date::date_loader();
    }

}