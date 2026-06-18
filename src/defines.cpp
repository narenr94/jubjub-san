#include "defines.h"


using json = nlohmann::json;



std::pair<std::vector<std::byte>, unsigned int> jsonToByteStreamConverter (const nlohmann::json& j) {
    std::string dumped = j.dump();
    std::vector<std::byte> buffer(dumped.size());
    std::memcpy(buffer.data(), dumped.data(), dumped.size());
    return { std::move(buffer), static_cast<unsigned int>(dumped.size()) };
}


bool PlayersSocketMap::isPlayerMember(const std::string& t_name){

    for(auto& pl : playerNameVector){
        if(pl == t_name){
            return true;
        }
    }

    return false;

}