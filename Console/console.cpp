using namespace std;

#include<iostream>
#include<string>
#include <vector>
#include "../Git/classes.hpp"

#include "../Plugins/plugin_loader.hpp"

#include <ostream>

#include <filesystem>
namespace fs = std::filesystem;
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_YELLOW   = 33,
        FG_PURPLE   = 35,
        FG_BLUE     = 36,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

Color::Modifier red(Color::FG_RED);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier purple(Color::FG_PURPLE);
Color::Modifier yellow(Color::FG_YELLOW);
Color::Modifier def(Color::FG_DEFAULT);

Color::Modifier errorcolor = red;
Color::Modifier plugin_color = purple;
Color::Modifier command_color = blue;
Color::Modifier arg_color = yellow;

using namespace GitAne;

using namespace Console;

namespace GitAne{
    
    namespace Console{

        void sayHello(vector<string> _){
            cout << "Hi from the Gitane Team !" << endl;
        }

        void concatenate(vector<string> words){
            if(words.size()==2){cout << words[0] << words[1] << endl;}
            else{cout << words[0]<<endl;}
        }

        void std_helpmsg(vector<string> words){
            cout<< "The default plugin" <<endl;
        }

        void print_color(Command c){
            cout << plugin_color << c.getPlugin() << " " << command_color << c.getName() << arg_color;
                unsigned int i=0;
                for(i=0;i<c.getNbMaxArg();i++){
                    cout << " ";
                    if(i>=c.getNbMinArg()){cout << "?";}
                    cout << "arg" << i+1;
                }
                cout << def << " -> " << c.getHelpMsg() << endl;
        }

        void repo_init(vector<string> _){
            GitRepo repo = GitAne::create_repo(fs::current_path());
            write_commit("initial_commit", false, "", true);
        }

        void write_commit_fun(vector<string> args){
            write_commit(args[0],false, "", false);
        }

        void list_branches_fun(vector<string>){
            GitRepo repo = repo_find("");
            unordered_map<string,string> k = get_branches(repo);
            string active_branch = get_active_branch(repo);
            for (auto& it: k){
                if(it.first == active_branch){
                    cout << it.first << " <-- active branch" << endl;
                }
                else{
                    cout << it.first << endl;
                }
            }
        }

        void move_branch_fun(vector<string> args){
            move_branch();
        }

        void delete_branch_fun(vector<string> args){
            delete_branch(args[0]);
        }

        void clean_fun(vector<string> args){
            clean();
        }

        

    }
}
std::string getHomeDirectory() {
    const char *homeDir = getenv("HOME");
    if (homeDir != nullptr) {
        return std::string(homeDir);
    } else {
        // HOME environment variable not set, you may need to handle this case
        return ""; // Alternatively, you could throw an exception or return a default directory
    }
}



int main(int argc, char* argv[]) {

    unsigned int uargc = argc;
    try{
        if(!fs::exists(getHomeDirectory() + "/.config/gac")){
            fs::create_directories(getHomeDirectory() +  "/.config/gac");
        }
        
    }
    catch(...){
        std::cout << "[!] Can not create folder ~/.config/gac, please check that .config exists." << std::endl;
    }
    initPlugin("std");
    addCommand("help",&std_helpmsg,"Help message about std plugin",0,0);
    addCommand("hello",&sayHello,"says hello :)",0,0); 
    addCommand("init",&repo_init,"inits the current repository to be a Gitane repository",0,0);
    addCommand("commit",&write_commit_fun,"commit the tracked files and name the commit arg1",1,1);
    addCommand("checkout",&checkoutcommit,"check out the commit arg1",1,1);
    addCommand("branch",&create_branch,"create branch arg1 at HEAD",1,1);
    addCommand("history",&show_history,"show history",0,0);
    addCommand("status",&status,"show history",0,0);
    addCommand("list_branches",&list_branches_fun,"list the branches of the GitAne repo",0,0);
    addCommand("move_branch",&move_branch_fun,"move the active branch to the position of HEAD",0,0);
    addCommand("delete_branch",&delete_branch_fun,"delete branch arg1",1,1);
    addCommand("clean",&clean_fun,"clean useless files",0,0);
    plugin_loader();


    unsigned int i; 

    if(argc==1){
        cout << "Welcome to Gitane !" << endl;
        cout << "Here is a list of commands you can use :" << endl;
        for(i=0;i<commandesvect.size();i++){
            print_color(*commandesvect[i]);
        }
    }
    else{
        if(string(argv[1]) == "help"){
            if(argc==2){
                cout << "Type gitane help cmd to learn more about the command cmd" << endl;
            }
            else if(argc==4){
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible(argv[3],argv[2],(*commandesvect[i]).getNbMinArg())){
                        print_color(*commandesvect[i]);
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << plugin_color << argv[2] << command_color << " " << argv[3] << errorcolor <<" is not a valid gitane command" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
            else if(argc==3){
                vector<string> argvect;
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible("help",argv[2],(*commandesvect[i]).getNbMinArg())){
                        (*commandesvect[i]).execute(argvect);
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << errorcolor << "Plugin " << plugin_color << argv[2] << errorcolor << " is not imported or doesn't have an help command" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
            else{
                cout << errorcolor << "Too many arguments for help" << def <<endl;
            }
        }
        else{

            vector<string> argvect;

            for(i=3;i<uargc;i++){
                argvect.push_back(string(argv[i]));
            }

            if(argc==2){    //appeler gac plugin = appeler gac plugin help
                bool foundsomeone = false;
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible((*commandesvect[i]).getName(),argv[1],(*commandesvect[i]).getNbMinArg())){
                        print_color(*commandesvect[i]);
                        foundsomeone = true;
                    }
                    
                }
                if(!foundsomeone){
                    cout << errorcolor << "Plugin "<< plugin_color << argv[1] << errorcolor << " is not imported" << def << endl;
                    cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
            else{ 

                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible(argv[2],argv[1],uargc-3)){
                        (*commandesvect[i]).execute(argvect);
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << plugin_color << argv[1] << " " << command_color << argv[2] << errorcolor << " is not a valid gitane command or is called with the wrong number of arguments" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
        }
    }

    return 0;
}

