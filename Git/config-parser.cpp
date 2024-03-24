#include "config-parser.hpp"

namespace GitAne{
    
    ConfigParser::ConfigParser(std::string f){
        ConfigParser::parse(f); // ça va renvoyer une erreur si c'est pas le bon format
    }
    void ConfigParser::parse(std::string f){
        std::ifstream file(f);
        std::string line;
        std::string section;
        if(!file.is_open()){
            std::cerr << "Error: " << f << " not found.";
        }
        while(std::getline(file,line)){
            line.erase(0,line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            if(line.empty() || line[0] == ';') continue;
            if(line[0] == '[' && line[line.length() -1] == ']'){
                section = line.substr(1,line.length() - 2);
            }
            else{
                std::istringstream iss(line);
                std::string key,value;
                std::getline(iss,key,'='); // ça met tout jusqu'à = dans la variable key
                std::getline(iss,value); // ça met tout le reste ici

                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                if(!section.empty()) config[section][key] = value;
                else config["global"][key] = value;
            }
        }
        file.close();
    }

    std::string ConfigParser::get(std::string section, std::string key){
        auto it = config.find(section);
        if(it != config.end()){
            auto sectionMap = it->second;
            auto keyIt = sectionMap.find(key);
            if(keyIt != sectionMap.end()){
                return keyIt->second;
            }
        }
        return "";
    }

    

}
