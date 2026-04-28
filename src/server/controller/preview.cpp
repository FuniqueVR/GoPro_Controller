/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

void GoProController::previewOn(std::string target, int32_t port){
    if(target.size() > 0) _previewOn(target, port); 
    else {
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        _previewAllOn(buffer, port);
    }
}

void GoProController::previewOff(std::string target){
    if(target.size() > 0) _previewOff(target); 
    else {
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        _previewAllOff(buffer);
    }
}
