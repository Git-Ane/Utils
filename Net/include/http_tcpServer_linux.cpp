// Code from a course on medium, rewritten for GitÂne.
#include "http_tcpServer_linux.h"
#include "HTTPRequest.hpp"
#include <sstream>

namespace GitAne::Net
{    
    TcpServer::TcpServer(std::string d, int p):
        domain(d), port(p), max_curr_conn(100)
    {

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(p); // passage en big-endian
        sockAddr.sin_addr.s_addr = inet_addr(d.c_str());
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

    }

    void TcpServer::stopListening(){

    }

    void TcpServer::acceptConnection(int& new_socket){
        new_socket = accept(id_socket, (sockaddr *)&sockAddr,(socklen_t *)&sockAddr_len);
        if(new_socket < 0) {
            std::ostringstream ss;
            ss << 
            "Server failed to accept incoming connection from ADDRESS: " 
            << inet_ntoa(sockAddr.sin_addr) << "; PORT: " 
            << ntohs(sockAddr.sin_port);
            std::cout << (ss.str());
            return;
        }
    }

    int TcpServer::stop(){
        close(id_socket);
        close(new_socket);
        return 0;
    }

}