#include "GoProMaster.h"
#include "../common/iphelper.h"
#include <iostream>

GoProMaster::GoProMaster() {
    
}

GoProMaster::~GoProMaster() {
    for (auto& s : servers) {
        s->client.close();
    }
}

std::string GoProMaster::addServer(const std::string& ip) {
    auto conn = std::make_shared<ServerConnection>();
    conn->ip = ip;
    conn->uuid = uuid::generate_uuid_v4();
    
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
    return conn->uuid;
}

void GoProMaster::reconnectAll() {
    for (auto& s : servers) {
        if (!s->connected) {
            std::cout << "Connecting to " << s->ip << "..." << std::endl;
            // Assuming ws://ip:9090 based on server implementation
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
        }
    }
}

void GoProMaster::disconnectAll(){
    for (auto& s : servers) {
        if (s->connected) {
            std::cout << "Disconnect to " << s->ip << "..." << std::endl;
            s->client.close();
        }
    }
}

void GoProMaster::cleanAll(){
    for(int i = servers.size() - 1; i >=0; --i){
        auto& s = servers[i];
        if(!s->connected){
            std::cout << "Cleaning " << s->ip << "..." << std::endl;
            servers.erase(servers.begin() + i);
        }
    }
}

void GoProMaster::reconnect(const std::string& uuid){
    for(auto& s : servers){
        if(s->uuid == uuid && !s->connected){
            std::cout << "Connecting to " << s->ip << "..." << std::endl;
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
            break;
        }
    }
}

void GoProMaster::disconnect(const std::string& uuid){
    for(auto& s : servers){
        if(s->uuid == uuid && !s->connected){
            std::cout << "Disconnecting to " << s->ip << "..." << std::endl;
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
            break;
        }
    }
}

void GoProMaster::clean(const std::string& uuid){
    for(int i = servers.size() - 1; i >=0; --i){
        auto& s = servers[i];
        if(s->uuid == uuid && !s->connected){
            std::cout << "Cleaning " << s->ip << "..." << std::endl;
            servers.erase(servers.begin() + i);
            break;
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
