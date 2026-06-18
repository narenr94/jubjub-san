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

        unsigned short int currInitPlayersCount();

};


