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