#include "client.hpp"

int main(){
    GitAne::NetClient c("127.0.0.1:8087","kaka@prout.com","prout22",false); // g mis sur false car j'ai déjà login !
    c.sendFile("caca", "BIG PROUT", "\n\n WESH WESH CANNE A PÊCHE &&");
    return 0;
}