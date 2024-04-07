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
        create_dir(repo.get_gitdir() / "tracked"); // contient les fichiers à suivre par branche.
        create_file(repo.get_gitdir() / "tracked" / "main"); // la branche par défaut
        ofstream conf = create_file(repo.get_gitdir() / "conf");
        conf << "# write what you want here, we will carefully ignore it.";

        conf.close();
        

        return repo;
    }

    void track_file(vector<string> args){
        GitRepo repo = repo_find("");
        fs::path path = repo.get_gitdir() / "tracked" / "main";  // TODO: changer le main par la branche active
        std::ifstream fichier(path);
        if (!fichier.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier " << path << std::endl;
            return;
        }

        // Vérifier si le nom est déjà présent dans le fichier
        std::string ligne;
        while (std::getline(fichier, ligne)) {
            if (ligne == args[0]) {
                // Le nom est déjà présent, donc nous n'avons rien à faire
                std::cout << "Le nom est déjà présent dans le fichier." << std::endl;
                return;
            }
        }

        // Le nom n'est pas présent, donc nous l'ajoutons à la fin du fichier
        std::ofstream fichier_sortie(path, std::ios_base::app);
        if (!fichier_sortie.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier en écriture." << std::endl;
            return;
        }

        fichier_sortie << args[0] << std::endl;
        std::cout << args[0] << " tracked successfuly.";
        return;

    }

    void untrack_file(vector<string> args){
        GitRepo repo = repo_find("");
        fs::path path = repo.get_gitdir() / "tracked" / "main";  // TODO: changer le main par la branche active
        std::ifstream fichier(path);
        if (!fichier.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier " << path << std::endl;
            return;
        }

        // Lire toutes les lignes du fichier dans un vecteur
        std::vector<std::string> lignes;
        std::string ligne;
        while (std::getline(fichier, ligne)) {
            lignes.push_back(ligne);
        }

        fichier.close();

        // Reouvrir le fichier pour l'écriture
        std::ofstream fichier_sortie(path);
        if (!fichier_sortie.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier en écriture." << std::endl;
            return;
        }

        // Parcourir le vecteur de lignes, écrire toutes les lignes sauf celle contenant le nom
        bool nomTrouve = false;
        for (const auto& l : lignes) {
            if (l == args[0]) {
                nomTrouve = true;
            } else {
                fichier_sortie << l << std::endl;
            }
        }

        if (nomTrouve) {
            std::cout << args[0] << " untracked successfuly.";
            return;
        } else {
            std::cout << "Le nom n'a pas été trouvé dans le fichier." << std::endl;
        }
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
        std::string hash = sha1(resultString);

        fs::path git_objects_folder = repo.get_gitdir() / "objects" / hash.substr(0, 2);
        if (fs::exists(git_objects_folder)) {
            if (fs::exists(git_objects_folder / hash.substr(2))) {
                std::cout << "File did not changed.";
                return true;
            }
        } else {
            create_dir(git_objects_folder);
        }
        ofstream f = create_file(git_objects_folder / hash.substr(2));
        f << resultString; // Write the data to file
        f.close();
        return true;
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

    unordered_map<string, string> kvlm_parse(const string& raw) {
    unordered_map<string, string> dct;

    size_t start = 0;

    while (start < raw.size()) {
        size_t spc = raw.find(' ', start);
        size_t nl = raw.find('\n', start);

        // Base case
        if (spc == string::npos || nl < spc) {
            dct[string()] = raw.substr(start + 1);
            break;
        }

        string key = raw.substr(start, spc - start);

        size_t end = start;
        while (true) {
            end = raw.find('\n', end + 1);
            if (raw[end + 1] != ' ' || end == string::npos) break;
        }

        string value = raw.substr(spc + 1, (end != string::npos ? end : raw.size()) - spc - 1);
        size_t pos;
        while ((pos = value.find("\n ")) != string::npos) {
            value.replace(pos, 2, "\n");
        }

        if (dct.find(key) != dct.end()) {
            dct[key] = dct[key] + "\n" + value;
        } else {
            dct[key] = value;
        }

        if (end == string::npos) break;
        start = end + 1;
    }

    return dct;
}

    string kvlm_serialize(const unordered_map<string, string>& kvlm) {
        string ret = "";
        // Output fields
        for (const auto& pair : kvlm) {
            // Skip the message itself
            if (pair.first == "") continue;

            // Normalize to a list
            vector<string> val;
            string valCopy = pair.second; // Make a copy to allow modification
            if (valCopy.find("\n") != string::npos) {
                size_t pos = 0;
                while ((pos = valCopy.find("\n", pos)) != string::npos) {
                    valCopy.replace(pos, 1, "\n ");
                    pos += 2;
                }
            }
            val.push_back(valCopy);

            for (const auto& v : val) {
                ret += pair.first + " " + v + "\n";
            }
        }

        // Append message if it exists
        auto it = kvlm.find("");
        if (it != kvlm.end()) {
            ret += "\n" + it->second + "\n";
        }

        // cout<< "serialized klvm into :" << endl << ret <<endl;

        return ret;
    }

    string GitCommit::serialize(GitRepo repo){
        return kvlm_serialize(kvlm);
    }

    void GitCommit::deserialize(string data){
        kvlm = kvlm_parse(data);
    }

    string GitTag::serialize(GitRepo repo){}

    void GitTag::deserialize(string data){}

    string GitTree::serialize(GitRepo repo){}

    void GitTree::deserialize(string data){}

    GitObject& read_object(GitRepo repo, string sha) { // Essayer avec ça. Si ça marhe pas, las passer en void, la faire prendre un GitObject* et le set dedans. On peut faire une variable qui indique le type et très facilement vérifeir le type de la variable de retour avec un Switch.        /*
        /*
        * Entrée: un repo, un sha
        * Sortie: l'objet associé (sous forme de GitObject*)
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