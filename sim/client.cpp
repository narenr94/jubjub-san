#include "kissnet.hpp"

#include "json.hpp"



#include <iostream>
#include <string>
#include <cstddef>
#include <vector>

using json = nlohmann::json;

std::pair<std::vector<std::byte>, unsigned int> clientJsonToByteStreamConverter (const nlohmann::json& j) {
    std::string dumped = j.dump();
    std::vector<std::byte> buffer(dumped.size());
    std::memcpy(buffer.data(), dumped.data(), dumped.size());
    return { std::move(buffer), static_cast<unsigned int>(dumped.size()) };
}

int main(int argc, const char* argv[]){

    if(argc > 2){
        exit(0);
    }

    kissnet::tcp_socket client_socket({argv[1], 8080});

    client_socket.connect();

    std::array<std::byte, 1024> receiveBuffer;

    auto [recv_bytes, rcv_status] = client_socket.recv(receiveBuffer);

    unsigned int  max_players = 0;

    if (recv_bytes > 0){

        std::string message(reinterpret_cast<char*>(receiveBuffer.data()), recv_bytes);

        std::cout << message << std::endl;

        json j = json::parse(message);
        max_players = j["players"].get<unsigned int>();

    }

    json j;
    j["player_list"] = {"a", "b", "c"};

    auto [byte_data, size] = clientJsonToByteStreamConverter(j);


    auto [bytes_sent, status] = client_socket.send(byte_data.data(), size);;

    std::cout << "Sent " << bytes_sent << " bytes to the server.\n";

    std::cout << "Sent " << j.dump() << std::endl;

    while(true){

    }



    // // 2. Convert string data into a pointer of bytes
    // const std::byte* byte_data = reinterpret_cast<const std::byte*>(text_to_send.data());

    // // 3. Send the bytes over TCP
    // auto [bytes_sent, status] = client_socket.send(byte_data, text_to_send.size());

    // std::cout << "Sent " << bytes_sent << " bytes to the server.\n";

    return 0;

}