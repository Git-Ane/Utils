using namespace std;

#include<iostream>
#include<string>
#include <vector>


namespace GitAne{

    /*! \brief A Command callable in the terminal
    */
    class Command
    {
    private:
        string name;
        void (*fonction) (vector<string>);
        string helpMessage;
        unsigned int nbminargs;
        unsigned int nbmaxargs;
    public:
        Command(string nom,void (*fonc) (vector<string>),string helpmess, unsigned int nbm, unsigned int nbM);

        /*! \brief Get Command name
            */
        string getName();

        /*! \brief Is function compatible with name + number of arguments
            */
        bool compatible(string nom,unsigned int nbarg);

        /*! \brief execute Command
            */
        void execute(vector<string> argvect);

        /*! \brief Print information about command
            */
        void print();

        /*! \brief Get minimal number of arguments of Command
            */
        unsigned int getNbMinArg();
        
    };

    namespace Console{

        /*! \brief The command list
        */
        extern vector<Command*> commandesvect;

        /*! \brief Add a Command to Command list
        \param name the name of the command
        \param fonc the function to execute when command is called
        \param helpmess the help message
        \param nbminarg minimum number of arguments
        \param nbmaxarg maximum number of arguments
        */
        void addCommand(string name, void (*fonc) (vector<string>),string helpmess, unsigned int nbminarg, unsigned int nbmaxarg);
    
    }
    
}