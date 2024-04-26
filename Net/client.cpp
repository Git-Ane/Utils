#include "include/HTTPRequest.hpp"
#include<iostream>
#include "string.h"
using namespace std;

namespace GitAne{

class NetClient {
    public:
        string server_url;
        NetClient(string s,string e, string m){
            server_url = s;
            email = e;
            cout << "Connexion à " << s << " en tant que " << e;
            
            // Demander au serveur de se connecter avec les informations données
            // En pratique il faudra demander une clé publique au serveur avant d'envoyer le MDP
            http::Request request(s + "/");
            const http::Response loginResponse = request.send("GET", "[GITPARAM]email="+e+"&mdp="+m, {"Content-Type: application/x-www-form-urlencoded"});
            cout << string(loginResponse.body.begin(), loginResponse.body.end()) << '\n'; // print the result
        }
    private:
        string token;
        string email;
    
    

};

}

int main(){
    GitAne::NetClient cTest("localhost:8087","test@test.com","testest");

    return 0;
}