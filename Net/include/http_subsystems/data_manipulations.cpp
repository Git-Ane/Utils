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
                std::string key = url_decode(token.substr(0, pos));
                std::string value = url_decode(token.substr(pos + 1));
                result[key] = value;
            }
        }

        return result;
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
}