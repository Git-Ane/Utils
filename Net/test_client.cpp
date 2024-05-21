#include "client.hpp"

int main(){
    GitAne::NetClient c("127.0.0.1:8086","mot_elegant@mot_elagant_bis.com3","bonjour",false,false); // g mis sur false car j'ai déjà login !
    c.sendFile("albama", "BIG ALABAMA", "\n\n WESH WESH CANNE A PÊCHE &&");
    return 0;
}