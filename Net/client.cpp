#include "include/HTTPRequest.hpp"
#include "client.hpp"
#include<iostream>
#include "string.h"
using namespace std;

namespace GitAne{


        NetClient::NetClient(string s,string e, string m){
            server_url = s;
            email = e;
            cout << "Connexion à " << s << " en tant que " << e;
            
            // Demander au serveur de se connecter avec les informations données
            // En pratique il faudra demander une clé publique au serveur avant d'envoyer le MDP
            http::Request request(s + "/");
            const http::Response loginResponse = request.send("GET", "[GITPARAM]email="+e+"&mdp="+m, {"Content-Type: application/x-www-form-urlencoded"});
            cout << string(loginResponse.body.begin(), loginResponse.body.end()) << '\n'; // print the result
        }


}

