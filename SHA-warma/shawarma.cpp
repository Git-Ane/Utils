#include "shawarma.hpp"
namespace GitAne{
    /*
    IDEE:
    Si on veut hasher CACA et que c'est:
    CACA
        /prout
        /PIPI
            /urinoire
    On le voit comme un arbre et on stocke pour chacun
    son hash et on en déduit celui de son père.
    */
    double hashing_file(std::string path, std::string content){
        return 0.;
    }

    double hashing_folder(std::string path){
        return 0.;
    }

    std::string sha1(const std::string &input) {
        /*
        Trivialité, pas besoin d'expliquer hein.
        */
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), hash);

        std::stringstream ss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return ss.str();
    }

   
}
