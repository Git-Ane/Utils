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
            if(write_to_git_object(repo,blob)){ // Pass the pointer to blob
                cout << "File written successfully !" << endl;
            }
        }



        void read_object_fun(vector<string> args){
            GitRepo repo = repo_find("");
            GitObject& b = read_object(repo,args[0]);
            cout << b.serialize(repo) << endl;
        }

        void write_commit(vector<string> args){
            GitRepo repo = repo_find("");
            GitCommit c;
            
            std::vector<std::string> result;

            std::ifstream fichier(repo.get_gitdir() / "tracked" / "main");
            if (!fichier.is_open()) {
                std::cerr << "Error: Can not open tracked files' file." << std::endl;
                return;
            }

            std::string ligne;
            while (std::getline(fichier, ligne)) {
                result.push_back(ligne);
            }
            fichier.close();
            unordered_map<string, string> k;
            
            cout << "=== START COMMIT ===" << endl;
            for (std::string& element : result) {
                cout << "Found " << element << endl;
                std::vector<unsigned char> a_hash;
                ifstream file(repo.get_gitdir() / ".." / element);
                std::stringstream buffer;
                buffer << file.rdbuf(); // Read the entire file into the stringstream buffer
                std::string content = buffer.str();
                string fmt = "blob";

                a_hash.insert(a_hash.end(), fmt.begin(), fmt.end());
                a_hash.push_back(' '); // Space character
                std::string lenStr = std::to_string(content.length());
                a_hash.insert(a_hash.end(), lenStr.begin(), lenStr.end());
                a_hash.push_back('\0'); // Null character
                a_hash.insert(a_hash.end(), content.begin(), content.end());
                std::string str_a_hash(a_hash.begin(),a_hash.end());
                std::string res_sha = GitAne::sha1(str_a_hash);
                cout << "    Hashed into: " << str_a_hash << endl;
                k.insert(std::make_pair(element, res_sha));
                GitBlob a_ajouter(str_a_hash);
                a_ajouter.deserialize(str_a_hash); // si on le met pas ça met 0, faut vraiment utiliser full fonctions quand on utilise des réfs ...
                write_to_git_object(repo,a_ajouter);
            }
            cout<<"Explored all files to add."<<endl;
            cout << "Writting the commit..." << endl;
            c.deserialize(kvlm_serialize(k));
            write_to_git_object(repo,c);
            cout << "=== END COMMIT ===" << endl;
            
        }


        void debug_plugin_loader(){
            initPlugin("debug");
            addCommand("hash",&hash,"print the hash value of a specific file",1,1);
            addCommand("write_blob",&write_file,"writes file arg1 in .git folder as a blob",1,1);
            addCommand("read_blob",&read_object_fun,"reads blob with sha of args[0]",1,1);
            addCommand("write_commit",&write_commit,"writes commit caca=args[0], prout=args[1]",2,2);
        }
    }

}


