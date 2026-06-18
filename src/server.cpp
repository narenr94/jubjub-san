#include "server.h"
#include "defines.h"
#include "playerListInitializer.h"
#include "logger.h"

#include "NetIF.hpp"


JubjubServer::JubjubServer():
m_maxPlayerCount(DEFAULT_NUM_OF_PLAYERS),
m_playerCount(0),
m_plInit(nullptr),
m_initComplete(false)
{

    m_serverIp = discoverIp();

    JUBJUB_LOG("Server IP : " + m_serverIp);

}

void JubjubServer::initialize(){

    JUBJUB_LOG("Initializer called with max player : " + std::to_string(m_maxPlayerCount));

    if(m_plInit){
        m_plInit.reset();
    }

    m_plInit = std::make_unique<PlayerListInitializer>(m_maxPlayerCount, this);
    

}

void JubjubServer::updateInitComplete(bool t_val){

    std::lock_guard<std::mutex> lock (m_serverAccessMutex);

    JUBJUB_LOG("m_initComplete updated to : " + std::to_string(t_val));
    m_initComplete = t_val;

    if(m_initComplete){
        checkAndCloseInitializer();
    }
    
}

void JubjubServer::checkAndCloseInitializer(){

    if(m_plInit){
        m_plInit->updateHaveEnoughPlayers(true);
        m_playerSocketMapVector = m_plInit->extractList();
        m_plInit.reset();

        updatePlayerCount();
    }

}

void JubjubServer::updatePlayerCount(){

    std::lock_guard<std::mutex> lock (m_serverAccessMutex);

    m_playerCount = 0;

    for(auto& mp : m_playerSocketMapVector){

        m_playerCount += mp.playerNameVector.size();

    }

}

std::string JubjubServer::discoverIp(){

    try {
       
        auto addrs = gmlc::netif::getInterfaceAddressesV4();

        auto it = std::find_if(addrs.begin(), addrs.end(),
                       [](const std::string& ip){ return ip != "127.0.0.1"; });
        if (it != addrs.end()) {
           JUBJUB_LOG("Chosen IP: " + *it);
        }
        
        return *it;
    }
    catch (...) {
        // Fallback if the machine is completely disconnected from the router

        return "127.0.0.1";
    }

}

void JubjubServer::updateMaxPlayer(unsigned short int t_maxClient){
    if(!m_initComplete){
        m_maxPlayerCount = t_maxClient;
        JUBJUB_LOG("Max player changed to : " + std::to_string(m_maxPlayerCount));
    }
    
}

JubjubServer* JubjubServer::getInstance(){

    static JubjubServer instance;

    return &instance;

}

std::string JubjubServer::getServerIp() const{
    return m_serverIp;
}


unsigned short int JubjubServer::currInitPlayersCount(){

    std::lock_guard<std::mutex> lock (m_serverAccessMutex);

    if((!m_initComplete) && m_plInit){
        
        return m_plInit->getPlayerInitializedSoFar();

    }
    else{
        return m_playerCount;
    }

}