using namespace std;

#include<iostream>
#include<string>
#include <vector>
#include "commands.hpp"



using namespace GitAne;

using namespace Console;




void sayHello(vector<string> _){
    cout << "Hi from the Gitane Team !" << endl;
}

void concatenate(vector<string> words){
    if(words.size()==2){cout << words[0] << words[1] << endl;}
    else{cout << words[0]<<endl;}
}


int main(int argc, char* argv[]) {

    unsigned int uargc = argc;


    addCommand("hello",&sayHello,"says hello :)",0,0); 
    addCommand("concatenate",&concatenate,"concatenates arg1 and arg2 (returns arg1 if only 1 argument)",1,2); 

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
            else{
                for(i=0;i<commandesvect.size();i++){
                    if((*commandesvect[i]).compatible(argv[2],(*commandesvect[i]).getNbMinArg())){
                        (*commandesvect[i]).print();
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << argv[2] <<" is not a valid gitane command" << endl;
                        cout << "Type gitane (or gac for short) to see a list of commands" << endl;
                }
            }
        }
        else{

            vector<string> argvect;

            for(i=2;i<uargc;i++){
                argvect.push_back(string(argv[i]));
            }

            for(i=0;i<commandesvect.size();i++){
                if((*commandesvect[i]).compatible(argv[1],uargc-2)){
                    (*commandesvect[i]).execute(argvect);
                    break;
                }
            }
            if(i==commandesvect.size()){
                    cout << "This is not a valid gitane command" << endl;
                    cout << "Type gitane (or gac for short) to see a list of commands" << endl;
            }
        }
    }

    return 0;
}

