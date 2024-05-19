#include "client.hpp"

int main(){
    GitAne::NetClient c("127.0.0.1:8086","kaka@prout.com3","prout22",false,false); // g mis sur false car j'ai déjà login !
    c.sendFile("caca", "BIG PROUT", "\n\n WESH WESH CANNE A PÊCHE &&");
    return 0;
}