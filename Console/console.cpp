using namespace std;

#include<iostream>
#include<string>
#include <vector>

#include "Plugins/plugin_loader.hpp"

#include <ostream>
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
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
    }
}


int main(int argc, char* argv[]) {

    unsigned int uargc = argc;

    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);

    initPlugin("std");
    addCommand("help",&std_helpmsg,"Help message about std plugin",0,0);
    addCommand("hello",&sayHello,"says hello :)",0,0); 
    addCommand("concatenate",&concatenate,"concatenates arg1 and arg2 (returns arg1 if only 1 argument)",1,2); 

    plugin_loader();

    unsigned int i; 

    if(argc==1){
        cout << "Welcome to Gitane !" << endl;
        cout << "Here is a list of commands you can use :" << endl;
        for(i=0;i<commandesvect.size();i++){
            (*commandesvect[i]).print();
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
                        (*commandesvect[i]).print();
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << red << argv[2] << " " << argv[3] <<" is not a valid gitane command" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
            else if(argc==3){
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible("help",argv[2],(*commandesvect[i]).getNbMinArg())){
                        (*commandesvect[i]).print();
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << red << "Plugin "<< argv[2] << " is not imported or doesn't have an help command" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
            else{
                cout << red << "Too many arguments for help" << def <<endl;
            }
        }
        else{

            vector<string> argvect;

            for(i=3;i<uargc;i++){
                argvect.push_back(string(argv[i]));
            }

            if(argc==2){    //appeler gac plugin = appeler gac plugin help
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible("help",argv[1],uargc-3)){
                        (*commandesvect[i]).execute(argvect);
                        break;
                    }
                    
                }
                if(i==commandesvect.size()){
                    cout << red << "Plugin "<< argv[1] << " is not imported or doesn't have an help command" << def << endl;
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
                        cout << red << "This is not a valid gitane command" << def << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
        }
    }

    return 0;
}

