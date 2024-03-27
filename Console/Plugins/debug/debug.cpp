#include "debug.hpp"
using namespace GitAne::Console;
using namespace GitAne;



namespace GitAne{

    namespace Debug{
        void hash(vector<string> args){
            if(args.size() < 1){
                cerr << "Hash a besoin d'un nom de fonction.";
            }
            cout << "Hash of your file: " << GitAne::sha1(args[0]) << endl;
        }

        void write_file(vector<string> args){
            GitRepo repo = repo_find("");
            if(write_to_git_object( args[0],repo)){cout<< "File written successfully !" << endl;}
        }


        void debug_plugin_loader(){
            initPlugin("debug");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
            addCommand("write_file",&write_file,"writes file arg1 in .git folder",1,1);
        }
    }

}


