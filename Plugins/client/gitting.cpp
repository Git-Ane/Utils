#include "gitting.hpp"

using namespace GitAne;
namespace fs = std::filesystem;


void copy_to_server(string proj_name){
    GitAne::NetClient cTest("localhost:8087","test@test.com","testest");
    GitRepo repo = repo_find("");
    for (const auto& entry : fs::recursive_directory_iterator(repo.get_gitdir())){
        if(fs::is_regular_file(entry)){
            string s = entry.path().string();
            cTest.sendFile(proj_name,s.substr(5,s.size()-5),get_file_content(entry));
            cout << s.substr(5,s.size()-5) << endl;
        }
    }
}