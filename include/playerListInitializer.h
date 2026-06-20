#pragma once

#include "defines.h"
#include "kissnet.hpp"

#include <mutex>
#include <vector>
#include <thread>
#include <string>

class JubjubServer;
struct rcvDataStruct;

class PlayerListInitializer{

    private:

        JubjubServer* m_server;

        unsigned short int m_maxPlayers;

        unsigned short int m_currInitPlayers;
        std::mutex m_currInitPlayersMutex;

        std::vector<PlayersSocketMap> m_playerList;

        bool m_haveEnoughPlayers = false;
        std::mutex m_haveEnoughPlayersMutex;

        std::thread m_acceptThread;
        std::vector<std::thread> m_clienthandleThreads;

        std::mutex m_fillSlotsMutex;        
        

        void connectToClients();

        void handleClient(kissnet::tcp_socket t_clinetSocket);

        bool processReceivedPlayerListData(std::vector<std::string>& t_plList, const unsigned int availableSlots, rcvDataStruct& receivedData);

        void checkAndCloseCliendHandleThreads();

        void incPlayerInitializedSoFar(unsigned short int t_val);

    public:

        PlayerListInitializer(unsigned short int t_maxPlayerCount, JubjubServer* t_server);
        ~PlayerListInitializer();

        std::vector<PlayersSocketMap> extractList();

        unsigned short int getPlayerInitializedSoFar();

        unsigned short int getMaxPlayerCount() const;

        unsigned short int slotsAvailable();

        void addToPlayerList(PlayersSocketMap& t_socMap);

        bool haveEnoughPlayers();

        void updateHaveEnoughPlayers(bool t_val);

        std::vector<std::string> getCopyOfRegisteredPlayerNames();

};

