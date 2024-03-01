using namespace std;

#include<iostream>
#include<string>
#include <vector>

namespace GitAne{
    /*! \brief A Command callable in the terminal
    */
    class Command{
        public:
            Command(string nom,void (*fonc) (vector<string>),string helpmess, unsigned int nbm, unsigned int nbM){
                name = nom;
                fonction = fonc;
                helpMessage = helpmess;
                nbminargs = nbm;
                nbmaxargs = nbM;
            }


            /*! \brief Get Command name
            */
            string getName(){
                return name;
            }

            /*! \brief Is function compatible with name + number of arguments
            */
            bool compatible(string nom,unsigned int nbarg){
                return (nom==name && nbarg >= nbminargs && nbarg <= nbmaxargs);
            }

            /*! \brief execute Command
            */
            void execute(vector<string> argvect){
                fonction(argvect);
            }

            /*! \brief Print information about command
            */
            void print(){
                cout << name;
                unsigned int i=0;
                for(i=0;i<nbmaxargs;i++){
                    cout << " ";
                    if(i>=nbminargs){cout << "?";}
                    cout << "arg" << i+1;
                }
                cout << " -> " << helpMessage << endl;
            }

            /*! \brief Get minimal number of arguments of Command
            */
            unsigned int getNbMinArg(){
                return nbminargs;
            }

        private:
            string name;
            void (*fonction) (vector<string>);
            string helpMessage;
            unsigned int nbminargs;
            unsigned int nbmaxargs;
    };

    using namespace GitAne;

    namespace Console{

        /*! \brief The command list
        */
        vector<Command> commandesvect;



        /*! \brief Add a Command to Command list
        \param name the name of the command
        \param fonc the fonc to execute when command is called
        \param helpmess the help message
        \param nbminarg minimum number of arguments
        \param nbmaxarg maximum number of arguments
        */
        void addCommand(string name, void (*fonc) (vector<string>),string helpmess, unsigned int nbminarg, unsigned int nbmaxarg){
            commandesvect.push_back(Command(name,fonc,helpmess,nbminarg,nbmaxarg));

    }
}

}

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
                    if(commandesvect[i].compatible(argv[2],commandesvect[i].getNbMinArg())){
                        commandesvect[i].print();
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
                if(commandesvect[i].compatible(argv[1],uargc-2)){
                    commandesvect[i].execute(argvect);
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

