#include "online.hpp"
#include <unordered_map>

using namespace GitAne::Console;
using namespace GitAne;

namespace GitAne{

    namespace Online{
        
        #include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;



        void add(string url){
            cout << "Please answer these questions to  add the server to your server list:\n"<< "- What name you want to give to this server?> ";
            string serv_name;
            getline(cin, serv_name);
            char reponse;
            do {
                cout << "\n- Do you have an account on the server? (Y/N)> ";
                cin >> reponse;
                // Convertir la lettre en majuscule pour rendre le code insensible à la casse
                reponse = toupper(reponse);
            } while (reponse != 'Y' && reponse != 'N');
            bool reg = (reponse == 'N');
            cout <<  "\nWhat is your e-mail?> ";
            string mail;
            getline(cin, mail);
            cout <<  "\nWhat is your password?> ";
            string pwd;
            getline(cin, pwd);
            NetClient net_cl(serv_name,mail,pwd,reg);
            if(net_cl.token != "UNSET"){
            ofstream outfile("server_list", ios::app);
                if (outfile.is_open()) {
                    outfile << serv_name << '\0' << url << '\0' << net_cl.token << '\n';
                    outfile.close();
                    cout << "`\nServer added." << endl;
                } else {
                    cout << "\nError: Impossible to open server_list file" << endl;
                }
            }
            
        }

        void online_plugin_loader(){
            initPlugin("online");

        }
    }

}


