#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<sstream>

namespace GitAne{
    class ConfigParser{
    private:
        std::map<std::string, std::map<std::string,std::string>> config;
        void parse(std::string filename);
        // nom de section -> (clé, valeur).
        // ça permet d'avoir des sections dans le.ini
    public:
        ConfigParser(std::string filename);
        std::string get(std::string section, std::string key);

    };
    

}