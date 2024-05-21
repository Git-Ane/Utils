#include "print.hpp"



namespace GitAne{
    namespace Print{

        void print_commit(unordered_map<string,string> k){
            cout << "Commit " << k["#name"] << " :" << endl << endl;
            cout << "Files :" << endl;
            for (auto it : k){
                if(it.first[0]!='#'){
                    cout << it.first << endl;
                }
            }
            cout << endl;
            cout << "Metadata :" << endl;
            for (auto it : k){
                if(it.first[0] == '#' && it.first != "#parent" && it.first != "#name" && it.first != "#second_parent" && it.first != "#temporary"){
                    cout << it.first.substr(1,it.first.size()-1) << " : " << it.second << endl;
                }
            }
            if(k["#second_parent"]==""){cout<< "has second parent : false" << endl;}
            else{cout<< "has second parent : true" << endl;}
            cout << endl;
        }

        void print_commit_fun(vector<string> args){
            GitRepo repo = repo_find("");
            string sha = sha_of_position(repo, args[0]);
            print_commit(open_commit(repo, sha));
        }



        void print_loader(){
            GitAne::Console::initPlugin("print");
            GitAne::Console::addCommand("commit",print_commit_fun,"prints the commit arg1",1,1);
        }
    }
}

