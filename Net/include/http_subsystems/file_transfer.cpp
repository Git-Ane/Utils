#include "file_transfer.hpp"
#include <fstream>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace GitAne::Net
{    

std::string buildReceiveFileResponse(std::string method, std::string args){
    /*
    \brief permet au serveur de recevoir des fichiers.
    */
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

    std::ofstream to_write("./bdd/gacs/" + maps["proj_name"] + "/" +  maps["file_name"]);
    to_write << maps["file_content"];
    to_write.close();
    buildOkPage("File Sender", maps["file_name"] + " updated.");

}

std::string buildSendFileResponse(std::string method, std::string args){
    /*
    \brief Permet au serveur d'envoyer des fichiers.
    */
}

std::string buildLaMuleResponse(std::string method, std::string args){
    return buildOkPage("La Mule", "Permet d'envoyer / recevoir des fichiers du serveur. Via l'url send ou receive. <br/> LaMule a besoin de token, de file_name, de file_content et de token.");
    
}
}