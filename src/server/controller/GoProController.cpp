/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <future>
#include "icmplib.h"

GoProController::GoProController() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    _loadRecord();
}

GoProController::~GoProController() {
    curl_global_cleanup();
    for(auto& thread : scan_workers){
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void GoProController::update(){
    std::thread([&]() {
        while(true){
            std::vector<std::string> fine = std::vector<std::string>();
            std::vector<std::string> buffer = std::vector<std::string>();
            {
                std::lock_guard lock(ips_mutex);
                buffer = std::vector<std::string>(camera_ips.begin(), camera_ips.end());
            }
            
            for(int32_t i = 0; i < buffer.size(); i++){
                auto ping = icmplib::Ping(icmplib::IPAddress(buffer.at(i)), ICMPLIB_TIMEOUT_1S);
                bool connected = ping.delay != ICMPLIB_TIMEOUT_1S || ping.response == icmplib::PingResponseType::Success || ping.response == icmplib::PingResponseType::Unreachable; 
                if(connected){
                    fine.push_back(buffer.at(i));
                }
            }

            {
                std::lock_guard lock(ips_alive_mutex);
                camera_alive_ips = fine;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(ICMPLIB_TIMEOUT_1S));
        }
    }).detach();

    std::thread([&]() {
        while(true){
            if(download_queue.size() != 0){
                std::string target_ip = download_queue.begin()->first.first;
                bool is_local = download_queue.begin()->first.second;
                feedback ff = download_queue.begin()->second;

                {
                    std::lock_guard<std::mutex> lock(download_queue_mutex);
                    download_queue.pop_front();
                }

                std::cout << "Http GET /last_media " << target_ip << ", " << is_local << std::endl;

                if (target_ip.empty()) {
                    std::cerr << "[last_media] " << target_ip << " Missing ip parameter" << std::endl;
                    ff("");
                    return;
                }

                try{
                    std::string res = exec("http://" + target_ip + ":8080/gopro/media/last_captured");
                    if(res.size() == 0) {
                        std::cerr << "[last_media] " << target_ip << " IP fetch failed" << std::endl;
                        ff("");
                        return;
                    }
                    json last_data = json::parse(res);
                    if(!last_data["file"].is_string() || !last_data["folder"].is_string()){
                        std::cerr << "[last_media] " << target_ip << " no last media file" << std::endl;
                        ff("");
                        return;
                    }
                    std::string folder = last_data["folder"].get<std::string>();
                    std::string file = last_data["file"].get<std::string>();
                    std::cout << "[last_media] get last_media " << target_ip << "/" << folder << "/" << file << std::endl;

                    std::string gopro_url = "http://" + target_ip + ":8080/videos/DCIM/" + folder + "/" + file + "?download=true";

                    if(is_local){
                        std::cout << "[last_media] return value: " << target_ip << " => " << gopro_url << std::endl;
                        ff(gopro_url);
                        return;
                    }else{
                        int32_t t = 0;
                        std::string download_path = "temp.download";
                        while(fs::exists("res/" + download_path)){
                            download_path = "temp.download" + std::to_string(t);
                            t++;
                        }
                        std::cout << "[last_media] try download " << gopro_url.c_str() << std::endl;
                        size_t size = requests::downloadFile(gopro_url.c_str(), ("res/" + download_path).c_str(), [&target_ip](size_t received_bytes, size_t total_bytes){
                            std::cout << "[last_media] download " << target_ip << " " << received_bytes << " / " << total_bytes << std::endl;
                        });
                        std::cout << "[last_media] return value: " << target_ip << " => " << download_path << std::endl;
                        ff(download_path);
                        return;
                    }
                }
                catch(const std::exception& ex){
                    std::cerr << ex.what() << std::endl;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(ICMPLIB_TIMEOUT_1S));
        }
    }).detach();
}
