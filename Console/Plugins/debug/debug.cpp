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
            GitBlob blob("prout"); // Create a GitBlob object with content "prout"
            blob.deserialize(args[0]);
            if(write_to_git_object(repo,&blob)){ // Pass the pointer to blob
                cout << "File written successfully !" << endl;
            }
        }



        void read_object(vector<string> args){
            GitRepo repo = repo_find("");

        }


        void debug_plugin_loader(){
            initPlugin("debug");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
            addCommand("write_file",&write_file,"writes file arg1 in .git folder as a blob",1,1);
        }
    }

}


