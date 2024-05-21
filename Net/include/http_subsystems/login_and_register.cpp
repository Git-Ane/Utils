#include "login_and_register.hpp"
#include "../../../SHA-warma/shawarma.hpp"
#include <iostream>
using json = nlohmann::json;
namespace GitAne::Net
{    

// Exemple de page web
std::string buildLoginResponse(std::string method, std::string args){
        /*
        \brief Construction of the Login page
        \param method GET or POST
        \param args query arguments, there is a parseQueryString built-in function.

        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */
        auto maps= parseQueryString(args);
        bool contiens_name = maps.find("name") != maps.end();
        bool contiens_pwd = maps.find("pwd") != maps.end();
        if(!contiens_name || !contiens_pwd){
            return buildBadRequestPage("Login","missing arguments, need <em>name</em> and <em>pwd</em>");
        }
        std::string username= maps["name"];
        std::string pwd = sha1(maps["pwd"]);
        std::ifstream f("./include/bdd/users.json");
        json data = json::parse(f);
        f.close();
        if(data.find(username) == data.end()) return buildUnauthorizedRequestPage("Login","username unknown.");
        else{
            std::string token = data[username]["tok"];
            if( data[username]["pwd"] == pwd) return buildOkPage("Login","Token:" + token);
            else return buildUnauthorizedRequestPage("Login","wrong password.");    
        }

    }
    std::string buildRegisterResponse(std::string method, std::string args){
        /*
        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */

        std::cout << "Arguments reçus: " << args << std::endl;
        auto maps= parseQueryString(args);
        bool contiens_name = maps.find("name") != maps.end();
        bool contiens_pwd = maps.find("pwd") != maps.end();
        if(!contiens_name || !contiens_pwd){
            return buildBadRequestPage("Register","missing arguments, need <em>name</em> and <em>pwd</em>");
        }
        std::string username= maps["name"];
        std::string pwd = sha1(maps["pwd"]);
        std::ifstream f("./include/bdd/users.json");
        json data = json::parse(f);
        f.close();
        if(data.find(username) != data.end()) {
            // l'utilisateur est déjà enregistré
            return buildUnauthorizedRequestPage("Register","username already taken");
        }
        else{
            std::string token = GitAne::Net::sha1(username + pwd); // tema le token incroyable
            data[username] = {
                {"pwd",pwd},
                {"tok",token}
            };
            std::ofstream outputFile("./include/bdd/users.json");
            outputFile << data.dump(2);  // dump(2) pour une mise en forme lisible
            outputFile.close();
            return buildOkPage("Register","Token:" + token);
        }
        

    }
}