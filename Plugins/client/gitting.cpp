#include "gitting.hpp"

using namespace GitAne;
using namespace std;
namespace fs = std::filesystem;


string convert_filename(string filename){
    std::string s = filename;
    std::replace( s.begin(), s.end(), '/', '!');
    return s;
}

std::string getHomeDirectoryGitting() {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr) {
                return std::string(homeDir);
            } else {
                // HOME environment variable not set, you may need to handle this case
                return ""; // Alternatively, you could throw an exception or return a default directory
            }
        }

std::pair<std::string, std::string> url_and_token() {
            std::ifstream infile2(getHomeDirectoryGitting() + "/.config/gac/act_serv");
            std::string server_name;
            if(std::getline(infile2, server_name))
            {

            
            std::ifstream infile(getHomeDirectoryGitting() + "/.config/gac/server_list");
            std::string line;
            while (std::getline(infile, line)) {
                std::istringstream iss(line);
                std::string current_server_name;
                if (std::getline(iss, current_server_name, '\0')) {
                    if (current_server_name == server_name) {
                        std::string url, token;
                        if (std::getline(iss, url, '\0') && std::getline(iss, token, '\0')) {
                            return std::make_pair(url, token);
                        }
                    }
                }
            }
            // Return empty strings if server name not found
            std::cout << "[!] act_server content does not correspond to any server in the list." << std::endl; 
            return std::make_pair("", "");
            }
            else {
                std::cout << "[!] act_server does not exists. Please do gac client connect server_name" << std::endl;
                return std::make_pair("", "");
            }
        }

void copy_to_server(string proj_name){
    std::pair<std::string, std::string> p = url_and_token();
    GitAne::NetClient cTest(p.first,"auto-registered-name",p.second, false,true);
    GitRepo repo = repo_find("");
    for (const auto& entry : fs::recursive_directory_iterator(repo.get_gitdir())){
        if(fs::is_regular_file(entry)){
            string s = convert_filename(entry.path().string());
            cTest.sendFile(proj_name,s.substr(5,s.size()-5),get_file_content(entry));
        }
    }
    cout << "Current git was copied to server as project " << proj_name << endl;
}


void pull(string proj_name){
    GitRepo repo = repo_find("");
    if(made_changes(repo)){throw(logic_error("Commit your changes before you pull!"));}
    if((get_branches(repo)[get_active_branch(repo)]!=get_head(repo,true))){throw(logic_error("Can't pull in detached HEAD mode"));}

    std::pair<std::string, std::string> p = url_and_token();
    GitAne::NetClient cTest(p.first,"auto-registered-name",p.second, false,true);
    string active_branch = get_active_branch(repo);
    string local_branch = get_branches(repo)[active_branch];
    string branches = cTest.receiveFile(proj_name,convert_filename("branches"));
    auto k = kvlm_parse(branches);
    string remote_branch = k[active_branch];
    string new_branch_sha = k[active_branch];
    if(remote_branch==""){throw(logic_error("This branch doesn't exist on the repo!"));}

    vector<string> to_add;
    while (local_branch != remote_branch)
    {
        to_add.push_back(remote_branch);
        auto kc = kvlm_parse(cTest.receiveFile(proj_name,convert_filename("objects/"+remote_branch.substr(0,2)+"/"+remote_branch.substr(2,remote_branch.size()-2))));
        remote_branch = kc["#parent"];
        if(remote_branch == "none"){throw(logic_error("Commits were made while you were away!"));}
    }

    for (int i=to_add.size()-1;i>=0;i--){
        string sha = to_add[i];
        string sc = cTest.receiveFile(proj_name,convert_filename("objects/"+sha.substr(0,2)+"/"+sha.substr(2,sha.size()-2)));
        auto kc = kvlm_parse(sc);
        for (auto it : kc){
            if(it.first[0]!= '#'){
                string entrypath = "objects/"+it.second.substr(0,2)+"/"+it.second.substr(2,it.second.size()-2);
                cout << "file " << it.first << endl;
                string content = cTest.receiveFile(proj_name,convert_filename(entrypath));
                GitBlob a_ajouter(content);
                a_ajouter.deserialize(content);
                write_to_git_object(repo,a_ajouter,it.second);
            }
        }
        GitCommit c;
        c.deserialize(sc);
        write_to_git_object(repo,c,sha);
        cout << "commit " << sha << endl;
    }

    k = get_branches(repo);
    k[active_branch] = new_branch_sha;
    ofstream branch_file(repo.get_gitdir() / "branches");
    branch_file << kvlm_serialize(k);
    branch_file.close();

    vector<string> args;
    args.push_back(active_branch);
    checkoutcommit(args);

    cout << "Pulled branch " << active_branch << endl;

}


void push(string proj_name){
    GitRepo repo = repo_find("");
    if(made_changes(repo)){throw(logic_error("Commit your changes before you push!"));}
    if((get_branches(repo)[get_active_branch(repo)]!=get_head(repo,true))){throw(logic_error("Can't push in detached HEAD mode"));}

    std::pair<std::string, std::string> p = url_and_token();
    GitAne::NetClient cTest(p.first,"auto-registered-name",p.second, false,true);
    string active_branch = get_active_branch(repo);
    string local_branch = get_branches(repo)[active_branch];
    string new_remote_branch = get_branches(repo)[active_branch];
    string branches = cTest.receiveFile(proj_name,convert_filename("branches"));
    auto k = kvlm_parse(branches);
    string remote_branch = k[active_branch];
    vector<string> to_add;
    if(remote_branch != ""){
        while (local_branch != remote_branch)
            {
                to_add.push_back(local_branch);
                local_branch = get_parent(repo,local_branch);
                if(local_branch == "none"){throw(logic_error("Commits were made while you were away!"));}
            }
    }
    else{
        while (!cTest.file_exists(proj_name,convert_filename("objects/"+local_branch.substr(0,2)+"/"+local_branch.substr(2,local_branch.size()-2))))        //tant que le fichier n'existe pas sur le serveur
            {
                to_add.push_back(local_branch);
                local_branch = get_parent(repo,local_branch);
                if(local_branch == "none"){throw(logic_error("Commits were made while you were away!"));}
            }
    }
    
    

    for (int i=to_add.size()-1;i>=0;i--){
        string sha = to_add[i];
        auto k = open_commit(repo,sha);
        for (auto it : k){
            if(it.first[0]!= '#'){
                string entrypath = "objects/"+it.second.substr(0,2)+"/"+it.second.substr(2,it.second.size()-2);
                cTest.sendFile(proj_name,convert_filename(entrypath),read_object(repo,it.second));
            }
        }
        string entrypath = "objects/"+sha.substr(0,2)+"/"+sha.substr(2,sha.size()-2);
        cTest.sendFile(proj_name,convert_filename(entrypath),read_object(repo,sha));
    }

    k[active_branch] = new_remote_branch;
    cTest.sendFile(proj_name,"branches",kvlm_serialize(k));
    cout << "Pushed branch " << active_branch << endl;
    
}


