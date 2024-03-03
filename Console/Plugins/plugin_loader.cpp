#include "plugin_loader.hpp"

std::vector<std::string> parseConfigFile(const std::string& filename) {
    std::vector<std::string> includes;
    std::ifstream configFile(filename.c_str()); 
    std::cerr << "Attempting to open config file: " << filename << std::endl;
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open config file: " << filename << std::endl;
        std::cerr << "Please verify you are in the Console/Plugins directory" << std::endl;
        return includes;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        // Assuming each line in the config file represents a header file to include
        includes.push_back(line);
    }

    return includes;
}

void generateIncludeDirectives(const std::vector<std::string>& includes, const std::string& outputFilename) {

    if (includes.empty()) {
        std::cerr << "No files to include." << std::endl;
        std::cout << "Do you still want to create an output file ? [Y/N]" <<std::endl;
        string rep;
        std::cin >> rep;
        if(rep!="Y"){return;}
    }


    std::ofstream outputFile(outputFilename.c_str());
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to create output file: " << outputFilename << std::endl;
        return;
    }


    for (size_t i = 0; i < includes.size(); ++i) {
    outputFile << "#include \"" << includes[i] << "\"\n";
    cout << "Include file generated successfully !" << endl;
}

    outputFile.close();
}

void generate_include_file(vector<string> f){
    string conf_file;
    if(f.size()==0){conf_file = "config.txt";}
    else{conf_file = f[0];}
    std::vector<std::string> includedFiles = parseConfigFile(conf_file);
    generateIncludeDirectives(includedFiles, "generated_includes.hpp");
}

namespace GitAne{

    void plugin_loader(){
        //GitAne::Joke::joke_plugin_loader();
    }

}