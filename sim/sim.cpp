#include "server.h"
#include <thread>
#include <iostream>
#include <string>
#include <vector>

#define PRINT(text) do { \
    std::cout << text << std::endl; \
} while(0)


enum class serverCmd{

    exit,
    finish_init,
    print_curr_players_count,
    print_curr_players_name,
    print_max_player_count

};

void cmdParser(JubjubServer* server){

    while(true){

        std::string cmd;

        PRINT("Enter Command:");
        std::cin >> cmd;
        PRINT("cmd entered:" + cmd + " size:" + std::to_string(cmd.size()));

        if(cmd == "exit"){

            PRINT("exiting!!!");
            return;
        }
        else if(cmd == "finish_init"){
            PRINT("finishisng initilaization!!!");
            server->updateInitComplete(true);
        }
        else if(cmd == "print_curr_players_count"){
            PRINT("print_curr_players_count!!!");
            PRINT("count:" + std::to_string(server->currInitPlayersCount()));
        }
        else if(cmd == "print_curr_players_name"){
            PRINT("print_curr_players_name!!!");
            std::vector<std::string> names = server->getAllPlayersNames();
            for(auto& nm : names){
                PRINT(nm);
            }
        }
        else if(cmd == "print_max_player_count"){
            PRINT("print_max_player_count!!!");
            PRINT("max player count:" + std::to_string(server->getMaxPlayersCount()));
        }
        else{
            PRINT("Invalid cmd!!!");
        }

    }

}

int main(){

    JubjubServer* server = JubjubServer::getInstance();

    server->initialize();

    std::thread t1(&cmdParser, server);

    while(true){
        
    }

    return 0;
}