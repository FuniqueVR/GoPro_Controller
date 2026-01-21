#include "GoProMaster.hpp"
#include <iostream>

GoProMaster::GoProMaster() {}

GoProMaster::~GoProMaster() {
    for (auto& s : servers) {
        s->client.close();
    }
}

void GoProMaster::addServer(const std::string& ip) {
    auto conn = std::make_shared<ServerConnection>();
    conn->ip = ip;
    
    conn->client.onopen = [conn]() {
        std::cout << "Connected to server: " << conn->ip << std::endl;
        conn->connected = true;
    };
    conn->client.onmessage = [conn](const std::string& msg) {
        std::cout << "Msg from " << conn->ip << ": " << msg << std::endl;
        conn->last_message = msg;
    };
    conn->client.onclose = [conn]() {
        std::cout << "Disconnected from server: " << conn->ip << std::endl;
        conn->connected = false;
    };

    servers.push_back(conn);
}

void GoProMaster::connectAll() {
    for (auto& s : servers) {
        if (!s->connected) {
            std::cout << "Connecting to " << s->ip << "..." << std::endl;
            // Assuming ws://ip:9090 based on server implementation
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
        }
    }
}

void GoProMaster::sendToAll(const std::string& msg) {
    for (auto& s : servers) {
        if (s->connected) {
            s->client.send(msg);
        }
    }
}

void GoProMaster::startRecordingAll() {
    sendToAll("f2_start_recording");
}

void GoProMaster::stopRecordingAll() {
    sendToAll("f3_stop_recording");
}

void GoProMaster::setModePhotoAll() {
    sendToAll("f4_mode_photo");
}

void GoProMaster::setModeVideoAll() {
    sendToAll("f5_mode_video");
}

const std::vector<std::shared_ptr<ServerConnection>>& GoProMaster::getServers() const {
    return servers;
}
