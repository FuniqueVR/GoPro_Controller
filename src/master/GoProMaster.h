#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "hv/WebSocketClient.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ServerConnection {
    std::string ip;
    hv::WebSocketClient client;
    bool connected = false;
    std::string last_message;
};

class GoProMaster {
public:
    GoProMaster();
    ~GoProMaster();

    void addServer(const std::string& ip);
    void connectAll();
    
    // Commands
    void startRecordingAll();
    void stopRecordingAll();
    void setModePhotoAll();
    void setModeVideoAll();

    // Data for UI
    const std::vector<std::shared_ptr<ServerConnection>>& getServers() const;

private:
    std::vector<std::shared_ptr<ServerConnection>> servers;
    void sendToAll(const std::string& msg);
};
