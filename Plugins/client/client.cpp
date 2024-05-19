#include "client.hpp"
#include <type_traits>

using namespace GitAne::Console;
using namespace std;

namespace GitAne{
    namespace Client{
        std::string getHomeDirectory() {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr) {
                return std::string(homeDir);
            } else {
                // HOME environment variable not set, you may need to handle this case
                return ""; // Alternatively, you could throw an exception or return a default directory
            }
        }

        void copy_to_server_fun(vector<string> args){
            copy_to_server(args[0]);
        }

        void push_fun(vector<string> args){
            push(args[0]);
        }

        void pull_fun(vector<string> args){
            pull(args[0]);
        }

        std::pair<std::string, std::string> url_and_token() {
            std::ifstream infile2(getHomeDirectory() + "/.config/gac/act_serv");
            std::string server_name;
            if(std::getline(infile2, server_name))
            {

            
            std::ifstream infile(getHomeDirectory() + "/.config/gac/server_list");
            std::string line;
            while (std::getline(infile, line)) {
                std::istringstream iss(line);
                std::string current_server_name;
                if (std::getline(iss, current_server_name, '\0')) {
                    if (current_server_name == server_name) {
                        std::string url, token;
                        if (std::getline(iss, url, '\0') && std::getline(iss, token, '\0')) {
                            return std::make_pair(url, token);
                        }
                    }
                }
            }
            // Return empty strings if server name not found
            std::cout << "[!] act_server content does not correspond to any server in the list." << std::endl; 
            return std::make_pair("", "");
            }
            else {
                std::cout << "[!] act_server does not exists. Please do gac client connect server_name" << std::endl;
                return std::make_pair("", "");
            }
        }

        void add(vector<string> args){
            string url = args[0];
            cout << "Please answer these questions to  add the server to your server list:\n"<< "- What name you want to give to this server?> ";
            string serv_name;
            cin >> serv_name;
            char reponse;
            do {
                cout << "\n- Do you have an account on the server? (Y/N)> ";
                cin >> reponse;
                // Convertir la lettre en majuscule pour rendre le code insensible à la casse
                reponse = toupper(reponse);
            } while (reponse != 'Y' && reponse != 'N');
            bool reg = (reponse == 'N');
            cout <<  "\nWhat is your e-mail?>";
            string mail;
            cin >> mail;
            cout <<  "\nWhat is your password?> ";
            string pwd;
            cin >> pwd;
            try{
                NetClient net_cl(url,mail,pwd,reg,false);
                if(net_cl.token != "UNSET"){
                ofstream outfile(getHomeDirectory() + "/.config/gac/server_list", ios::app);
                    if (outfile.is_open()) {
                        outfile << serv_name << '\0' << url << '\0' << net_cl.token << '\n';
                        outfile.close();
                        cout << "`\nServer added." << endl;
                    } else {
                        cout << "\nError: Impossible to open server_list file" << endl;
                    }
                }
            }
            catch (...) {
                cout << "\nError. The server is either offline or refusing new connection." << endl;
            }
            
        }

        void remove_server(vector<string> args) {
            string server_name = args[0];
            ifstream infile(getHomeDirectory() + "/.config/gac/server_list");
            ofstream outfile(getHomeDirectory() + "/.config/gac/temp_file");
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
                remove((getHomeDirectory() + "/.config/gac/server_list").c_str());
                rename((getHomeDirectory() + "/.config/gac/temp_file").c_str(), (getHomeDirectory() + "/.config/gac/server_list").c_str());
            } else {
                cout << "Error: can not open server_list file and temp_file." << endl;
            }
        }

        void list_server(vector<string> args) {
            ifstream infile(getHomeDirectory() + "/.config/gac/server_list");
            string line;
            if(infile.is_open()){
                while (getline(infile, line)) {
                    string current_server_name = line.substr(0, line.find('\0'));
                    cout << current_server_name << endl;
                }
            }
            cout << "[*] All servers printed." << endl;
        }

        void connect(vector<string> args) {
            cout << "Be sure that the server exists ! If you can not connect or push, it's probably that the name entered is wrong." << endl;
            string server_name = args[0];
            ofstream outfile(getHomeDirectory() + "/.config/gac/act_serv");
            if (outfile.is_open()) {
                outfile << server_name;
                outfile.close();
                cout << "[*] Main server moved to " << server_name << endl;
            } else {
                cout << "Error: can not edit act_server file." << endl;
            }
        }

        void client_plugin_loader(){
            initPlugin("client");
            addCommand("add", &add, "add a server to your server list. Takes the url as an argument (and it's all)",1,1);
            addCommand("remove", &remove_server, "Remove a server from your server list, using his name on your computer.", 1, 1);
            addCommand("connect",&connect, "Set the server passed as an argument to be the server you are working on.",1,1);
            addCommand("server_list", &list_server, "List all your servers.", 0,0);
            addCommand("cts",&copy_to_server_fun, "copy current git to server",1,1);
            addCommand("push",&push_fun, "push",1,1);
            addCommand("pull",&pull_fun, "pull",1,1);
            
        }
    }
}

