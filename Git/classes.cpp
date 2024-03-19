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

        return repo;
    }


}