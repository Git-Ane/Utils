#include "gitting.hpp"

using namespace GitAne;
using namespace std;
namespace fs = std::filesystem;


string convert_filename(string filename){
    std::string s = filename;
    std::replace( s.begin(), s.end(), '/', '!');
    return s;
}


void copy_to_server(string proj_name){
    GitAne::NetClient cTest("localhost:8087","test@test.com","testest", false);
    GitRepo repo = repo_find("");
    for (const auto& entry : fs::recursive_directory_iterator(repo.get_gitdir())){
        if(fs::is_regular_file(entry)){
            string s = convert_filename(entry.path().string());
            cout << s << endl;
            cTest.sendFile(proj_name,s.substr(5,s.size()-5),get_file_content(entry));
            cout << s.substr(5,s.size()-5) << endl;
        }
    }
}


void push(string proj_name){
    GitAne::NetClient cTest("localhost:8087","test@test.com","testest", false);
    GitRepo repo = repo_find("");
    string active_branch = get_active_branch(repo);
    string local_branch = get_branches(repo)[active_branch];
    string branches = cTest.receiveFile(proj_name,convert_filename("branches"));
    auto k = kvlm_parse(branches);
    string remote_branch = k[active_branch];
    cout << local_branch << " " << remote_branch << endl;
    vector<string> to_add;
    while (local_branch != remote_branch)
    {
        to_add.push_back(local_branch);
        local_branch = get_parent(repo,local_branch);
        if(local_branch == "none"){throw(logic_error("Commits were made while you were away!"));}
    }

    for (int i=to_add.size()-1;i>=0;i--){
        string sha = to_add[i];
        string entrypath = "objects/"+sha.substr(0,2)+"/"+sha.substr(2,sha.size()-2);
        cout << sha << endl;
        cTest.sendFile(proj_name,convert_filename(entrypath),read_object(repo,sha));
    }

    k[active_branch] = local_branch;
    cTest.sendFile(proj_name,"branches",kvlm_serialize(k));
    
}


