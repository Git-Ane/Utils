#include "include/HTTPRequest.hpp"
#include "include/http_subsystems/data_manipulations.hpp"
#include<iostream>
#include "string.h"
#include "client.hpp"
#include <curl/curl.h>
#include <iostream>
#include <unordered_map>
using namespace std;


namespace GitAne{

        unordered_map<std::string, std::string> NetClient::parseHTTPResponse(const std::string& httpResponse) {
            std::unordered_map<std::string, std::string> result;

            // Trouver la position de la première ligne vide (marquant la fin des en-têtes HTTP)
            size_t pos = httpResponse.find("\n\n");
            if (pos == std::string::npos) {
                std::cerr << "Erreur : Réponse HTTP malformée." << std::endl;
                return result;
            }

            // Extraire les en-têtes HTTP
            std::string headers = httpResponse.substr(0, pos);

            // Extraire le code d'erreur
            size_t codePos = headers.find(" ");
            if (codePos != std::string::npos) {
                std::string errorCode = headers.substr(codePos + 1, 3);
                result["error_code"] = errorCode;
            }

            // Extraire la taille de la réponse
            size_t contentLengthPos = headers.find("Content-Length: ");
            if (contentLengthPos != std::string::npos) {
                size_t endPos = headers.find("\r\n", contentLengthPos);
                std::string contentLengthStr = headers.substr(contentLengthPos + 16, endPos - contentLengthPos - 16);
                result["content_length"] = contentLengthStr;
            }

            // Extraire le corps de la réponse
            std::string responseBody = httpResponse.substr(pos + 4);
            result["response_body"] = responseBody;

            return result;
        }

        unordered_map<std::string, std::string> NetClient::body_to_args(const std::string& body) {
            std::unordered_map<std::string, std::string> args;

            // Trouver le contenu entre </h1> et </body>
            size_t start_pos = body.find("</h1>");
            if (start_pos == std::string::npos) {
                std::cerr << "Erreur : Balise </h1> non trouvée." << std::endl;
                return args;
            }
            start_pos += 5; // Avancer le curseur après </h1>

            size_t end_pos = body.find("</body>");
            if (end_pos == std::string::npos) {
                std::cerr << "Erreur : Balise </body> non trouvée." << std::endl;
                return args;
            }

            // Extraire le contenu entre </h1> et </body>
            std::string content = body.substr(start_pos, end_pos - start_pos);

            // Parcourir chaque ligne du contenu
            size_t pos = 0;
            while (pos < content.size()) {
                // Trouver la fin de la ligne
                size_t line_end = content.find("\n", pos);
                if (line_end == std::string::npos) {
                    line_end = content.size();
                }

                // Extraire la ligne
                std::string line = content.substr(pos, line_end - pos);

                // Trouver la position du séparateur ":"
                size_t separator_pos = line.find(":");
                if (separator_pos != std::string::npos) {
                    // Extraire la clé et la valeur
                    std::string key = Net::url_decode(line.substr(0, separator_pos));
                    std::string value = Net::url_decode(line.substr(separator_pos + 1));

                    // Ajouter la paire clé-valeur à l'unordered_map
                    args[key] = value;
                }

                // Aller à la prochaine ligne
                pos = line_end + 1;
            }

            return args;
        }
        NetClient::NetClient(string s,string e, string m, bool reg, bool skip){
            if(skip){
                // skip mode is when we already have the token stored in local.
                server_url = s;
                email = e;
                token = m;
                return;
            }
            server_url = s;
            email = e;
            token = "UNSET";
            cout << "Connexion à " << s << " en tant que " << e;
            
            // Demander au serveur de se connecter avec les informations données
            // En pratique il faudra demander une clé publique au serveur avant d'envoyer le MDP
            if(reg){
                http::Request request(s + "/register");
                auto registerResponse = request.send_hotfix("POST", "[GITPARAM]name="+e+"&pwd="+m, {"Content-Type: application/x-www-form-urlencoded"});
                auto resRegister = parseHTTPResponse(registerResponse);
                cout << "Réponse du serveur:\n" << "Code d'erreur: " << resRegister["error_code"] << "\nTaille: " << resRegister["content_length"] << "\n Message: " << resRegister["response_body"] << endl;
                if(resRegister["error_code"] == "200"){
                    cout << "[*] Successfully registered." << endl;
                    auto map = body_to_args(resRegister["response_body"]);
                    token = map["Token"];
                    std::cout << "Token defined as " << token << std::endl;
                }
                else{
                    cout << "[!] Can not register." << endl;
                }
            }
            else{
                http::Request requestLog(s + "/login");
                auto loginresponse = requestLog.send_hotfix("POST", "[GITPARAM]name="+e+"&pwd="+m, {"Content-Type: application/x-www-form-urlencoded"});
                auto resLogin = parseHTTPResponse(loginresponse);
                cout << "Réponse du serveur:\n" << "Code d'erreur: " << resLogin["error_code"] << "\nTaille: " << resLogin["content_length"] << endl;
                if(resLogin["error_code"] == "200"){
                    cout << "[*] Successfuly connected." << endl;
                    auto map = body_to_args(resLogin["response_body"]);
                    token = map["Token"];
                    std::cout << "Token defined as " << token << std::endl;
                }
                else {cout << " [!] Failed to connect.";}
            }
            


        }




        std::string url_encode(const std::string& decoded)
        {
            const auto encoded_value = curl_easy_escape(nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
            std::string result(encoded_value);
            curl_free(encoded_value);
            return result;
        }

        std::string url_decode(const std::string& encoded)
        {
            int output_length;
            const auto decoded_value = curl_easy_unescape(nullptr, encoded.c_str(), static_cast<int>(encoded.length()), &output_length);
            std::string result(decoded_value, output_length);
            curl_free(decoded_value);
            return result;
        }



        std::unordered_map<std::string, std::string> NetClient::sendFile(string proj_name, string file_name, string file_content){
            http::Request requestSend(server_url + "/lamule/send");
            auto sendResponse = requestSend.send_hotfix("POST", "[GITPARAM]token="+token+"&proj_name="+url_encode(proj_name)+"&file_name="+url_encode(file_name)+"&file_content="+url_encode(file_content), {"Content-Type: application/x-www-form-urlencoded"});
            auto resSend = parseHTTPResponse(sendResponse);
            return resSend;
        }

        string NetClient::receiveFile(string proj_name, string file_name){
            http::Request requestSend(server_url + "/lamule/receive");
            auto recResponse = requestSend.send_hotfix("POST", "[GITPARAM]token="+token+"&proj_name="+url_encode(proj_name)+"&file_name="+url_encode(file_name), {"Content-Type: application/x-www-form-urlencoded"});
            auto resRec = parseHTTPResponse(recResponse);
            return resRec["response_body"].substr(63,resRec["response_body"].size()-63-15);
        }


        bool NetClient::file_exists(string proj_name, string file_name){
            http::Request requestSend(server_url + "/lamule/receive");
            auto recResponse = requestSend.send_hotfix("POST", "[GITPARAM]token="+token+"&proj_name="+url_encode(proj_name)+"&file_name="+url_encode(file_name), {"Content-Type: application/x-www-form-urlencoded"});
            auto resRec = parseHTTPResponse(recResponse);
            return resRec["error_code"]=="200";
        }

        














        

    
    

};

