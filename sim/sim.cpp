#include "server.h"

int main(){

    JubjubServer* server = JubjubServer::getInstance();

    server->initialize();

    return 0;
}