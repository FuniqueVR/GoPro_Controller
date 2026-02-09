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
inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

inline std::string GetRemoteIPBySerial(std::string serial){
    if(serial.size() < 3){
        std::cerr << "Serial string must be at least 3" << "\n";
        return "";
    }

    return std::string("172.2") + serial[0] + std::string(".1") + serial[1] + serial[2] + std::string(".51");
}

inline std::string GetRemoteURLBySerial(std::string serial){
    if(serial.size() < 3){
        std::cerr << "Serial string must be at least 3" << "\n";
        return "";
    }

    return std::string("http://172.2") + serial[0] + std::string(".1") + serial[1] + serial[2] + std::string(".51:8080");
}

inline std::string GetRemoteURLByIP(std::string IP){
    if(IP.size() < 12){
        std::cerr << "The IP string size must larger than 12" << "\n";
        return "";
    }

    return std::string("http://") + IP + std::string(":8080");
}

inline std::string exec(std::string cmd) {
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string result;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cmd.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1500L);

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


inline std::vector<std::string> execs(std::vector<std::string> cmds) {
    CURLM* curlm = curl_multi_init();
    CURL* curl = NULL;
    int32_t still_running = 0;
    int32_t msgs_left = 0;
    int32_t http_status_code = 0;
    CURLMcode resm;
    CURLcode res;
    CURLMsg *msg=NULL;
    std::vector<std::string> result = std::vector<std::string>(cmds.size(), "");

    if(curlm) {
        curl_multi_setopt(curlm, CURLMOPT_MAX_TOTAL_CONNECTIONS, 64L);
        for(int32_t i = 0; i < cmds.size(); i++){
            CURL* curlb = curl_easy_init();
            curl_easy_setopt(curlb, CURLOPT_URL, cmds[i].c_str());
            curl_easy_setopt(curlb, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curlb, CURLOPT_WRITEDATA, &result[i]);
            curl_easy_setopt(curlb, CURLOPT_TIMEOUT_MS, 1500L);
            curl_multi_add_handle(curlm, curlb);
        }

        curl_multi_perform(curlm, &still_running);

        do {
            int32_t numfds = 0;
            resm = curl_multi_wait(curlm, NULL, 0, 1500, &numfds);

            if(res != CURLE_OK) {
                //std::cerr << "GET failed curl_multi_wait: " << numfds << "/" << cmds.size() << std::endl;
            }

            curl_multi_perform(curlm, &still_running);
        } while (still_running);
            
        while((msg = curl_multi_info_read(curlm, &msgs_left))){
            if (msg->msg == CURLMSG_DONE) {
                curl = msg->easy_handle;

                res = msg->data.result;
                if (res != CURLE_OK) {
                    //fprintf(stderr, "CURL error code: %d\n", msg->data.result);
                    continue;
                }

                // Get HTTP status code
                http_status_code=0;
                std::string buffer = "";

                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status_code);
                curl_easy_getinfo(curl, CURLINFO_PRIVATE, buffer);

                if(http_status_code==200) {
                    printf("200 OK for %s\n", buffer);
                } else {
                    fprintf(stderr, "GET of %s returned http status code %d\n", buffer, http_status_code);
                }

                curl_multi_remove_handle(curlm, curl);
                curl_easy_cleanup(curl);
            }
            else {
                fprintf(stderr, "error: after curl_multi_info_read(), CURLMsg=%d\n", msg->msg);
            }
        }

        curl_multi_cleanup(curlm);
    }else{
        std::cerr << "Curl init failed" << std::endl;
    }

    return result;
}

inline std::string get_env_var( std::string const & key ) {                                 
    char * val;                                                                        
    val = getenv( key.c_str() );                                                       
    std::string retval = "";                                                           
    if (val != NULL) {                                                                 
        retval = val;                                                                    
    }                                                                                  
    return retval;                                                                        
}  

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    inline std::string generate_uuid_v4() {
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
