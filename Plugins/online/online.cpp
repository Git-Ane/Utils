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



        void add(vector<string> args){
            string url = args[0];
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

        void remove_server(vector<string> args) {
            string server_name = args[0];
            ifstream infile("server_list");
            ofstream outfile("temp_file");
            string line;
            if (infile.is_open() && outfile.is_open()) {
                while (getline(infile, line)) {
                    string current_server_name = line.substr(0, line.find('\0'));
                    if (current_server_name != server_name) {
                        outfile << line << '\n';
                    }
                }
                infile.close();
                outfile.close();
                remove("server_list");
                rename("temp_file", "server_list");
            } else {
                cout << "Error: can not open server_list file and temp_file." << endl;
            }
        }

        void connect(vector<string> args) {
            string server_name = args[0];
            ofstream outfile("act_serv");
            if (outfile.is_open()) {
                outfile << server_name;
                outfile.close();
                cout << "[*] Main server moved to " << server_name << endl;
            } else {
                cout << "Error: can not edit act_server file." << endl;
            }
        }



        void online_plugin_loader(){
            initPlugin("online");

        }
    }

}


