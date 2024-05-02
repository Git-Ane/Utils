#include "file_transfer.hpp"
#include "response_builder.hpp"
#include <fstream>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace GitAne::Net
{    

std::string buildReceiveFileResponse(std::string method, std::string args){
    /*
    \brief permet au serveur de recevoir des fichiers.
    \brief c'est la réponse à "send" !
    */
    std::cout << "HEY !";
    
    auto maps= parseQueryString(args);
    bool contiens_token = maps.find("token") != maps.end();
    if(!contiens_token){
        return buildBadRequestPage("File receiver","missing arguments, need <em>token</em>");
    }
    bool contiens_proj_name = maps.find("proj_name") != maps.end();
    if(!contiens_token){
        return buildBadRequestPage("File receiver","missing arguments, need <em>proj_name</em>");
    }

    bool contiens_nom = maps.find("file_name") != maps.end();
    if(!contiens_nom){
        return buildBadRequestPage("File receiver","missing arguments, need <em>file_name</em>");
    }
    bool contiens_content = maps.find("file_content") != maps.end();
    if(!contiens_content){
        return buildBadRequestPage("File receiver","missing arguments, need <em>file_content</em>");
    }
    std::cout << "AAA";
    
    if (!fs::is_directory("./bdd/gacs/" + maps["proj_name"]) || !fs::exists("./bdd/gacs/" + maps["proj_name"])) { // Check if src folder exists
        std::cout << "Le dossier n'existait pas !" << std::endl;
        fs::create_directory("./bdd/gacs/" + maps["proj_name"]); // create src folder
    }
    else{
        std::cout << "Le dossier existait bien."  << std::endl;
    }
    std::ofstream to_write("./bdd/gacs/" + maps["proj_name"] + "/" +  maps["file_name"],std::ios::out);
    if(to_write.is_open()){
        std::cout << "bien ouvert !";
    to_write << maps["file_content"];
    to_write.close();
    return buildOkPage("File Sender", maps["file_name"] + " updated.");
    }
    else{
        return buildInternalServerErrorPage("File Sender", "On a eu un pb la.");
    }

}

std::string buildSendFileResponse(std::string method, std::string args){
    /*
    \brief Permet au serveur d'envoyer des fichiers.
    */
    return "bite";
}

std::string buildLaMuleResponse(std::string method, std::string args){
    return buildOkPage("La Mule", "Permet d'envoyer / recevoir des fichiers du serveur. Via l'url send ou receive. <br/> LaMule a besoin de token, de file_name, de file_content et de token.");
    
}
}