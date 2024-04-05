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

    

    bool write_to_git_object(GitRepo repo, GitObject& obj) {
        /*
        * Entrée: Réf d'un GitObj
        * Sortie: True si on a pu l'écrire à l'URL donnée par le SHA
        */
        // Normalement ça marche et c'est polymorphe. On utilise le fait que la référence va permettre au compilo de savoir quelle variable "fmt" chercher
        // de même pour serialize.

        string content = obj.serialize(repo);

        std::vector<unsigned char> result;
        string fmt = obj.get_format();
        result.insert(result.end(), fmt.begin(), fmt.end());
        result.push_back(' '); // Space character
        std::string lenStr = std::to_string(content.length());
        result.insert(result.end(), lenStr.begin(), lenStr.end());
        result.push_back('\0'); // Null character
        result.insert(result.end(), content.begin(), content.end());

        std::string resultString(result.begin(), result.end());
        std::cout << "On obtient: " << resultString << std::endl;
        std::string hash = sha1(resultString);

        fs::path git_objects_folder = repo.get_gitdir() / "objects" / hash.substr(0, 2);
        if (fs::exists(git_objects_folder)) {
            if (fs::exists(git_objects_folder / hash.substr(2))) {
                std::cerr << "COLLISION DE SHA, IMPOSSIBLE ?! NANI ! LA FIN DU MODNE ! LE BITCOIN S'EFFONDRE nfaie hffiezf";
                return false;
            }
        } else {
            create_dir(git_objects_folder);
        }
        ofstream f = create_file(git_objects_folder / hash.substr(2));
        f << resultString; // Write the data to file
        f.close();
        return true;
    }

    GitObject::GitObject(string data){
        if(data!=""){
            //deserialize(data);
        }
        else{
            init();
        }
    }

    string GitObject::get_format(){
        return "";
    }
    string GitBlob::get_format(){
        return "blob";
    }
    string GitCommit::get_format(){
        return "commit";
    }
    string GitTree::get_format(){
        return "tree";
    }
    string GitTag::get_format(){
        return "tag";
    }

    void GitObject::init(){

    }

    string GitObject::serialize(GitRepo repo){throw logic_error("Pas implemente");}

    void GitObject::deserialize(string data){throw logic_error("Pas implemente");}

    string GitBlob::serialize(GitRepo repo){
        return blobdata;
    }

    void GitBlob::deserialize(string data){
        blobdata = data;
    }

    string GitCommit::serialize(GitRepo repo){}

    void GitCommit::deserialize(string data){}

    string GitTag::serialize(GitRepo repo){}

    void GitTag::deserialize(string data){}

    string GitTree::serialize(GitRepo repo){}

    void GitTree::deserialize(string data){}

    GitObject& read_object(GitRepo repo, string sha) { // Essayer avec ça. Si ça marhe pas, las passer en void, la faire prendre un GitObject* et le set dedans. On peut faire une variable qui indique le type et très facilement vérifeir le type de la variable de retour avec un Switch.        /*
        /*
        * Entrée: un repo, un sha
        * Sortie: l'objet associé (sous forme de GitBlob*)
        */
        fs::path path = repo.get_gitdir() / "objects" / sha.substr(0, 2) / sha.substr(2, sha.size() - 2);

        if (!(fs::exists(path))) {
            throw invalid_argument(path.string() + " does not exist !");
        }

        ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf(); // Read the entire file into the stringstream buffer

        std::string content = buffer.str(); // Convert the buffer to a string

        std::cout << "Content of the file:" << std::endl;
        std::cout << content << std::endl;

        file.close();

        std::string raw = buffer.str();
        // Read object type
        size_t x = raw.find(' ');
        std::string fmt = raw.substr(0, x);

        // Read and validate object size
        size_t y = raw.find('\x00', x);
        int size = std::stoi(raw.substr(x, y - x));
        if (static_cast<uLong>(size) != raw.size() - y - 1) {
            throw std::runtime_error("Malformed object " + sha + ": bad length");
        }

        content = raw.substr(y + 1);

        // Pick constructor based on object type
        
        if (fmt == "blob") {
            static GitBlob obj = GitBlob(content);
            obj.deserialize(content);
            return obj;
        } else {
            throw std::runtime_error("Unknown type " + fmt + " for object " + sha);
        }
        
    }


}