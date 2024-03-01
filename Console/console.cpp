using namespace std;

#include<iostream>
#include<string>
#include <vector>

namespace GitAne{
    class Command{
        public:
            Command(string nom,void (*fonc) (void),string helpmess){
                name = nom;
                fonction = fonc;
                helpMessage = helpmess;
            }

            string getName(){
                return name;
            }

            bool compatible(string nom){
                return (nom==name);
            }

            void execute(){
                fonction();
            }

            void print(){
                cout << name << " -> " << helpMessage << endl;
            }

        private:
            string name;
            void (*fonction) (void);
            string helpMessage;
    };

}

using namespace GitAne;

vector<Command> commandesvect;

void addCommand(string name, void (*fonc) (void),string helpmess){
    commandesvect.push_back(Command(name,fonc,helpmess));
}


void sayHello(){
    cout << "Hi from the Gitane Team !\n";
}


int main(int argc, char* argv[]) {

    addCommand("hello",&sayHello,"says hello :)"); 

    unsigned int i; 

    if(argc==1){
        cout << "Welcome to Gitane !" << endl;
        cout << "Here is a list of commands you can use :" << endl;
        for(i=0;i<commandesvect.size();i++){
            commandesvect[i].print();
        }
    }
    else{
        if(string(argv[1]) == "help"){
            if(argc==2){
                cout << "Type gitane help cmd to learn more about the command cmd" << endl;
            }
            else{
                for(i=0;i<commandesvect.size();i++){
                    if(commandesvect[i].compatible(argv[2])){
                        commandesvect[i].print();
                        break;
                    }
                }
                if(i==commandesvect.size()){
                        cout << argv[2] <<" is not a valid gitane command" << endl;
                        cout << "Type gitane to see a list of commands" << endl;
                }
            }
        }
        else{

            for(i=0;i<commandesvect.size();i++){
                if(commandesvect[i].compatible(argv[1])){
                    commandesvect[i].execute();
                    break;
                }
            }
            if(i==commandesvect.size()){
                    cout << "This is not a valid gitane command" << endl;
                    cout << "Type gitane to see a list of commands" << endl;
            }
        }
    }

    return 0;
}

