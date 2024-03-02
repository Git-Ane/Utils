using namespace std;

#include<iostream>
#include<string>
#include <vector>
#include "commands.hpp"

namespace GitAne{
    /*! \brief A Command callable in the terminal
    */
    
            Command::Command(string nom,void (*fonc) (vector<string>),string helpmess, unsigned int nbm, unsigned int nbM){
                name = nom;
                fonction = fonc;
                helpMessage = helpmess;
                nbminargs = nbm;
                nbmaxargs = nbM;
            }


            /*! \brief Get Command name
            */
            string Command::getName(){
                return name;
            }

            /*! \brief Is function compatible with name + number of arguments
            */
            bool Command::compatible(string nom,unsigned int nbarg){
                return (nom==name && nbarg >= nbminargs && nbarg <= nbmaxargs);
            }

            /*! \brief execute Command
            */
            void Command::execute(vector<string> argvect){
                fonction(argvect);
            }

            /*! \brief Print information about command
            */
            void Command::print(){
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
            unsigned int Command::getNbMinArg(){
                return nbminargs;
            }


    using namespace GitAne;

    namespace Console{

        /*! \brief The command list
        */
        vector<Command*> commandesvect;



        /*! \brief Add a Command to Command list
        \param name the name of the command
        \param fonc the function to execute when command is called
        \param helpmess the help message
        \param nbminarg minimum number of arguments
        \param nbmaxarg maximum number of arguments
        */
        void addCommand(string name, void (*fonc) (vector<string>),string helpmess, unsigned int nbminarg, unsigned int nbmaxarg){
             commandesvect.push_back(new Command(name,fonc,helpmess,nbminarg,nbmaxarg));

    }
}
}