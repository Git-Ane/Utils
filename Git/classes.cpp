#include "classes.hpp"

using namespace std;
namespace fs = std::filesystem;
using namespace GitAne;

namespace GitAne{

    void create_dir(fs::path path){
        if(fs::create_directory(path)){cout << "Directory " << path.string() << " created successfully !" << endl;}
        else{throw invalid_argument(path.string() + " could not be created");}
    }

    ofstream create_file(fs::path path){
        if (!fs::exists(path)) {
            ofstream outputFile(path);
            if (outputFile.is_open()) {
                cout << "File " << path.string() << " created successfully !" << endl;
            }
            else {
                std::cerr << "Failed to create file " << path.string() << endl;
            }
            return outputFile;
        }
        else {
            throw invalid_argument ("File " + path.string() + " already exists");
        }
    }

    GitRepo::GitRepo(fs::path path, bool force){
        worktree = path;
        gitdir = path / ".git";         //on peut custom le nom du dossier
        if(!(force || fs::is_directory(gitdir))){
            throw invalid_argument(path.string() + " is not a Gitane Repository");
        }
    }

    fs::path GitRepo::get_gitdir(){
        return gitdir;
    }


    GitRepo repo_find(fs::path path){
        return GitRepo(path,false);
    }

    GitRepo create_repo(fs::path path){
        GitRepo repo = GitRepo(path, true);

        if(fs::exists(path)){
            if(!(fs::is_directory(path))){
                throw invalid_argument(path.string() + " is not a directory");
            }
            if(fs::exists(repo.get_gitdir()) && !fs::is_empty(repo.get_gitdir())){
                throw invalid_argument(path.string() + " is already a Gitane directory");
            }

        }
        else{
            create_dir(path);
        }

        create_dir(repo.get_gitdir());

        ofstream descr = create_file(repo.get_gitdir() / "descr");
        descr << "Bonjour je suis une description";
        descr.close();

        ofstream head = create_file(repo.get_gitdir() / "HEAD");
        head << "ref: refs/heads/master"; // jsp ce que ça veut dire lol
        head.close();

        create_dir(repo.get_gitdir() / "branches");
        create_dir(repo.get_gitdir() / "objects");
        create_dir(repo.get_gitdir() / "refs");
        create_dir(repo.get_gitdir() / "refs/tags");
        create_dir(repo.get_gitdir() / "refs/heads");

        ofstream conf = create_file(repo.get_gitdir() / "conf");
        conf << "# write what you want here, we will carefully ignore it.";

        conf.close();
        

        return repo;
    }

    

    bool write_to_git_objects(const std::string &file_path, GitRepo rep) {
        std::ifstream input_file(file_path, std::ios::binary);
        if (!input_file) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier " << file_path << std::endl;
            return false;
        }

        std::stringstream buffer;
        buffer << input_file.rdbuf();
        std::string content = buffer.str();
        std::string hash = sha1(content);

        fs::path git_objects_folder = rep.get_gitdir()/ "objects"/ hash.substr(0, 2);
        if(fs::exists(git_objects_folder)){
            if(fs::exists(git_objects_folder / hash.substr(2))){
                std::cerr << "COLLISION DE SHA, IMPOSSIBLE ?! NANI ! LA FIN DU MODNE ! LE BITCOIN S'EFFONDRE nfaie hffiezf";
                return false;
            }
        }
        else{
            create_dir(git_objects_folder);
        }
        ofstream f = create_file(git_objects_folder /   hash.substr(2));
        f << content; // Je suis pas sûr qu'on doive mettre le contenu ici, à voir.
        f.close();
        return true;
    }

    GitObject::GitObject(string data){
        if(data!=""){
            deserialize(data);
        }
        else{
            init();
        }
    }

    void GitObject::init(){}


}