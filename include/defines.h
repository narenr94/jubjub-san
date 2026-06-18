#pragma once

#include "kissnet.hpp"
#include "json.hpp"

#include <string>
#include <vector>
#include <cstddef>
#include <utility>

#define DEFAULT_NUM_OF_PLAYERS 6

#define RECV_BUFFER_SIZE 1024

#define RECV_POLL_DELAY_MS 1000

#define DEFAULT_SERVER_DISCOVERY_PORT 8080

struct PlayersSocketMap{

    kissnet::tcp_socket clientSocket;
    std::vector<std::string> playerNameVector;

    bool isPlayerMember(const std::string& t_name);

};


std::pair<std::vector<std::byte>, unsigned int> jsonToByteStreamConverter (const nlohmann::json& j);