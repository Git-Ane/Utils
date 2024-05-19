using namespace std;

#include "joke.hpp"


using namespace GitAne::Console;



namespace GitAne{

    namespace Joke{
        void jonhatan(vector<string>){
            cout<<"Qu'est ce qui est jaune et qui attend ?"<< endl << "Jonathan !"<<endl;
        }


        void blague_vincent(vector<string>) {
            cout << "C'est un tétard qui croyait qui croyait qu'il était tôt mais enfaite il était tard" << endl;
        }

        void blague_glass(vector<string>){
            cout << "An optimist says 'The glass is half full'" << endl;
            cout << "A pessimist says 'The glass is half empty'" << endl;
            cout << "A GitAne developper says 'The glass is twice as large as necessary'" << endl;
        }

        void blague_best(vector<string>){
            cout << "I have the best joke ever about programming" << endl;
            cout << "But it only works on my computer!" << endl;
        }


        void joke_plugin_loader(){
            initPlugin("joke");
            addCommand("jonathan",&jonhatan,"A joke about Jonathan",0,0);
            addCommand("vincent",&blague_vincent,"hehe",0,0);
            addCommand("glass",&blague_glass,"a joke I totally invented",0,0);
            addCommand("best",&blague_best,"the best joke ever about programming",0,0);
        }
    }

}


