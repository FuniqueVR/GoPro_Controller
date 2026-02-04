/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#ifndef IPHELPER_H
#define IPHELPER_H
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <random>
#include <sstream>
#include <curl/curl.h>

// 1. Add this callback outside the function
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string GetRemoteIPBySerial(std::string serial){
    if(serial.size() != 3){
        std::cerr << "Serial string must be at least 3" << "\n";
        return "";
    }

    return std::string("172.2") + serial[0] + std::string(".1") + serial[1] + serial[2] + std::string(".51");
}

std::string GetRemoteURLBySerial(std::string serial){
    if(serial.size() != 3){
        std::cerr << "Serial string must be at least 3" << "\n";
        return "";
    }

    return std::string("http://172.2") + serial[0] + std::string(".1") + serial[1] + serial[2] + std::string(".51:8080");
}

std::string GetRemoteURLByIP(std::string IP){
    if(IP.size() < 12){
        std::cerr << "The IP string size must larger than 12" << "\n";
        return "";
    }

    return std::string("http://") + IP + std::string(":8080");
}

std::string exec(std::string cmd) {
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string result;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cmd.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1500L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "GET failed: " << curl_easy_strerror(res) << std::endl;
            result.clear();
        }
            
        curl_easy_cleanup(curl);
    }else{
        std::cerr << "Curl init failed" << std::endl;
    }

    return result;
}

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
}
#endif // IPHELPER_H