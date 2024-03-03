#include "include/HTTPRequest.hpp"
#include<iostream>
#include "string.h"
#include "include/http_tcpServer_linux.h"

using namespace std;


int main()
{
    std::string a = "127.0.0.1"; // je sais pas encore si on pourra direct mettre des domaines, je testerai sur cr-dev.io, en attendant on met l'ip 127.0.0.1, de touet façon les redirections A marchent donc c'est pas grave.
    GitAne::Net::TcpServer server(a,8087);
    server.startListening();
    return 0;
}
