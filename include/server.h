#pragma once

#include "kissnet.hpp"
#include "playerListInitializer.h"

#include <string>
#include <vector>
#include <memory>
#include <mutex>

class JubjubServer{

    private:

        unsigned short int m_maxPlayerCount;
        unsigned short int m_playerCount;
        std::string m_serverIp;
        std::vector<PlayersSocketMap> m_playerSocketMapVector;

        bool m_initComplete;

        std::unique_ptr<PlayerListInitializer> m_plInit;

        std::mutex m_serverAccessMutex;

        std::string discoverIp();

        JubjubServer();

        void checkAndCloseInitializer();

        void updatePlayerCount();



    public:

        static JubjubServer* getInstance();

        void initialize();

        std::string getServerIp() const;

        void updateMaxPlayer(unsigned short int t_maxClient);

        void updateInitComplete(bool t_val);

        unsigned short int currInitPlayers();

};


// #include <iostream>
// #include "kissnet.hpp"

// int main() {
//     // 1. Create a listening socket on port 8080 (IPv4)
//     kissnet::tcp_socket server_listening_socket({"0.0.0.0", 8080});
//     server_listening_socket.bind();
//     server_listening_socket.listen();

//     std::cout << "Server listening on port 8080...\n";

//     // 2. Block and accept an incoming connection
//     kissnet::tcp_socket client_socket = server_listening_socket.accept();
//     std::cout << "Client connected!\n";

//     // 3. Prepare a buffer to store incoming bytes
//     std::array<std::byte, 1024> receive_buffer;

//     // 4. Read data from the client socket
//     auto [bytes_received, status] = client_socket.recv(receive_buffer);

//     if (bytes_received > 0) {
//         // Convert std::byte array back to text for display
//         std::string message(reinterpret_cast<char*>(receive_buffer.data()), bytes_received);
//         std::cout << "Received: " << message << std::endl;
//     }

//     return 0;
// }



// #include <iostream>
// #include <string>
// #include "kissnet.hpp"

// int main() {
//     // 1. Create and connect socket to local server
//     kissnet::tcp_socket client_socket({"127.0.0.1", 8080});
//     client_socket.connect();

//     std::string text_to_send = "Hello from kissnet client!";
    
//     // 2. Convert string data into a pointer of bytes
//     const std::byte* byte_data = reinterpret_cast<const std::byte*>(text_to_send.data());

//     // 3. Send the bytes over TCP
//     auto [bytes_sent, status] = client_socket.send(byte_data, text_to_send.size());

//     std::cout << "Sent " << bytes_sent << " bytes to the server.\n";

//     return 0;
// }
