/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>
#include <thread>
#include <future>
#include "../../common/config.h"

std::string GoProController::getMediaList(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        json res;
        try{
            SingleResponse result = _getMediaList(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::future<SingleResponse>> calls = std::vector<std::future<SingleResponse>>();
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        for(std::string ip : buffer){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                return _getMediaList(ip);
            }));
        }

        for(auto& call : calls){
            try{
                SingleResponse result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json i;
            i["ip"] = address;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    return arr.dump();
}

std::string GoProController::getLastMedia(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        try{
            SingleResponse result = _getLastMedia(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["filename"] = res;
        arr.push_back(i);
    }else{
        json res;
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        std::vector<SingleResponse> results = _getAllLastMedia(buffer);
        for(int32_t i = 0; i < results.size(); i++){
            address = results[i].first;
            try{
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json b;
            b["ip"] = address;
            b["filename"] = res;
            arr.push_back(b);
        }
    }
    return arr.dump();
}

std::string GoProController::getFetchURL(std::string target_ip, bool is_local){
    std::cout << "Http GET /last_media " << target_ip << ", " << is_local << std::endl;

    if (target_ip.empty()) {
        std::cerr << "[last_media] " << target_ip << " Missing ip parameter" << std::endl;
        return "";
    }

    try{
        std::string res = exec("http://" + target_ip + ":8080/gopro/media/last_captured");
        if(res.size() == 0) {
            std::cerr << "[last_media] " << target_ip << " IP fetch failed" << std::endl;
            return "";
        }
        json last_data = json::object();
        if(json::accept(res)){
            last_data = json::parse(res);
        }
        
        if(!last_data["file"].is_string() || !last_data["folder"].is_string()){
            std::cerr << "[last_media] " << target_ip << " no last media file" << std::endl;
            return "";
        }
        std::string folder = last_data["folder"].get<std::string>();
        std::string file = last_data["file"].get<std::string>();
        std::cout << "[last_media] get last_media " << target_ip << "/" << folder << "/" << file << std::endl;

        std::string gopro_url = "http://" + target_ip + ":8080/videos/DCIM/" + folder + "/" + file + "?download=true";

        if(is_local){
            std::cout << "[last_media] return value: " << target_ip << " => " << gopro_url << std::endl;
            return gopro_url;
        }else{
            int32_t t = 0;
            std::string download_path = "temp.download";
            while(fs::exists("res/" + download_path)){
                download_path = "temp.download" + std::to_string(t);
                t++;
            }
            std::cout << "[last_media] try download " << gopro_url.c_str() << std::endl;
#ifdef SERVER_MEDIA_DOWNLOAD_LOG
            auto start = std::chrono::high_resolution_clock::now();
#endif
            size_t size = requests::downloadFile(gopro_url.c_str(), ("res/" + download_path).c_str(), [&target_ip, &start](size_t received_bytes, size_t total_bytes){
#ifdef SERVER_MEDIA_DOWNLOAD_LOG
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;
                if(elapsed.count() >= SERVER_MEDIA_DOWNLOAD_PERIOD){
                    start = end;
                    std::cout << "[last_media] download " << target_ip << " " << received_bytes << " / " << total_bytes << std::endl;
                }
#endif
            });
            std::cout << "[last_media] return value: " << target_ip << " => " << download_path << std::endl;
            return download_path;
        }
    }
    catch(const std::exception& ex){
        std::cerr << ex.what() << std::endl;
        return "";
    }
}

std::string GoProController::getThumbnailData(std::string target_ip, std::string path){
    
}
