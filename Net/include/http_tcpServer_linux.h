// Code from a course on medium, rewritten for GitÂne.
// Utils/Net/include/http_tcpServer_linux.cpp
#ifndef base
#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#endif 
#include "HTTPRequest.hpp"
#include "iostream"
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include "url_tree.hpp"
#include <filesystem>
#include <arpa/inet.h> // inet passe d'un char* à un int
#include "http_subsystems/login_and_register.hpp"
#include "http_subsystems/file_transfer.hpp"
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <unordered_map>
#include <set>


namespace GitAne::Net
{    
    
    class TcpServer
    {
    /* Primitives obligatoires */
        public:
            TcpServer(std::string domain, int port);
            ~TcpServer();
            int start();
            int stop();
            int max_curr_conn; // If |users| >= max_curr_conn, all connections refused
            void startListening(); // lance la boucle du serveur
            void stopListening(); // l'arrête
            void acceptConnection(int& new_socket);
            
        
        private:
            int id_socket;
            int new_socket; // quand quelqu'un rejoint on s'en sert pour stocker
            int sockAddr_len;
            int port;
            struct sockaddr_in sockAddr; // stocke le port, l'ip, le protocole,...
            const int BUFFER_SIZE;
            std::string domain;
            std::string serverMessage; // C'est ce que le serveur renvoie sur la page HTML, sera modifiée.
            void sendResponse(std::string msg);

    

    /* Gestion des Plug-ins */
    private:
        UrlTree urlTree;




    };
}
