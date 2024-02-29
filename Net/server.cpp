#include "include/HTTPRequest.hpp"
#include<iostream>
#include "string.h"
#include "include/http_tcpServer_linux.h"

using namespace std;
// TODO: mettre sous forme de classe

int main()
{
    std::string a = "0.0.0.0.0";
    GitAne::Net::TcpServer server(a,8087);
    server.startListening();
    return 0;
}
