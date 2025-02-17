#include "merge.hpp"

using namespace GitAne::Console;
using namespace GitAne;
namespace fs = std::filesystem;
using namespace std;
namespace GitAne{
    namespace Merge{
    void check_merge_conflicts(const string& target_branch) {
            GitRepo r = repo_find("");
            if(made_changes(r)){
                throw(logic_error("Commit your changes before you merge"));
            }
            string current_branch_hash = get_head(r,true);
            string target_branch_hash = get_hash_of_branch(target_branch, true);
            set<string> current_history;
            string h = current_branch_hash;
            while (h != "none"){
                current_history.insert(h);
                h = get_parent(r,h);
            }
            h = target_branch_hash;
            string lca_hash;
            while (current_history.find(h) == current_history.end()){
                h = get_parent(r,h);
            }
            lca_hash = h;
            cout << "LCA : " << lca_hash << endl;
            if(!fs::exists(r.get_gitdir() / "merge")){
                create_dir(r.get_gitdir() / "merge");
            }
            if(target_branch_hash == ""){
                cerr << "La branche indiquée n'existe pas." << endl;
                return;
            }
            if (current_branch_hash == target_branch_hash) {
                cout << "Branches are the same, no fusion is necessary" << endl;
                return;
            }

            if (fs::exists(r.get_gitdir() / "merge"/ current_branch_hash)) {
                cerr << "The active branch is already in a merge. Use 'gac merge abort' to start again" << endl;
                return;

            }
            else {
                ofstream f = create_file(r.get_gitdir() / "merge"/ current_branch_hash);
                if (!f) {
                    cerr << "The locking merge file could not be created" << endl;
                    return;
                }

                // Écrire le hash de la branche cible dans le fichier de verrouillage
                f << target_branch_hash;
                f.close();
            }
            

            vector<string> conflicting_files;

            unordered_map<string, string> current_files = open_commit(r,current_branch_hash);
            unordered_map<string, string> target_files = open_commit(r,target_branch_hash);
            unordered_map<string, string> lca_files = open_commit(r,lca_hash);

            for (const auto& current_file : current_files) {
                if (target_files.find(current_file.first) != target_files.end() && current_file.first[0] != '#') {
                    // Le fichier est présent dans les deux branches, vérifier s'il y a un conflit
                    if (!try_to_merge(current_file.second, target_files[current_file.first], lca_files[current_file.first], current_file.first)) {
                        /*
                            Ajouter la v.2
                        */

                        string hash2 = target_files[current_file.first];
                        if(!fs::exists(r.get_gitdir() / "objects" / hash2.substr(0, 2) / hash2.substr(2))){
                            cerr << "The hash of the file " << current_file.first << " is not valid for the target branch" << endl;
                            return;
                        }
                        ifstream file_v_cible(r.get_gitdir() / "objects" / hash2.substr(0, 2) / hash2.substr(2));
                        char c;
                        while(file_v_cible.get(c)){
                            if(c== '\0') break;
                        }
                        string res;
                        while (file_v_cible.get(c)) {
                            res += c;
                        }
                        file_v_cible.close();
                        ofstream output_file(current_file.first, ios::app);
                        // Vérifier si le fichier est ouvert
                        if (output_file.is_open()) {
                            // Déplacer le curseur à la fin du fichier
                            output_file.seekp(0, ios::end);

                            // Écrire la version de la branche cible suivie de la chaîne res
                            output_file << "[" << "VERSION DE LA BRANCHE CIBLE" << "]" << endl;
                            output_file << res;
                            // Fermer le fichier
                            output_file.close();
                        } else {
                            cerr << "Error when opening file" << endl;
                        }

                        /*
                            Modifier en .conflit
                        */
                        string conflicted_filename = current_file.first + ".conflit";
                        if (rename(current_file.first.c_str(), conflicted_filename.c_str()) != 0) {
                            cerr << "Erreur lors du renommage du fichier en conflit : " << current_file.first << endl;
                        } else {
                            cout << "The conflicting file " << current_file.first << " was renamed in " << conflicted_filename << endl;
                        }
                        conflicting_files.push_back(current_file.first);
                    }
                    else {
                    }
                }
            }

            for (const auto& target_file : target_files) {
                if (current_files.find(target_file.first) == current_files.end() && target_file.first[0] != '#'){
                    string content = read_object(r,target_file.second);
                    ofstream f = create_file(target_file.first);
                    f << content;
                    f.close();
                }
            }

            if (!conflicting_files.empty()) {
                cout << "Conflicts were found in the following files :" << endl;
                for (const auto& file : conflicting_files) {
                    cout << file << endl;
                }
            } else {
                cout << "No conflict was detected" << endl;
                cout << "Use 'gac merge validate' to finish the merge" << endl;
            }
            return;
        }

    bool try_to_merge(string sha_a, string sha_b, string sha_lca, fs::path file_name){
        if(sha_a == sha_b){return true;}
        if(sha_b == sha_lca){return true;}
        if(sha_a == sha_lca){
            GitRepo r = repo_find("");
            string content = read_object(r,sha_b, false);
            ofstream f(file_name);
            f << content;
            f.close();
            return true;
        }
        return false;
    }

    void remove_merge(void){
            GitRepo r = repo_find("");
            if(filesystem::remove_all(r.get_gitdir() / "merge") == 0){
                cerr << "Erreur lors de la suppression du merge. Peut-être que le dossier n'existait déjà plus ?" << endl;
                return;
            }
            cout << "MERGE successfuly aborted." << endl;
            return;
        }

    void validate_and_merge(const string& target_branch) {
        GitRepo r = repo_find("");
        string current_branch = get_active_branch(r);
        string current_branch_hash = get_hash_of_branch(current_branch, true);
        string target_branch_hash = get_hash_of_branch(target_branch, true);
        // Vérifier si la branche cible est encore à la même version
        string target_branch_hash_locked;
        ifstream lock_file(r.get_gitdir() / "merge" / current_branch_hash);
        if (lock_file) {
            getline(lock_file, target_branch_hash_locked);
        }


        // Vérifier si tous les fichiers en conflit ont été résolus
        vector<string> conflicting_files;
        for (const auto& entry : fs::directory_iterator(".")) {
            if (fs::is_regular_file(entry.path())) {
                if (entry.path().extension() == ".conflit") {
                    conflicting_files.push_back(entry.path().filename());
                }
            }
        }

        if (!conflicting_files.empty()) {
            cout << "Conflicting files exist, resolve them before merging" << endl;
            for (const auto& file : conflicting_files) {
                cout << file << endl;
            }
            return;
        }

        // Supprimer le fichier de verrouillage du merge
        fs::remove(r.get_gitdir() / "merge" / current_branch_hash);
        
        write_commit("merge_commit",false, target_branch_hash, false);

        cout << "The merge of branches was done successfully!" << endl;
        return;
    }

    void check_wrapper(vector<string> v){
        check_merge_conflicts(v[0]);
        return;
    }

    void validate_wrapper(vector<string> v){
        validate_and_merge(v[0]);
        return;
    }
    void abort_wrapper(vector<string> v){
        remove_merge();
        return;
    }

    void merge_plugin_loader(){
        initPlugin("merge");
        addCommand("check",&check_wrapper,"check arg1 will init a merge process from your branch to arg1 and mark every merge conflicts.",1,1);
        addCommand("validate",&validate_wrapper,"validate arg1 will validate the merge with the branch arg1 IFF arg1's hash did not changed since the beginning of the merge process.",1,1);
        addCommand("abort",&abort_wrapper,"abort destroy all merges being done on this branch.",0,0);
    }
    }
}