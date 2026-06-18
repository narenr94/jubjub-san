#include "playerListInitializer.h"
#include "server.h"
#include "logger.h"

#include <thread>
#include <array>

using json = nlohmann::json;

struct rcvDataStruct{

    std::array<std::byte, RECV_BUFFER_SIZE> data;
    unsigned int size;

};

PlayerListInitializer::PlayerListInitializer(unsigned short int t_maxPlayerCount, JubjubServer* t_server):
m_maxPlayers(t_maxPlayerCount),
m_currInitPlayers(0),
m_server(t_server)
{
    m_acceptThread = std::thread(&PlayerListInitializer::connectToClients, this);
    
}

PlayerListInitializer::~PlayerListInitializer(){

    if(!haveEnoughPlayers()){
        updateHaveEnoughPlayers(true);
    }

    if(m_acceptThread.joinable()){
        m_acceptThread.join();
    }

    
}

void PlayerListInitializer::connectToClients(){

    kissnet::tcp_socket server_listening_socket({"0.0.0.0", DEFAULT_SERVER_DISCOVERY_PORT});
    server_listening_socket.bind();
    server_listening_socket.listen();

    do{

        server_listening_socket.set_non_blocking();
        kissnet::tcp_socket client_socket;

        while (!haveEnoughPlayers()) {
            client_socket = server_listening_socket.accept();
            std::this_thread::sleep_for(std::chrono::milliseconds(RECV_POLL_DELAY_MS));
        }

        if(haveEnoughPlayers()){
            break;
        }        

              
        // Start thread for this client
        m_clienthandleThreads.emplace_back(&PlayerListInitializer::handleClient, this, std::move(client_socket));

    }while(!haveEnoughPlayers());
    

}

bool PlayerListInitializer::processReceivedPlayerListData(std::vector<std::string>& t_plList, const unsigned int availableSlots, rcvDataStruct& receivedData){

    unsigned int size_pl = 0;

    if(!receivedData.size){
        throw std::runtime_error("No data received!!!");
    }

    std::string rcvd_str = std::string(reinterpret_cast<char*>(receivedData.data.data()), receivedData.size);
    json rcvd_j(rcvd_str);

    if(rcvd_j.contains("player_list")){
        if(rcvd_j["player_list"].is_array()){
            size_pl = rcvd_j["player_list"].size();
        }
        else{
            throw std::runtime_error("received unexpected format!!!");
        }
    }
    else{
        throw std::runtime_error("required key:player_list, not found!!!");
    }

    if(size_pl <= availableSlots){

        //check if any player name overlap exists
        {
            std::lock_guard<std::mutex> lock(m_fillSlotsMutex);

            for(auto& newPl : rcvd_j["player_list"]){
                for(auto& exPl : m_playerList){
                    if(exPl.isPlayerMember(newPl)){
                        return false;
                    }
                }
            }
        }

        t_plList.clear();

        for(auto& pl : rcvd_j["player_list"]){
            t_plList.push_back(pl);
        }

        return true;
    }
    else{
        return false;
    }

}

unsigned short int PlayerListInitializer::slotsAvailable(){

    return getMaxPlayerCount() - getPlayerInitializedSoFar();

}

void PlayerListInitializer::handleClient(kissnet::tcp_socket t_clientSocket) {
    json j;
    std::vector<std::string> plList;

    while (true) {
        unsigned short int slotsAvailable;
        slotsAvailable = this->slotsAvailable();

        if (slotsAvailable == 0 || haveEnoughPlayers()) {
            break;
        }

        j["attribute"] = "avail_slots";
        j["players"] = slotsAvailable;

        auto [byte_data, size] = jsonToByteStreamConverter(j);
        auto [bytes_sent, send_status] = t_clientSocket.send(byte_data.data(), size);

        std::array<std::byte, RECV_BUFFER_SIZE> receiveBuffer;
        unsigned int bytes_received = 0;
        t_clientSocket.set_non_blocking(); 

        while (!haveEnoughPlayers()) {
            auto [recv_bytes, rcv_status] = t_clientSocket.recv(receiveBuffer);
            if (recv_bytes) {
                bytes_received = recv_bytes;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(RECV_POLL_DELAY_MS));
        }

        if (haveEnoughPlayers()) {
            break;
        }

        bool validData = false;

        try{
            rcvDataStruct rcvData;
            rcvData.data = receiveBuffer;
            rcvData.size = bytes_received;
            validData = processReceivedPlayerListData(plList, slotsAvailable, rcvData);
        }catch (const std::exception& ex) {
            // Log the error, but don’t crash the thread
            JUBJUB_LOG("Error processing player list: " + std::string(ex.what()));
            // Optionally: continue loop, or break depending on severity
            continue;
        }

        

        slotsAvailable = this->slotsAvailable();

        if (validData && plList.size() <= slotsAvailable) {
            PlayersSocketMap socMap;
            socMap.playerNameVector = plList;
            socMap.clientSocket = std::move(t_clientSocket);
            addToPlayerList(socMap);
            break;
        }
    }
}

void PlayerListInitializer::addToPlayerList(PlayersSocketMap& t_socMap){

    incPlayerInitializedSoFar(t_socMap.playerNameVector.size());

    {
        std::lock_guard<std::mutex> lock(m_fillSlotsMutex);
        m_playerList.push_back(std::move(t_socMap));

    }

    

    if(m_currInitPlayers >= m_maxPlayers){
        updateHaveEnoughPlayers(true);
        m_server->updateInitComplete(true);
    }

}



bool PlayerListInitializer::haveEnoughPlayers(){

    std::lock_guard<std::mutex> lock (m_haveEnoughPlayersMutex);
    return m_haveEnoughPlayers;

}

void PlayerListInitializer::updateHaveEnoughPlayers(bool t_val){
    std::lock_guard<std::mutex> lock (m_haveEnoughPlayersMutex);
    m_haveEnoughPlayers = t_val;

    if(m_haveEnoughPlayers){
        checkAndCloseCliendHandleThreads();
    }
}

void PlayerListInitializer::checkAndCloseCliendHandleThreads(){
    for(auto& th : m_clienthandleThreads){
        if(th.joinable()){
            th.join();
        }
    }
}

unsigned short int PlayerListInitializer::getPlayerInitializedSoFar(){
    std::lock_guard<std::mutex> lock (m_currInitPlayersMutex);
    return m_currInitPlayers;
}

void PlayerListInitializer::incPlayerInitializedSoFar(unsigned short int t_val){
    std::lock_guard<std::mutex> lock (m_currInitPlayersMutex);
    m_currInitPlayers += t_val;
    if(m_currInitPlayers > m_maxPlayers){
        throw std::runtime_error("Exceeded max players!!!!");
    }
}

unsigned short int PlayerListInitializer::getMaxPlayerCount() const {
    return m_maxPlayers;
}

std::vector<PlayersSocketMap> PlayerListInitializer::extractList(){
    std::lock_guard<std::mutex> lock(m_fillSlotsMutex);
    return std::move(m_playerList);
}