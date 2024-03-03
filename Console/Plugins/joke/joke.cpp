using namespace std;

#include "joke.hpp"


using namespace GitAne::Console;



namespace GitAne{

    namespace Joke{
        void jonhatan(vector<string>){
            cout<<"Qu'est ce qui est jaune et qui attend ?"<< endl << "Jonathan !"<<endl;
        }

        void chinese(vector<string>){
            cout << "Quelles sont les pronoms d'un chinois sur Discord ?" << endl << "He/Yaaaa !" << endl;
        }

        void blague_alice(vector<string>){
            cout << "C'est un pingouin qui respire par les fesses." << endl << "Un jour il s'assoit et il meurt." << endl;
        }


        void joke_plugin_loader(){
            initPlugin("joke");
            addCommand("jonathan",&jonhatan,"A joke about Jonathan",0,0);
            addCommand("chinese",&chinese,"A joke about chinese ppl",0,0);
            addCommand("alice",&blague_alice,"blague de merde",0,0); // dsl pour son humour
        }
    }

}


