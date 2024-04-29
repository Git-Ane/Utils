#include "classes.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

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
        gitdir = path / ".gac";         //on peut custom le nom du dossier
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
        head << "none";
        head.close();

        ofstream active_branch_file = create_file(repo.get_gitdir() / "active_branch");
        active_branch_file << "main";
        active_branch_file.close();

        ofstream branches = create_file(repo.get_gitdir() / "branches");
        branches << "main none";
        branches.close();

        create_dir(repo.get_gitdir() / "objects");
        create_dir(repo.get_gitdir() / "refs");
        create_dir(repo.get_gitdir() / "refs/tags");
        create_dir(repo.get_gitdir() / "refs/heads");
        create_file(repo.get_gitdir() / "tracked"); 
        ofstream conf = create_file(repo.get_gitdir() / "conf");
        conf << "# write what you want here, we will carefully ignore it.";

        conf.close();
        

        return repo;
    }

    void track_file(vector<string> args){
        GitRepo repo = repo_find("");
        if(!fs::exists(args[0])){
            throw(invalid_argument("File " + args[0] + " doesn't exist"));
        }
        fs::path path = repo.get_gitdir() / "tracked"; 
        std::ifstream fichier(path);
        if (!fichier.is_open()) {
            std::cerr << "Error : can't open file " << path << std::endl;
            return;
        }

        // Vérifier si le nom est déjà présent dans le fichier
        std::string ligne;
        while (std::getline(fichier, ligne)) {
            if (ligne == args[0]) {
                // Le nom est déjà présent, donc nous n'avons rien à faire
                std::cout << "File " << args[0] << " is already tracked" << std::endl;
                return;
            }
        }

        // Le nom n'est pas présent, donc nous l'ajoutons à la fin du fichier
        std::ofstream fichier_sortie(path, std::ios_base::app);
        if (!fichier_sortie.is_open()) {
            std::cerr << "Error : can't write in file " << path << std::endl;
            return;
        }

        fichier_sortie << args[0] << std::endl;
        std::cout << args[0] << " tracked successfuly." << endl;
        return;

    }

    void untrack_file(vector<string> args){
        GitRepo repo = repo_find("");
        fs::path path = repo.get_gitdir() / "tracked";  // TODO: changer le main par la branche active
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
            std::cerr << "Error : can't write in file " << path << std::endl;
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
            std::cout << args[0] << " untracked successfuly." << endl;
            return;
        } else {
            std::cout << "File " << args[0] << " is not tracked" << std::endl;
        }
    }

    string get_hash_of_branch(string nom, bool ignore_temporary){
        GitRepo r = repo_find("");
        string s = get_file_content(r.get_gitdir() / "branches");
        unordered_map<string,string> k = kvlm_parse(s);
        string sha = k[nom];
        k = kvlm_parse(read_object(r,sha));
        if(k["#temporary"]=="true"){return k["#parent"];} 
        else{return sha;}
    }

    unordered_map<string, string> list_files_in_branch(string hash){
        unordered_map<string, string> files_map;
        fs::path objects_dir = repo_find("").get_gitdir() / "objects" / hash.substr(0, 2) / hash.substr(2);
        ifstream file(objects_dir);
        string line;

        // Ignorer les trois premières lignes
        getline(file, line);
        getline(file, line);
        getline(file, line);

        // Lire le nom du fichier et son hash et les stocker dans la map
        while (getline(file, line)) {
            istringstream iss(line);
            string name, file_hash;
            if (iss >> name >> file_hash) {
                files_map[name] = file_hash;
            }
        }

        return files_map;
    }

    


    string write_to_git_object(GitRepo repo, GitObject& obj) {
        /*
        * Entrée: Réf d'un GitObj
        * Sortie: le sha
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
                return hash;
            }
        } else {
            create_dir(git_objects_folder);
        }
        ofstream f = create_file(git_objects_folder / hash.substr(2));
        f << resultString; // Write the data to file
        f.close();
        return hash;
    }


    /*! \brief get le vector des fichiers tracked 
    */
    vector<string> get_tracked_files(GitRepo repo){
        std::vector<std::string> result;


        std::ifstream fichier(repo.get_gitdir() / "tracked");
        if (!fichier.is_open()) {
            throw(invalid_argument("Error: Can not open tracked files' file." ));
        }

        std::string ligne;
        while (std::getline(fichier, ligne)) {
            result.push_back(ligne);
        }
        fichier.close();
        return result;
    }

    void status(vector<string>){
        GitRepo repo = repo_find("");
        vector<string> files;
        files = listFiles();
        string sha = get_head(repo,true);
        string s = read_object(repo,sha);
        unordered_map<string,string> k = kvlm_parse(s);
        vector<string> tracked;
        for (auto& it: k){
            if(it.first[0] != '#'){
                tracked.push_back(it.first);
            }
        }
        set<string> files_set;
        for(string& file : files){
            files_set.insert(file);
        }
        set<string> tracked_set;
        for(string& file : tracked){
            tracked_set.insert(file);
        }

        cout << "=== START GAC STATUS ===" << endl << endl;

        for(string& file : files){
            if (tracked_set.find(file) == tracked_set.end()){
                cout << file << " Added" << endl;
            }
            else{
                if(sha1(get_file_content(file)) == sha1(read_object(repo,k[file]))){
                    cout << file << " Unchanged" << endl;
                }
                else{
                    cout << file << " Modified" << endl;
                }
            }
        }

        for(string& file : tracked){
            if (files_set.find(file) == files_set.end()){
                cout << file << " Deleted" << endl;
            }
        }

        cout << endl << "=== END GAC STATUS ===" << endl;
    }

    bool made_changes(GitRepo repo){
        vector<string> files;
        files = listFiles();
        string sha = get_head(repo,true);
        string s = read_object(repo,sha);
        unordered_map<string,string> k = kvlm_parse(s);
        vector<string> tracked;
        for (auto& it: k){
            if(it.first[0] != '#'){
                tracked.push_back(it.first);
            }
        }
        set<string> files_set;
        for(string& file : files){
            files_set.insert(file);
        }
        set<string> tracked_set;
        for(string& file : tracked){
            tracked_set.insert(file);
        }


        for(string& file : files){
            if (tracked_set.find(file) == tracked_set.end()){
                return true;
            }
            else{
                if(sha1(get_file_content(file)) == sha1(read_object(repo,k[file]))){
                }
                else{
                    return true;
                }
            }
        }

        for(string& file : tracked){
            if (files_set.find(file) == files_set.end()){
                return true;
            }
        }

        return false;
    }


    void write_commit(string name, bool temporary, string second_parent){
        GitRepo repo = repo_find("");
        vector<string> files;
        files = listFiles();
        for(string& file : files){
            vector<string> args;
            args.push_back(file);
            track_file(args);
            
        }
        GitCommit c;
        unordered_map<string,string> branches = get_branches(repo);
        string sha_head = get_head(repo,true);

        if(branches[get_active_branch(repo)]!=get_head(repo,false) && sha_head != "none"){
            throw(logic_error("Can't commit in detached Head mode"));
        }
        
        unordered_map<string, string> k;
        k.insert(make_pair("#name",name));
        k.insert(make_pair("#parent",sha_head));
        if(temporary){k["#temporary"] = "true";}
        else{k["#temporary"] = "false";}
        if(second_parent != ""){k["#second_parent"] = second_parent;}
        
        cout << "=== START COMMIT ===" << endl;
        for (std::string& element : files) {
            cout << "Found " << element << endl;
            std::vector<unsigned char> a_hash;
            if(!fs::exists(element)){throw(logic_error("File "+element+" doesn't exist"));}
            ifstream file(repo.get_gitdir() / ".." / element);
            std::stringstream buffer;
            buffer << file.rdbuf(); // Read the entire file into the stringstream buffer
            std::string content = buffer.str();

            cout << "Read file " << element << " content was : "<< endl << content << endl;

            GitBlob a_ajouter(content);
            a_ajouter.deserialize(content); // si on le met pas ça met 0, faut vraiment utiliser full fonctions quand on utilise des réfs ...
            string res_sha =  write_to_git_object(repo,a_ajouter);
            k.insert(std::make_pair(element, res_sha));
        }
        cout<<"Explored all files to add."<<endl;
        cout << "Writting the commit..." << endl;
        c.deserialize(kvlm_serialize(k));
        string sha = write_to_git_object(repo,c);
        cout << "Changing HEAD..." << endl;
        set_head(repo,sha);

        branches[get_active_branch(repo)] = sha;
        ofstream branch_file(repo.get_gitdir() / "branches");
        branch_file << kvlm_serialize(branches);
        branch_file.close();

        cout << "=== END COMMIT ===" << endl;
        
    }


    /*! \brief T'as vraiment besoin d'une desciption mdr ?
    */
    string get_file_content(fs::path path){
        ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        return content;
    }


    /*! \brief get le dico des branches (associe a chaque branche le sha du commit le plus recent de la branche)
        */
    unordered_map<string,string> get_branches(GitRepo repo){
        return kvlm_parse(get_file_content(repo.get_gitdir() / "branches"));
    }

    void checkoutcommit(vector<string> args){
        GitRepo repo = repo_find("");
        string sha = sha_of_position(repo,args[0]);
        string sha_head = get_head(repo,false);
        if(sha_head == sha){
            cout << "You are already at head" << endl;
            if(args[0].substr(0,4)!="HEAD"){set_active_branch(repo,args[0]);}
            return;
        }
        unordered_map<string,string> branches = get_branches(repo);
        if(!(branches[get_active_branch(repo)]!=sha_head && sha_head != "none")){write_commit("temporary_commit",true,"");}
        else{cout<< "No temporary commit was made, because of detached HEAD mode" <<endl << branches[get_active_branch(repo)] << " " << sha_head << endl;}
        vector<string> tracked = get_tracked_files(repo);
        ofstream tracks_file(repo.get_gitdir() / "tracked");
        tracks_file.close();
        cout << "sha : " << sha << endl;
        string s;
        if(sha=="none"){
            s = "";
        }
        else{
            s = read_object(repo,sha);
        }
        cout << "ok" << endl;
          
        unordered_map<string,string> k = kvlm_parse(s);
        set<string> still_exist;
        string name;
        if(sha == "none"){
            name = "none";
        }
        else{
            name = k["#name"];
        }
        cout << "=== START CHECKOUT COMMIT " + name + " ===" <<endl;
        for (auto& it: k) {
            cout << "File " << it.first << endl;
            if(it.first[0]!='#'){
                string s = read_object(repo,it.second);
                ofstream f(it.first);
                f << s;
                f.close();
                vector<string> args;
                args.push_back(it.first);
                track_file(args);
                still_exist.insert(it.first);
            }
        }
        for (string& element : tracked){    //les fichiers qui sont suivis mais pas dans le commit ou on checkout on les supprime
            if (still_exist.find(element) == still_exist.end()){
                cout << "File " << element << " doesn't exist" << endl << "Do you want to remove it ? [y/n] "; 
                //je demande mais on va pt enlever ca en vrai
                string rep;
                cin>>rep;
                if(rep=="y"){fs::remove(element);}
            }
        }
        set_head(repo,sha);
        if(args[0].substr(0,4)!="HEAD"){set_active_branch(repo,args[0]);}
        cout << "=== END CHECKOUT COMMIT " + name + " ===" <<endl;

    }


    /*! \brief get le sha d'une position ecrite comprehensiblement (genre HEAD-2 ou main)
    */
    string sha_of_position(GitRepo repo,string pos){
        string sha;
        if(pos=="HEAD"){
            return get_head(repo,true);
        }
        else if(pos == "HEAD*"){
            string s = read_object(repo,get_head(repo,true)); 
            unordered_map<string,string> k = kvlm_parse(s);
            if (k.find("#second_parent") == k.end()){
                throw(invalid_argument("This commit only has 1 parent"));
            }
            string parent_sha = k["#second_parent"];
            return parent_sha;
        }
        else if(pos.substr(0,5)=="HEAD-"){
            int i = stoi(pos.substr(5,pos.size()-5));
            sha = get_head(repo,false);
            for (int a=0;a<i;a++){
                sha = get_parent(repo,sha);
                if(sha=="none"){throw(invalid_argument("You went too far back"));}
            }
        }
        else{
            unordered_map<string,string> branches = get_branches(repo);
            sha = branches[pos];
            if(sha == ""){throw(invalid_argument(pos + " is not a branch name"));}
        }
        return sha;
    }

    void show_history(vector<string> args){
        GitRepo repo = repo_find("");
        string sha = get_head(repo,true);
        vector<string> names;
        while(sha!="none"){
            names.push_back(open_commit(repo,sha)["#name"]);
            sha = get_parent(repo,sha);
        }
        cout << endl << "=== BEGIN HISTORY ===" << endl;
        for(int i = names.size()-1;i>=0;i--){
            cout << names[i] << endl;
        }
        cout << endl << "=== END HISTORY ===" << endl;
    }
//darcs c ce que le prof a dit de check pour les git merge

    /*! \brief get the sha of head
    */
    string get_head(GitRepo repo, bool ignore_temporary){
        ifstream file(repo.get_gitdir() / "HEAD");
        std::stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        file.close();
        if(ignore_temporary && content != "none"){
            string s = read_object(repo,content);   //j'arrive pas a faire du polymorphisme pour dire que c un commit
            unordered_map<string,string> k = kvlm_parse(s);
            if(k["#temporary"]=="true"){content = k["#parent"];}        //c moins propre pour le code que faire get_parent mais ca va plus vite
        }
        return content;
    }

    unordered_map<string,string> open_commit(GitRepo repo, string sha){
        string s = read_object(repo,sha);   //j'arrive pas a faire du polymorphisme pour dire que c un commit
        unordered_map<string,string> k = kvlm_parse(s);
        return k;
    }


    /*! \brief set the head to the commit of sha sha
    */
    void set_head(GitRepo repo, string sha){
        ofstream headfile(repo.get_gitdir() / "HEAD");
        headfile << sha;
        headfile.close();
    }


    /*! \brief get the active_branch
    */
    string get_active_branch(GitRepo repo){
        ifstream file(repo.get_gitdir() / "active_branch");
        std::stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        file.close();
        return content;
    }


    /*! \brief set the active_branch to branch
    */
    void set_active_branch(GitRepo repo, string branch){
        ofstream headfile(repo.get_gitdir() / "active_branch");
        headfile << branch;
        headfile.close();
    }

    /*! \brief get the parent of commit with sha sha
    */
    string get_parent(GitRepo repo, string sha){
        string s = read_object(repo,sha);   //j'arrive pas a faire du polymorphisme pour dire que c un commit
        unordered_map<string,string> k = kvlm_parse(s);
        string parent_sha = k["#parent"];
        return parent_sha;
    }

    void create_branch(vector<string> args){
        string name = args[0];
        GitRepo repo = repo_find("");
        string sha = get_head(repo,true);
        unordered_map<string,string> branches = get_branches(repo);
        if(branches.find(name) != branches.end()){
            throw(invalid_argument("Branch " + name + " already exists"));
        }
        branches[name] = sha;
        set_active_branch(repo,name);
        ofstream branch_file(repo.get_gitdir() / "branches");
        branch_file << kvlm_serialize(branches);
        branch_file.close();

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

    string read_object(GitRepo repo, string sha, bool write) { // Essayer avec ça. Si ça marhe pas, las passer en void, la faire prendre un GitObject* et le set dedans. On peut faire une variable qui indique le type et très facilement vérifeir le type de la variable de retour avec un Switch.        /*
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

        if(write){
            std::cout << "Content of the file:" << std::endl;
            std::cout << content << std::endl;
        }

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
        
        return content;
        
    }


    bool isHidden(string path) {
        return path.at(0) == '.';
    }

    string convertToNormalFileFormat(fs::path path){
        return path.string().substr(2,path.string().size()-2);
    }

    vector<string> listFiles() {
        vector<string> rep;
        fs::path path = ".";
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            string entrypath = convertToNormalFileFormat(entry.path());
            if (fs::is_regular_file(entrypath) && !isHidden(entrypath)) {
                rep.push_back(entrypath);
            }
        }
        return rep;
    }


}