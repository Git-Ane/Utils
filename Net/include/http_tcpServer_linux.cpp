// Code from a course on medium, rewritten for GitÂne.
#include "http_tcpServer_linux.h"
#include "HTTPRequest.hpp"
#include "url_tree.hpp"
#include <cstddef>
#include <cstring>
#include <functional>
#include <sstream>


/*

Still in work. Only support redirection & URL tree yet. Missing:
- Argument parsing (through ?var1=val1?var2=val2.... or GET/POST arg, to decide)
- GET/POST differentiation
- HTML File support to devellop easier definitions
- Database for register/login
- Everything related to 
*/

namespace GitAne::Net
{    

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

    std::string buildDefaultResponse(std::string method, std::string args){
        /*
        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>GitÂne - Main Page </h1><p>If you want to move to a specified plug-in manual, move to [domain]/plugin/[plugin_name]. Everything should be explained there if they respect the conventions.</p><br/><p>If you are developping a plug-in and you can not see your plug-in here, be sure you editted the plugin-list file.</p> <h2> Installed Plugins </h2> <p>Nothing there yet.</p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        std::string s = ss.str();
        return s;
    }

     std::string buildLoginResponse(std::string method, std::string args){
        /*
        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>GitÂne - Login </h1><p>Debug only for now.</p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        std::string s = ss.str();
        return s;
    }
    std::string buildRegisterResponse(std::string method, std::string args){
        /*
        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>GitÂne - Register </h1><p>Debug only for now.</p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        std::string s = ss.str();
        return s;
    }
    std::string buildRegister2Response(std::string method, std::string args){
        /*
        Default print for the API, accessible using only your domain name.
        It (will) print to you all the availables URLs & descriptors (according to the plug-ins).
        */
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>GitÂne - Register2 </h1><p>Debug only for now.</p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        std::string s = ss.str();
        return s;
    }
    /* PRIMITIVES */
    TcpServer::TcpServer(std::string d, int p):
        domain(d), port(p), max_curr_conn(100), BUFFER_SIZE(30720),
        serverMessage(buildDefaultResponse("",""))
    {

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(p); // passage en big-endian
        sockAddr.sin_addr.s_addr = inet_addr(d.c_str());
        urlTree = UrlTree();
        urlTree.addPath("/", buildDefaultResponse);
        urlTree.addPath("/login", buildLoginResponse);
        urlTree.addPath("/register", buildRegisterResponse);
        urlTree.addPath("/register/2", buildRegister2Response);
        start();
    }

    TcpServer::~TcpServer(){
        stop();
    }

    int TcpServer::start()
    {
        id_socket = socket(AF_INET,SOCK_STREAM,0);
        if(id_socket < 0)
        {
            std::cout << "ERREUR: Impossible de créer une socket";
            return 1;
        }
        sockAddr_len = sizeof(sockAddr);
        if(bind(id_socket,(sockaddr *)&sockAddr,sockAddr_len) < 0){
            std::cout << "ERREUR: Impossible de connecter le socket à l'addresse " << domain;
            return 1;
        }
        return 0;
    }

    std::pair<std::string, std::string> urlAndMethod(const char* buffer) {
        const char* firstNewLine = std::strchr(buffer, '\n');
        if (firstNewLine == nullptr) {
            std::cerr << "ERREUR: Format de requête invalide." << std::endl;
            return {"", ""};
        }

        // Extraire la première ligne
        std::string requestLine(buffer, firstNewLine - buffer);

        // Trouver la méthode et l'URL dans la première ligne
        std::istringstream iss(requestLine);
        std::string method, url, protocol;
        iss >> method >> url >> protocol;

        return {method, url};
    }

    

    void TcpServer::stopListening(){

    }

    void TcpServer::acceptConnection(int& new_socket){
        new_socket = accept(id_socket, (sockaddr *)&sockAddr,(socklen_t *)&sockAddr_len);
        if(new_socket < 0) {
            std::ostringstream ss; // https://cplusplus.com/reference/sstream/stringstream/
            ss << 
            "Server failed to accept incoming connection from ADDRESS: " 
            << inet_ntoa(sockAddr.sin_addr) << "; PORT: " 
            << ntohs(sockAddr.sin_port);
            std::cout << (ss.str());
            return;
        }
        else{
           // pas besoin de log car c'est déjà wrap dans la fonction startListening.
        }
    }

    int TcpServer::stop(){
        close(id_socket);
        close(new_socket);
        return 0;
    }

    /* SEULE PARTIE IMPORTANTE  */

    void TcpServer::startListening(){
        if(listen(id_socket,max_curr_conn) < 0) {
            std::cout << "ERREUR: Queue pleine.";
            return;
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " 
        << inet_ntoa(sockAddr.sin_addr) 
        << " PORT: " << ntohs(sockAddr.sin_port) 
        << " ***\n\n";
        std::cout << (ss.str());


        int bytesReceived;
        while(true){
            acceptConnection(new_socket); // va attendre que quelqu'un se log
            char buffer[BUFFER_SIZE] = {0}; // l'erreur c'est un 'may' qui se produit pas car la constante est bien set dans le constructeur.
            bytesReceived = read(new_socket,buffer,BUFFER_SIZE); // les infos qu'il a donné
            if(bytesReceived < 0) {
                std::cout << "ERREUR: Bytes reçues invalides.";
                return; // le serveur arrête de lire les requêtes.
            }
            std::ostringstream ss;
            ss << "Received:\n " << buffer;
            std::cout << ss.str();
            auto [method,url] = urlAndMethod(buffer);
            std::string params = extractGitParams(buffer);
            try{
                std::function<std::string(std::string method, std::string args)> associatedFun = urlTree.getActionForUrl(url);
                if(associatedFun == nullptr) {
                    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1>GitÂne - 404, looks like you are lost. </h1><p>TODO: Add explaination about where to go for what.</p><br/> <h2> Installed Plugins </h2> <p>Nothing there yet.</p></body></html>";
                    std::ostringstream ss;
                    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
                    << htmlFile;
                    sendResponse(ss.str());
                    

                }
                else {
                std::string m = (urlTree.getActionForUrl(url)) (method,params);
                sendResponse(m);

                }
                
            }
            catch (const std::bad_function_call& e) {
                    std::cerr << e.what();
            };
            close(new_socket);
        }

    }

    void TcpServer::sendResponse(std::string msg) {
        long bytesSent;
        bytesSent = write(new_socket, msg.c_str(), msg.size());

        if (bytesSent == msg.size())
        {
            std::cout << ("------ Server Response sent to client ------\n\n");
        }
        else
        {
            std::cout << ("Error sending response to client");
        }
        return;
    }

}