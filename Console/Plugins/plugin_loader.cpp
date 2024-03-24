#include "plugin_loader.hpp"


namespace GitAne{

    void plugin_loader(){
        GitAne::Joke::joke_plugin_loader();
        GitAne::Debug::debug_plugin_loader();
    }

}