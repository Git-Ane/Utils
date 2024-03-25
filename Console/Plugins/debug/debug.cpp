#include "debug.hpp"
using namespace GitAne::Console;



namespace GitAne{

    namespace Debug{
        void hash(vector<string> args){
            if(args.size() < 1){
                cerr << "Hash a besoin d'un nom de fonction.";
            }
            cout << "Hash of your file: " << GitAne::sha1(args[0]);
        }


        void debug_plugin_loader(){
            initPlugin("debug");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
        }
    }

}


