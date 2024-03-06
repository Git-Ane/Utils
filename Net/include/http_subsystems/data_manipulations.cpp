#include "data_manipulations.hpp"


namespace GitAne::Net{
    std::string extractGitParams(const std::string& input) {
        /*
        * Extrait les paramètres d'une requête
        */
        std::istringstream iss(input);
        std::string line;

        while (std::getline(iss, line)) {
            if (line.find("[GITPARAM]") == 0) {
                return line.substr(10);
            }
        }
        return ""; 
    }
    std::map<std::string, std::string> parseQueryString(const std::string& queryString) {
        std::map<std::string, std::string> result;
        std::istringstream iss(queryString);

        std::string token;
        while (std::getline(iss, token, '&')) {
            size_t pos = token.find('=');
            if (pos != std::string::npos) {
                std::string key = token.substr(0, pos);
                std::string value = token.substr(pos + 1);
                result[key] = value;
            }
        }

        return result;
    }
}