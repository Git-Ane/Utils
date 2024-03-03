using namespace std;

#include<iostream>
#include<string>
#include <vector>
#include "commands.hpp"




namespace GitAne{
    /*! \brief A Command callable in the terminal
    */
    
            Command::Command(string nom, string plugin_name,void (*fonc) (vector<string>),string helpmess, unsigned int nbm, unsigned int nbM){
                name = nom;
                plugin = plugin_name;
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

            string Command::getPlugin(){
                return plugin;
            }

            string Command::getHelpMsg(){
                return helpMessage;
            }

            /*! \brief Is function compatible with name + number of arguments
            */
            bool Command::compatible(string nom,string nomplugin ,unsigned int nbarg){
                return (nom==name && nbarg >= nbminargs && nbarg <= nbmaxargs && nomplugin==plugin);
            }

            /*! \brief execute Command
            */
            void Command::execute(vector<string> argvect){
                fonction(argvect);
            }

            /*! \brief Print information about command
            */
            void Command::print(){
                cout << plugin << " " << name;
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
            unsigned int Command::getNbMaxArg(){
                return nbmaxargs;
            }


    using namespace GitAne;

    namespace Console{

        /*! \brief The command list
        */
        vector<Command*> commandesvect;
        string active_plugin_name = "";



        /*! \brief Add a Command to Command list
        \param name the name of the command
        \param fonc the function to execute when command is called
        \param helpmess the help message
        \param nbminarg minimum number of arguments
        \param nbmaxarg maximum number of arguments
        \param nbmaxarg maximum number of arguments
        */
        void addCommand(string name, void (*fonc) (vector<string>),string helpmess, unsigned int nbminarg, unsigned int nbmaxarg){
             commandesvect.push_back(new Command(name,active_plugin_name,fonc,helpmess,nbminarg,nbmaxarg));
        }

        /*! \brief Call first when loading a plugin
        \param plugin_name the name of the plugin
        */
        void initPlugin(string plugin_name){
            active_plugin_name = plugin_name;
        }
    }
}