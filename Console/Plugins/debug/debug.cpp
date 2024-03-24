using namespace std;
#include "debug.hpp"
using namespace GitAne::Console;



namespace GitAne{

    namespace Joke{
        void hash(vector<string> args){
            if(args.size() < 1){
                cerr << "Hash a besoin d'un nom de fonction.";
            }
            
        }


        void debug_plugin_loader(){
            initPlugin("joke");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
        }
    }

}


