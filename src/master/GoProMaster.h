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
    std::string uuid;
    hv::WebSocketClient client;
    bool connected = false;
    std::string last_message;
};

class GoProMaster {
public:
    GoProMaster();
    ~GoProMaster();

    std::string addServer(const std::string& ip);
    void reconnectAll();
    void disconnectAll();
    void cleanAll();

    void reconnect(const std::string& uuid);
    void disconnect(const std::string& uuid);
    void clean(const std::string& uuid);

    
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
