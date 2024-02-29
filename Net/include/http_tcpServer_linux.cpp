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
        if(bind(id_socket,(sockaddr *)&sockAddr,sizeof(sockAddr)) < 0){
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

    void TcpServer::acceptConnection(http::Socket &new_socket){

    }

    int TcpServer::stop(){
        close(id_socket);
        return 0;
    }

}