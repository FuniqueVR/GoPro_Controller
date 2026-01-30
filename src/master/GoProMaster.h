#pragma once

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include "hv/WebSocketClient.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ConvertState {
    uint32_t resolution;
    uint32_t fps;
};

struct CameraInfo {
    std::string ip;
    std::string server;
    json state;
};

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

    std::string addServer(const std::string& ip);
    void reconnectAll();
    void disconnectAll();
    void cleanAll();

    void reconnect(const std::string& ip);
    void disconnect(const std::string& ip);
    void clean(const std::string& ip);

    // Commands
    void command_only(const std::string command, std::string target = "");
    void command_only(const std::string camera, const std::string command, std::string target = "");
    void query_only(const std::string command, std::string target = "");
    void query_only(const std::string camera, const std::string command, std::string target = "");
    void startRecordingAll();
    void stopRecordingAll();
    void setModePhotoAll();
    void setModeVideoAll();

    std::mutex server_mtx;

    // Data for UI
    const std::vector<std::shared_ptr<CameraInfo>>& getCameras() const;
    const std::vector<std::shared_ptr<ServerConnection>>& getServers() const;
private:
    std::vector<std::shared_ptr<CameraInfo>> cameras;
    std::vector<std::shared_ptr<ServerConnection>> servers;
    std::thread t1;

    void update();
    void processMessage(const std::string& ip, const std::string& msg);
    void sendToAll(const std::string& msg);
    void cleanCameraFromServer(const std::string ip);

public:
    int32_t findCamera(const std::string ip);
    int32_t findServer(const std::string ip);
    int32_t findCamera(const std::string server, const std::string ip);
};
