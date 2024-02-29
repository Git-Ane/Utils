// Code from a course on medium, rewritten for GitÂne.
// Utils/Net/include/http_tcpServer_linux.cpp
#include "HTTPRequest.hpp"
#include "iostream"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet passe d'un char* à un int
namespace GitAne::Net
{    class TcpServer
    {
    public:
        TcpServer(std::string domain, int port);
        ~TcpServer();
        int start();
        int stop();
        int max_curr_conn; // If |users| >= max_curr_conn, all connections refused
        void startListening();
        void stopListening();
        void acceptConnection(http::Socket &new_socket);
    
    private:
        int id_socket;
        int port;
        struct sockaddr_in sockAddr;
        std::string domain;

    };
}
