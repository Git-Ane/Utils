#include "debug.hpp"
#include <unordered_map>
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
            std::cout << "La je set le fmt à " << blob.fmt << std::endl; 
            ifstream file(args[0]);
            std::stringstream buffer;
            buffer << file.rdbuf(); // Read the entire file into the stringstream buffer
            std::string content = buffer.str();
            blob.deserialize(content);
            write_to_git_object(repo,blob); // Pass the pointer to blob
            cout << "File written successfully !" << endl;
            
        }



        void read_object_fun(vector<string> args){
            GitRepo repo = repo_find("");
            GitObject& b = read_object(repo,args[0],true);
            cout << b.serialize(repo) << endl;
        }

        void head_sha(vector<string> args){
            GitRepo repo = repo_find("");
            cout << get_head(repo,true) << endl;
        }

        void listFilesFun(vector<string>){
            vector<string> files = listFiles();
            cout << "=== START LIST FILES ===" << endl << endl;
            for(int i=0;i<files.size();i++){
                cout << files[i] << endl;
            }
            cout << endl << "=== END LIST FILES ===" << endl;
        }



        void debug_plugin_loader(){
            initPlugin("debug");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
            addCommand("write_blob",&write_file,"writes file arg1 in .git folder as a blob",1,1);
            addCommand("read_blob",&read_object_fun,"reads blob with sha of args[0]",1,1);
            addCommand("get_head_sha",&head_sha,"get sha of head",0,0);
            addCommand("list_files",&listFilesFun,"list files present in the working directory",0,0);
        }
    }

}


